///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file is auto-generated by api_gen. DO NOT EDIT!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "image.h"
#include "manual.h"
#include "math_data.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum FlButtonFlags {
  // Default flags
  FlButtonFlags_None = 0,
  // React on left mouse button (default)
  FlButtonFlags_MouseButtonLeft = 1 << 0,
  // React on right mouse button
  FlButtonFlags_MouseButtonRight = 1 << 1,
  // React on center mouse button
  FlButtonFlags_MouseButtonMiddled = 1 << 2,
} FlButtonFlags;

typedef struct FlButton {
} FlButton;

#ifdef __cplusplus
}
#endif
