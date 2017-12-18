/*
 * QEMU Geforce NV2A implementation
 *
 * Copyright (c) 2012 espes
 * Copyright (c) 2015 Jannik Vogel
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 or
 * (at your option) version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
// #include "hw/hw.h"
// #include "hw/i386/pc.h"
// #include "ui/console.h"
// #include "hw/pci/pci.h"
// #include "ui/console.h"
// #include "hw/display/vga.h"
// #include "hw/display/vga_int.h"
#include "queue.h"
// #include "qemu/thread.h"
// #include "qapi/qmp/qstring.h"
// #include "gl/gloffscreen.h"
// #include "gl/glextensions.h"



#include <SDL.h>
#include <GL/glew.h>
#include <assert.h>
#include <util.h>
#include <log.h>
#include <stdlib.h>

#define true 1
#define false 0
typedef int bool;
typedef uint32_t hwaddr;
typedef int qemu_irq;
#define qemu_mutex_lock SDL_LockMutex
#define qemu_mutex_unlock SDL_UnlockMutex
typedef int MemoryRegion;

#define pci_irq_assert(...)
#define pci_irq_deassert(...)
#define ldl_le_p(p) (*((uint32_t*)(p)))
// #define g_malloc0(size) calloc(1, size)
// #define g_free(ptr) free(ptr)


#include "g-lru-cache.h"
#include "swizzle.h"
#include "nv2a_shaders.h"
#include "nv2a_debug.h"
// #include "nv2a.h"
#include "nv2a_int.h"


#define USE_TEXTURE_CACHE

#define GET_MASK(v, mask) (((v) & (mask)) >> (ffs(mask)-1))

#define SET_MASK(v, mask, val) ({                                    \
        const unsigned int __val = (val);                             \
        const unsigned int __mask = (mask);                          \
        (v) &= ~(__mask);                                            \
        (v) |= ((__val) << (ffs(__mask)-1)) & (__mask);              \
    })

#define CASE_4(v, step)                                              \
    case (v):                                                        \
    case (v)+(step):                                                 \
    case (v)+(step)*2:                                               \
    case (v)+(step)*3


#define NV2A_DEVICE(obj) \
    OBJECT_CHECK(NV2AState, (obj), "nv2a")

void reg_log_read(int block, hwaddr addr, uint64_t val);
void reg_log_write(int block, hwaddr addr, uint64_t val);


enum FifoMode {
    FIFO_PIO = 0,
    FIFO_DMA = 1,
};

enum FIFOEngine {
    ENGINE_SOFTWARE = 0,
    ENGINE_GRAPHICS = 1,
    ENGINE_DVD = 2,
};

typedef struct DMAObject {
    unsigned int dma_class;
    unsigned int dma_target;
    hwaddr address;
    hwaddr limit;
} DMAObject;

typedef struct VertexAttribute {
    bool dma_select;
    hwaddr offset;

    /* inline arrays are packed in order?
     * Need to pass the offset to converted attributes */
    unsigned int inline_array_offset;

    float inline_value[4];

    unsigned int format;
    unsigned int size; /* size of the data type */
    unsigned int count; /* number of components */
    uint32_t stride;

    bool needs_conversion;
    uint8_t *converted_buffer;
    unsigned int converted_elements;
    unsigned int converted_size;
    unsigned int converted_count;

    float *inline_buffer;

    GLint gl_count;
    GLenum gl_type;
    GLboolean gl_normalize;

    GLuint gl_converted_buffer;
    GLuint gl_inline_buffer;
} VertexAttribute;

typedef struct Surface {
    bool draw_dirty;
    bool buffer_dirty;
    bool write_enabled_cache;
    unsigned int pitch;

    hwaddr offset;
} Surface;

typedef struct SurfaceShape {
    unsigned int z_format;
    unsigned int color_format;
    unsigned int zeta_format;
    unsigned int log_width, log_height;
    unsigned int clip_x, clip_y;
    unsigned int clip_width, clip_height;
    unsigned int anti_aliasing;
} SurfaceShape;

