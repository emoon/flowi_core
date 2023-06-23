
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file is auto-generated by api_gen. DO NOT EDIT!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "manual.h"
#include "math_data.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

struct FlStyle;

// Permantly set a color
static void fl_style_set_color(FlStyleColor color, FlColor value);

// Permantly set a color (ARGB)
static void fl_style_set_color_u32(FlStyleColor color, uint32_t value);

// Temporary push a color change (ARGB)
static void fl_style_push_color_u32(FlStyleColor color, uint32_t value);

// Temporary push a color change
static void fl_style_push_color(FlStyleColor color, FlColor value);

// Temporary push a color change
static void fl_style_pop_color();

// Pushes a single style change
static void fl_style_push_single(FlStyleSingle style, float value);

// Pushes a Vec2 style change
static void fl_style_push_vec2(FlStyleVec2 style, FlVec2 value);

// Pops a style change
static void fl_style_pop();

#include "style.inl"

#ifdef __cplusplus
}
#endif
