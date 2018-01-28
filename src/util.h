#pragma once

#include <stdint.h>
#include <SDL.h>

#define KiB(x) (x * 1024)
#define MiB(x) (x * KiB(1024))

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