typedef struct TextureShape {
    bool cubemap;
    unsigned int dimensionality;
    unsigned int color_format;
    unsigned int levels;
    unsigned int width, height, depth;

    unsigned int min_mipmap_level, max_mipmap_level;
    unsigned int pitch;
} TextureShape;

typedef struct TextureKey {
    TextureShape state;
    uint64_t data_hash;
    uint8_t* texture_data;
    uint8_t* palette_data;
} TextureKey;

typedef struct TextureBinding {
    GLenum gl_target;
    GLuint gl_texture;
    unsigned int refcnt;
} TextureBinding;

typedef struct KelvinState {
    hwaddr dma_notifies;
    hwaddr dma_state;
    hwaddr dma_semaphore;
    unsigned int semaphore_offset;
} KelvinState;

typedef struct ContextSurfaces2DState {
    hwaddr dma_image_source;
    hwaddr dma_image_dest;
    unsigned int color_format;
    unsigned int source_pitch, dest_pitch;
    hwaddr source_offset, dest_offset;

} ContextSurfaces2DState;

typedef struct ImageBlitState {
    hwaddr context_surfaces;
    unsigned int operation;
    unsigned int in_x, in_y;
    unsigned int out_x, out_y;
    unsigned int width, height;

} ImageBlitState;

typedef struct GraphicsObject {
    uint8_t graphics_class;
    union {
        ContextSurfaces2DState context_surfaces_2d;

        ImageBlitState image_blit;

        KelvinState kelvin;
    } data;
} GraphicsObject;

typedef struct GraphicsSubchannel {
    hwaddr object_instance;
    GraphicsObject object;
    uint32_t object_cache[5];
} GraphicsSubchannel;

typedef struct GraphicsContext {
    bool channel_3d;
    unsigned int subchannel;
} GraphicsContext;


typedef struct PGRAPHState {
    SDL_mutex * lock;

    uint32_t pending_interrupts;
    uint32_t enabled_interrupts;
    SDL_cond * interrupt_cond;

    hwaddr context_table;
    hwaddr context_address;


    unsigned int trapped_method;
    unsigned int trapped_subchannel;
    unsigned int trapped_channel_id;
    uint32_t trapped_data[2];
    uint32_t notify_source;

    bool fifo_access;
    SDL_cond * fifo_access_cond;

    SDL_cond * flip_3d;

    unsigned int channel_id;
    bool channel_valid;
    GraphicsContext context[NV2A_NUM_CHANNELS];

    hwaddr dma_color, dma_zeta;
    Surface surface_color, surface_zeta;
    unsigned int surface_type;
    SurfaceShape surface_shape;
    SurfaceShape last_surface_shape;

    hwaddr dma_a, dma_b;
    GLruCache *texture_cache;
    bool texture_dirty[NV2A_MAX_TEXTURES];
    TextureBinding *texture_binding[NV2A_MAX_TEXTURES];

    GHashTable *shader_cache;
    ShaderBinding *shader_binding;

    bool texture_matrix_enable[NV2A_MAX_TEXTURES];

    /* FIXME: Move to NV_PGRAPH_BUMPMAT... */
    float bump_env_matrix[NV2A_MAX_TEXTURES-1][4]; /* 3 allowed stages with 2x2 matrix each */

    SDL_GLContext *gl_context;
    GLuint gl_framebuffer;
    GLuint gl_color_buffer, gl_zeta_buffer;
    GraphicsSubchannel subchannel_data[NV2A_NUM_SUBCHANNELS];

    hwaddr dma_report;
    hwaddr report_offset;
    bool zpass_pixel_count_enable;
    unsigned int zpass_pixel_count_result;
    unsigned int gl_zpass_pixel_count_query_count;
    GLuint* gl_zpass_pixel_count_queries;

    hwaddr dma_vertex_a, dma_vertex_b;

    unsigned int primitive_mode;

    bool enable_vertex_program_write;

    uint32_t program_data[NV2A_MAX_TRANSFORM_PROGRAM_LENGTH][VSH_TOKEN_SIZE];

    uint32_t vsh_constants[NV2A_VERTEXSHADER_CONSTANTS][4];
    bool vsh_constants_dirty[NV2A_VERTEXSHADER_CONSTANTS];

    /* lighting constant arrays */
    uint32_t ltctxa[NV2A_LTCTXA_COUNT][4];
    bool ltctxa_dirty[NV2A_LTCTXA_COUNT];
    uint32_t ltctxb[NV2A_LTCTXB_COUNT][4];
    bool ltctxb_dirty[NV2A_LTCTXB_COUNT];
    uint32_t ltc1[NV2A_LTC1_COUNT][4];
    bool ltc1_dirty[NV2A_LTC1_COUNT];

    // should figure out where these are in lighting context
    float light_infinite_half_vector[NV2A_MAX_LIGHTS][3];
    float light_infinite_direction[NV2A_MAX_LIGHTS][3];
    float light_local_position[NV2A_MAX_LIGHTS][3];
    float light_local_attenuation[NV2A_MAX_LIGHTS][3];

    VertexAttribute vertex_attributes[NV2A_VERTEXSHADER_ATTRIBUTES];

    unsigned int inline_array_length;
    uint32_t inline_array[NV2A_MAX_BATCH_LENGTH];
    GLuint gl_inline_array_buffer;

    unsigned int inline_elements_length;
    uint32_t inline_elements[NV2A_MAX_BATCH_LENGTH];

    unsigned int inline_buffer_length;

    unsigned int draw_arrays_length;
    unsigned int draw_arrays_max_count;
    /* FIXME: Unknown size, possibly endless, 1000 will do for now */
    GLint gl_draw_arrays_start[1000];
    GLsizei gl_draw_arrays_count[1000];

    GLuint gl_element_buffer;
    GLuint gl_memory_buffer;

    GLuint gl_vertex_array;

    uint32_t regs[0x2000];
} PGRAPHState;


