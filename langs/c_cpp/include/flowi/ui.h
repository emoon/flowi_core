///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file is auto-generated by api_gen. DO NOT EDIT!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "image.h"
#include "layout.h"
#include "manual.h"
#include "math_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlUi {
} FlUi;

// Draw static text with the selected font
static void fl_ui_text(const char* text);

// Draw image. Images can be created with [Image::create_from_file] and [Image::create_from_memory]
static void fl_ui_image(FlImage image);

// Draw image with given size
static void fl_ui_image_with_size(FlImage image, FlVec2 size);

// Set position for the next ui-element (this is used when [LayoutMode::Manual] is used)
static void fl_ui_set_pos(FlVec2 pos);

// Get the last widget size. This is usually used for doing manual layouting
static FlRect fl_ui_get_last_widget_size(FlVec2 pos);

// Push button widget that returns true if user has pressed it
static bool fl_ui_push_button_with_icon(const char* text, FlImage image, FlVec2 text_pos, float image_scale);

// Push button widget that returns true if user has pressed it
static bool fl_ui_push_button(const char* text);

#include "ui.inl"
#ifdef __cplusplus
}
#endif
