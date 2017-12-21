#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <dev.h>
#include <mem.h>
#include <sched.h>
#include "nv2a.h"

class Video {
protected:
    Scheduler     *m_sched;
    MemoryRegion  *m_mem;
    MemoryRegion  *m_ram;
    MemoryRegion  *m_mmio;
    MemoryRegion  *m_vram;
    NV2AState     *m_nv2a;

    SDL_Window    *m_window;
    SDL_GLContext  m_context;
    // GLuint         m_vao, m_vbo, m_ebo, m_tex;
    // GLuint         m_vert_shader;
    // GLuint         m_frag_shader;
    // GLuint         m_shader_prog;

    bool           m_render_thread_should_exit;
    SDL_Thread    *m_render_thread;

public:
    Video(MemoryRegion *mem, MemoryRegion *ram, Scheduler *sched);
    ~Video();
    int Initialize();
    int Update();
    int Cleanup();
    void RenderThread();
    static int EventHandler(MemoryRegion *region, struct MemoryRegionEvent *event, void *user_data);
    void FixmeLock();
    void FixmeUnlock();
    
    // int InitShaders();
    // int InitGeometry();
    // int InitTextures();
    // void *GetFramebuffer();
};

#endif