typedef struct CacheEntry {
    QSIMPLEQ_ENTRY(CacheEntry) entry;
    unsigned int method : 14;
    unsigned int subchannel : 3;
    bool nonincreasing;
    uint32_t parameter;
} CacheEntry;

typedef struct Cache1State {
    unsigned int channel_id;
    enum FifoMode mode;

    /* Pusher state */
    bool push_enabled;
    bool dma_push_enabled;
    bool dma_push_suspended;
    hwaddr dma_instance;

    bool method_nonincreasing;
    unsigned int method : 14;
    unsigned int subchannel : 3;
    unsigned int method_count : 24;
    uint32_t dcount;
    bool subroutine_active;
    hwaddr subroutine_return;
    hwaddr get_jmp_shadow;
    uint32_t rsvd_shadow;
    uint32_t data_shadow;
    uint32_t error;

    bool pull_enabled;
    enum FIFOEngine bound_engines[NV2A_NUM_SUBCHANNELS];
    enum FIFOEngine last_engine;

    /* The actual command queue */
    SDL_mutex * cache_lock;
    SDL_cond * cache_cond;
    QSIMPLEQ_HEAD(, CacheEntry) cache;
    QSIMPLEQ_HEAD(, CacheEntry) working_cache;
} Cache1State;

typedef struct ChannelControl {
    hwaddr dma_put;
    hwaddr dma_get;
    uint32_t ref;
} ChannelControl;



