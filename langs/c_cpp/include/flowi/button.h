
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

struct FlButton;

// Show a regular push button
static bool fl_button_regular(const char *label);

// Show a regular push button with a specific size
static bool fl_button_regular_size(const char *label, FlVec2 size);

// Show a regular push button without any frame padding.
static bool fl_button_small(const char *label);

// Invisible button that allows custom using drawing, but still acts like a
// button.
static bool fl_button_invisible(const char *label, FlVec2 size,
                                FlButtonFlags flags);

// Button with a check box state
static bool fl_button_check_box(const char *label, bool *state);

// Radio button
static bool fl_button_radio(const char *label, bool state);

// TODO: Document
static void fl_button_bullet();

// TODO: Document
static bool fl_button_image_with_text(FlImage image, const char *label);

#include "button.inl"

#ifdef __cplusplus
}
#endif