typedef struct NV2AState {
    // PCIDevice dev;
    qemu_irq irq;

    bool exiting;

    // VGACommonState vga;
    // GraphicHwOps hw_ops;

    // QEMUTimer *vblank_timer;

    MemoryRegion *vram;
    MemoryRegion vram_pci;
    uint8_t *vram_ptr;
    MemoryRegion ramin;
    uint8_t *ramin_ptr;

    MemoryRegion mmio;

    MemoryRegion block_mmio[NV_NUM_BLOCKS];

    struct {
        uint32_t pending_interrupts;
        uint32_t enabled_interrupts;
    } pmc;

    struct {
        uint32_t pending_interrupts;
        uint32_t enabled_interrupts;

        // QemuThread puller_thread;
        Cache1State cache1;

        uint32_t regs[0x2000];
    } pfifo;

    struct {
        uint32_t regs[0x1000];
    } pvideo;

    struct {
        uint32_t pending_interrupts;
        uint32_t enabled_interrupts;

        uint32_t numerator;
        uint32_t denominator;

        uint32_t alarm_time;
    } ptimer;

    struct {
        uint32_t regs[0x1000];
    } pfb;

    struct PGRAPHState pgraph;

    struct {
        uint32_t pending_interrupts;
        uint32_t enabled_interrupts;

        hwaddr start;
    } pcrtc;

    struct {
        uint32_t core_clock_coeff;
        uint64_t core_clock_freq;
        uint32_t memory_clock_coeff;
        uint32_t video_clock_coeff;
    } pramdac;

    struct {
        ChannelControl channel_control[NV2A_NUM_CHANNELS];
    } user;

} NV2AState;



void update_irq(NV2AState *d)
{
    /* PFIFO */
    if (d->pfifo.pending_interrupts & d->pfifo.enabled_interrupts) {
        d->pmc.pending_interrupts |= NV_PMC_INTR_0_PFIFO;
    } else {
        d->pmc.pending_interrupts &= ~NV_PMC_INTR_0_PFIFO;
    }

    /* PCRTC */
    if (d->pcrtc.pending_interrupts & d->pcrtc.enabled_interrupts) {
        d->pmc.pending_interrupts |= NV_PMC_INTR_0_PCRTC;
    } else {
        d->pmc.pending_interrupts &= ~NV_PMC_INTR_0_PCRTC;
    }

    /* PGRAPH */
    if (d->pgraph.pending_interrupts & d->pgraph.enabled_interrupts) {
        d->pmc.pending_interrupts |= NV_PMC_INTR_0_PGRAPH;
    } else {
        d->pmc.pending_interrupts &= ~NV_PMC_INTR_0_PGRAPH;
    }

    if (d->pmc.pending_interrupts && d->pmc.enabled_interrupts) {
        NV2A_DPRINTF("raise irq\n");
        // pci_irq_assert(&d->dev);
    } else {
        // pci_irq_deassert(&d->dev);
    }
}

static DMAObject nv_dma_load(NV2AState *d, hwaddr dma_obj_address)
{
    // assert(dma_obj_address < memory_region_size(&d->ramin));

    uint32_t *dma_obj = (uint32_t*)(d->ramin_ptr + dma_obj_address);
    uint32_t flags = ldl_le_p(dma_obj);
    uint32_t limit = ldl_le_p(dma_obj + 1);
    uint32_t frame = ldl_le_p(dma_obj + 2);

    return (DMAObject){
        .dma_class = GET_MASK(flags, NV_DMA_CLASS),
        .dma_target = GET_MASK(flags, NV_DMA_TARGET),
        .address = (frame & NV_DMA_ADDRESS) | GET_MASK(flags, NV_DMA_ADJUST),
        .limit = limit,
    };
}

static void *nv_dma_map(NV2AState *d, hwaddr dma_obj_address, hwaddr *len)
{
    // assert(dma_obj_address < memory_region_size(&d->ramin));

    DMAObject dma = nv_dma_load(d, dma_obj_address);

    /* TODO: Handle targets and classes properly */
    NV2A_DPRINTF("dma_map %x, %x, %" HWADDR_PRIx " %" HWADDR_PRIx "\n",
                 dma.dma_class, dma.dma_target, dma.address, dma.limit);

    dma.address &= 0x07FFFFFF;

    // assert(dma.address + dma.limit < memory_region_size(d->vram));
    *len = dma.limit;
    return d->vram_ptr + dma.address;
}

// #include "nv2a_pbus.cpp"
// #include "nv2a_pcrtc.cpp"
// #include "nv2a_pfb.cpp"
#include "nv2a_pfifo.cpp"
// #include "nv2a_pgraph.cpp"
// #include "nv2a_pmc.cpp"
// #include "nv2a_pramdac.cpp"
// #include "nv2a_prmcio.cpp"
// #include "nv2a_prmvio.cpp"
// #include "nv2a_ptimer.cpp"
// #include "nv2a_pvideo.cpp"
// #include "nv2a_stubs.cpp"
// #include "nv2a_user.cpp"

typedef uint64_t (*read_func)(void *opaque, hwaddr addr, unsigned int size);
typedef void (*write_func)(void *opaque, hwaddr addr, uint64_t val, unsigned int size);
typedef struct {
    read_func read;
    write_func write;
} MemoryRegionOps;

typedef struct NV2ABlockInfo {
    const char* name;
    hwaddr offset;
    uint64_t size;
    MemoryRegionOps ops;
} NV2ABlockInfo;

static const struct NV2ABlockInfo blocktable[] = {

#define ENTRY(NAME, OFFSET, SIZE, RDFUNC, WRFUNC) \
    [ NV_ ## NAME ]  = { \
        .name = #NAME, .offset = OFFSET, .size = SIZE, \
        .ops = { .read = RDFUNC, .write = WRFUNC }, \
    }, \

    ENTRY(PMC,      0x000000, 0x001000, NULL, NULL) // pmc_read,      pmc_write)
    ENTRY(PBUS,     0x001000, 0x001000, NULL, NULL) // pbus_read,     pbus_write)
    ENTRY(PFIFO,    0x002000, 0x002000, NULL, NULL) // pfifo_read,    pfifo_write)
    ENTRY(PRMA,     0x007000, 0x001000, NULL, NULL) // prma_read,     prma_write)
    ENTRY(PVIDEO,   0x008000, 0x001000, NULL, NULL) // pvideo_read,   pvideo_write)
    ENTRY(PTIMER,   0x009000, 0x001000, NULL, NULL) // ptimer_read,   ptimer_write)
    ENTRY(PCOUNTER, 0x00a000, 0x001000, NULL, NULL) // pcounter_read, pcounter_write)
    ENTRY(PVPE,     0x00b000, 0x001000, NULL, NULL) // pvpe_read,     pvpe_write)
    ENTRY(PTV,      0x00d000, 0x001000, NULL, NULL) // ptv_read,      ptv_write)
    ENTRY(PRMFB,    0x0a0000, 0x020000, NULL, NULL) // prmfb_read,    prmfb_write)
    ENTRY(PRMVIO,   0x0c0000, 0x001000, NULL, NULL) // prmvio_read,   prmvio_write)
    ENTRY(PFB,      0x100000, 0x001000, NULL, NULL) // pfb_read,      pfb_write)
    ENTRY(PSTRAPS,  0x101000, 0x001000, NULL, NULL) // pstraps_read,  pstraps_write)
    ENTRY(PGRAPH,   0x400000, 0x002000, NULL, NULL) // pgraph_read,   pgraph_write)
    ENTRY(PCRTC,    0x600000, 0x001000, NULL, NULL) // pcrtc_read,    pcrtc_write)
    ENTRY(PRMCIO,   0x601000, 0x001000, NULL, NULL) // prmcio_read,   prmcio_write)
    ENTRY(PRAMDAC,  0x680000, 0x001000, NULL, NULL) // pramdac_read,  pramdac_write)
    ENTRY(PRMDIO,   0x681000, 0x001000, NULL, NULL) // prmdio_read,   prmdio_write)
    //ENTRY(PRAMIN, 0x700000, 0x100000, NULL, NULL) // pramin_read,   pramin_write)
    ENTRY(USER,     0x800000, 0x800000, NULL, NULL) // user_read,     user_write)
#undef ENTRY
};

static const char* nv2a_reg_names[] = {};
// static const char* nv2a_method_names[] = {};

void reg_log_read(int block, hwaddr addr, uint64_t val) {
    if (blocktable[block].name) {
        hwaddr naddr = blocktable[block].offset + addr;
        if (naddr < ARRAY_SIZE(nv2a_reg_names) && nv2a_reg_names[naddr]) {
            NV2A_DPRINTF("%s: read [%s] -> 0x%" PRIx64 "\n",
                    blocktable[block].name, nv2a_reg_names[naddr], val);
        } else {
            NV2A_DPRINTF("%s: read [%" HWADDR_PRIx "] -> 0x%" PRIx64 "\n",
                    blocktable[block].name, addr, val);
        }
    } else {
        NV2A_DPRINTF("(%d?): read [%" HWADDR_PRIx "] -> 0x%" PRIx64 "\n",
                block, addr, val);
    }
}

void reg_log_write(int block, hwaddr addr, uint64_t val) {
    if (blocktable[block].name) {
        hwaddr naddr = blocktable[block].offset + addr;
        if (naddr < ARRAY_SIZE(nv2a_reg_names) && nv2a_reg_names[naddr]) {
            NV2A_DPRINTF("%s: [%s] = 0x%" PRIx64 "\n",
                    blocktable[block].name, nv2a_reg_names[naddr], val);
        } else {
            NV2A_DPRINTF("%s: [%" HWADDR_PRIx "] = 0x%" PRIx64 "\n",
                    blocktable[block].name, addr, val);
        }
    } else {
        NV2A_DPRINTF("(%d?): [%" HWADDR_PRIx "] = 0x%" PRIx64 "\n",
                block, addr, val);
    }
}

#if 0
static void nv2a_overlay_draw_line(VGACommonState *vga, uint8_t *line, int y)
{
    NV2A_DPRINTF("nv2a_overlay_draw_line\n");

    NV2AState *d = container_of(vga, NV2AState, vga);
    DisplaySurface *surface = qemu_console_surface(d->vga.con);

    int surf_bpp = surface_bytes_per_pixel(surface);
    int surf_width = surface_width(surface);

    if (!(d->pvideo.regs[NV_PVIDEO_BUFFER] & NV_PVIDEO_BUFFER_0_USE)) return;

    hwaddr base = d->pvideo.regs[NV_PVIDEO_BASE];
    hwaddr limit = d->pvideo.regs[NV_PVIDEO_LIMIT];
    hwaddr offset = d->pvideo.regs[NV_PVIDEO_OFFSET];

    int in_width = GET_MASK(d->pvideo.regs[NV_PVIDEO_SIZE_IN],
                            NV_PVIDEO_SIZE_IN_WIDTH);
    int in_height = GET_MASK(d->pvideo.regs[NV_PVIDEO_SIZE_IN],
                             NV_PVIDEO_SIZE_IN_HEIGHT);
    int in_s = GET_MASK(d->pvideo.regs[NV_PVIDEO_POINT_IN],
                        NV_PVIDEO_POINT_IN_S);
    int in_t = GET_MASK(d->pvideo.regs[NV_PVIDEO_POINT_IN],
                        NV_PVIDEO_POINT_IN_T);
    int in_pitch = GET_MASK(d->pvideo.regs[NV_PVIDEO_FORMAT],
                            NV_PVIDEO_FORMAT_PITCH);
    int in_color = GET_MASK(d->pvideo.regs[NV_PVIDEO_FORMAT],
                            NV_PVIDEO_FORMAT_COLOR);

    // TODO: support other color formats
    assert(in_color == NV_PVIDEO_FORMAT_COLOR_LE_CR8YB8CB8YA8);

    int out_width = GET_MASK(d->pvideo.regs[NV_PVIDEO_SIZE_OUT],
                             NV_PVIDEO_SIZE_OUT_WIDTH);
    int out_height = GET_MASK(d->pvideo.regs[NV_PVIDEO_SIZE_OUT],
                             NV_PVIDEO_SIZE_OUT_HEIGHT);
    int out_x = GET_MASK(d->pvideo.regs[NV_PVIDEO_POINT_OUT],
                         NV_PVIDEO_POINT_OUT_X);
    int out_y = GET_MASK(d->pvideo.regs[NV_PVIDEO_POINT_OUT],
                         NV_PVIDEO_POINT_OUT_Y);


    if (y < out_y || y >= out_y + out_height) return;

    // TODO: scaling, color keys

    int in_y = y - out_y;
    if (in_y >= in_height) return;

    assert(offset + in_pitch * (in_y + 1) <= limit);
    uint8_t *in_line = d->vram_ptr + base + offset + in_pitch * in_y;

    int x;
    for (x=0; x<out_width; x++) {
        int ox = out_x + x;
        if (ox >= surf_width) break;
        int ix = in_s + x;
        if (ix >= in_width) break;

        uint8_t r,g,b;
        convert_yuy2_to_rgb(in_line, ix, &r, &g, &b);

        unsigned int pixel = vga->rgb_to_pixel(r, g, b);
        switch (surf_bpp) {
        case 1:
            ((uint8_t*)line)[ox] = pixel;
            break;
        case 2:
            ((uint16_t*)line)[ox] = pixel;
            break;
        case 4:
            ((uint32_t*)line)[ox] = pixel;
            break;
        default:
            assert(false);
            break;
        }
    }
}

static int nv2a_get_bpp(VGACommonState *s)
{
    if ((s->cr[0x28] & 3) == 3) {
        return 32;
    }
    return (s->cr[0x28] & 3) * 8;
}

static void nv2a_get_offsets(VGACommonState *s,
                             uint32_t *pline_offset,
                             uint32_t *pstart_addr,
                             uint32_t *pline_compare)
{
    NV2AState *d = container_of(s, NV2AState, vga);
    uint32_t start_addr, line_offset, line_compare;

    line_offset = s->cr[0x13]
        | ((s->cr[0x19] & 0xe0) << 3)
        | ((s->cr[0x25] & 0x20) << 6);
    line_offset <<= 3;
    *pline_offset = line_offset;

    start_addr = d->pcrtc.start / 4;
    *pstart_addr = start_addr;

    line_compare = s->cr[VGA_CRTC_LINE_COMPARE] |
        ((s->cr[VGA_CRTC_OVERFLOW] & 0x10) << 4) |
        ((s->cr[VGA_CRTC_MAX_SCAN] & 0x40) << 3);
    *pline_compare = line_compare;
}

static void nv2a_vga_gfx_update(void *opaque)
{
    VGACommonState *vga = opaque;
    vga->hw_ops->gfx_update(vga);

    NV2AState *d = container_of(vga, NV2AState, vga);
    d->pcrtc.pending_interrupts |= NV_PCRTC_INTR_0_VBLANK;
    update_irq(d);
}

static void nv2a_init_memory(NV2AState *d, MemoryRegion *ram)
{
    /* xbox is UMA - vram *is* ram */
    d->vram = ram;

     /* PCI exposed vram */
    memory_region_init_alias(&d->vram_pci, OBJECT(d), "nv2a-vram-pci", d->vram,
                             0, memory_region_size(d->vram));
    pci_register_bar(&d->dev, 1, PCI_BASE_ADDRESS_MEM_PREFETCH, &d->vram_pci);


    /* RAMIN - should be in vram somewhere, but not quite sure where atm */
    memory_region_init_ram(&d->ramin, OBJECT(d), "nv2a-ramin", 0x100000);
    /* memory_region_init_alias(&d->ramin, "nv2a-ramin", &d->vram,
                         memory_region_size(&d->vram) - 0x100000,
                         0x100000); */

    memory_region_add_subregion(&d->mmio, 0x700000, &d->ramin);


    d->vram_ptr = memory_region_get_ram_ptr(d->vram);
    d->ramin_ptr = memory_region_get_ram_ptr(&d->ramin);

    memory_region_set_log(d->vram, true, DIRTY_MEMORY_NV2A);
    memory_region_set_dirty(d->vram, 0, memory_region_size(d->vram));

    /* hacky. swap out vga's vram */
    memory_region_destroy(&d->vga.vram);
    memory_region_init_alias(&d->vga.vram, OBJECT(d), "vga.vram",
                             d->vram, 0, memory_region_size(d->vram));
    d->vga.vram_ptr = memory_region_get_ram_ptr(&d->vga.vram);
    vga_dirty_log_start(&d->vga);


    pgraph_init(d);

    /* fire up puller */
    qemu_thread_create(&d->pfifo.puller_thread,
                       pfifo_puller_thread,
                       d, QEMU_THREAD_JOINABLE);
}

static int nv2a_initfn(PCIDevice *dev)
{
    int i;
    NV2AState *d;

    d = NV2A_DEVICE(dev);

    dev->config[PCI_INTERRUPT_PIN] = 0x01;

    d->pcrtc.start = 0;

    d->pramdac.core_clock_coeff = 0x00011c01; /* 189MHz...? */
    d->pramdac.core_clock_freq = 189000000;
    d->pramdac.memory_clock_coeff = 0;
    d->pramdac.video_clock_coeff = 0x0003C20D; /* 25182Khz...? */



    /* legacy VGA shit */
    VGACommonState *vga = &d->vga;
    vga->vram_size_mb = 4;
    /* seems to start in color mode */
    vga->msr = VGA_MIS_COLOR;

    vga_common_init(vga, OBJECT(dev));
    vga->get_bpp = nv2a_get_bpp;
    vga->get_offsets = nv2a_get_offsets;
    vga->overlay_draw_line = nv2a_overlay_draw_line;

    d->hw_ops = *vga->hw_ops;
    d->hw_ops.gfx_update = nv2a_vga_gfx_update;
    vga->con = graphic_console_init(DEVICE(dev), &d->hw_ops, vga);


    /* mmio */
    memory_region_init(&d->mmio, OBJECT(dev), "nv2a-mmio", 0x1000000);
    pci_register_bar(&d->dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &d->mmio);

    for (i=0; i<ARRAY_SIZE(blocktable); i++) {
        if (!blocktable[i].name) continue;
        memory_region_init_io(&d->block_mmio[i], OBJECT(dev),
                              &blocktable[i].ops, d,
                              blocktable[i].name, blocktable[i].size);
        memory_region_add_subregion(&d->mmio, blocktable[i].offset,
                                    &d->block_mmio[i]);
    }

    /* init fifo cache1 */
    qemu_mutex_init(&d->pfifo.cache1.cache_lock);
    qemu_cond_init(&d->pfifo.cache1.cache_cond);
    QSIMPLEQ_INIT(&d->pfifo.cache1.cache);
    QSIMPLEQ_INIT(&d->pfifo.cache1.working_cache);

    return 0;
}

static void nv2a_exitfn(PCIDevice *dev)
{
    NV2AState *d;
    d = NV2A_DEVICE(dev);

    d->exiting = true;
    qemu_cond_signal(&d->pfifo.cache1.cache_cond);
    qemu_thread_join(&d->pfifo.puller_thread);

    qemu_mutex_destroy(&d->pfifo.cache1.cache_lock);
    qemu_cond_destroy(&d->pfifo.cache1.cache_cond);

    pgraph_destroy(&d->pgraph);
}

static void nv2a_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->vendor_id = PCI_VENDOR_ID_NVIDIA;
    k->device_id = PCI_DEVICE_ID_NVIDIA_GEFORCE_NV2A;
    k->revision = 161;
    k->class_id = PCI_CLASS_DISPLAY_3D;
    k->init = nv2a_initfn;
    k->exit = nv2a_exitfn;

    dc->desc = "GeForce NV2A Integrated Graphics";
}

static const TypeInfo nv2a_info = {
    .name          = "nv2a",
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(NV2AState),
    .class_init    = nv2a_class_init,
};

static void nv2a_register(void)
{
    type_register_static(&nv2a_info);
}
type_init(nv2a_register);

void nv2a_init(PCIBus *bus, int devfn, MemoryRegion *ram)
{
    PCIDevice *dev = pci_create_simple(bus, devfn, "nv2a");
    NV2AState *d = NV2A_DEVICE(dev);
    nv2a_init_memory(d, ram);
}
#endif
