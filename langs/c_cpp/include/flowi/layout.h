///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file is auto-generated by api_gen. DO NOT EDIT!
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "manual.h"
#include "math_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FlCursor {
} FlCursor;

// Separator, generally horizontal. Inside a menu bar or in horizontal layout mode, this becomes a vertical separator.
static void fl_cursor_separator();

// Call between widgets or groups to layout them horizontally. X position given in window coordinates.
static void fl_cursor_same_line(float offset_from_start_x, float spacing);

// Undo a same_line() or force a new line when in a horizontal-layout context.
static void fl_cursor_new_line();

// Undo a same_line() or force a new line when in a horizontal-layout context.
static void fl_cursor_spacing();

// Add a dummy item of given size. Unlike widgets.invisible_button(), dummmy() won't take the mouse click or be
// navigable into.
static void fl_cursor_dummy(FlVec2 size);

// Move content position toward the right, by indent_w, or style.IndentSpacing if indent_w <= 0
static void fl_cursor_indent(float indent);

// Move content position back to the left, by indent_w, or style.IndentSpacing if indent_w <= 0
static void fl_cursor_unindent(float indent_w);

static void fl_cursor_begin_group();

static void fl_cursor_end_group();

// Cursor position in window coordinates (relative to window position)
static FlVec2 fl_cursor_get_pos();

static float fl_cursor_get_pos_x();

static float fl_cursor_get_pos_y();

// Set position in window coordinates (relative to window position)
static void fl_cursor_set_pos(FlVec2 pos);

static void fl_cursor_set_pos_x(float x);

static void fl_cursor_set_pos_y(float y);

// cursor position in absolute coordinates (useful to work with ImDrawList API).
// generally top-left == GetMainViewport()->Pos == (0,0) in single viewport mode,
// and bottom-right == GetMainViewport()->Pos+Size == io.DisplaySize in single-viewport mode.
static FlVec2 fl_cursor_screen_pos();

static void fl_cursor_set_screen_pos(FlVec2 pos);

// vertically align upcoming text baseline to FramePadding.y so that it will align properly to regularly framed items
// (call if you have text on a line before a framed item)
static void fl_cursor_align_text_to_frame_padding();

// ~ FontSize
static float fl_cursor_get_text_line_height();

// ~ FontSize + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of text)
static float fl_cursor_get_text_line_height_with_spacing();

// ~ FontSize + style.FramePadding.y * 2
static float fl_cursor_get_frame_height();

// ~ FontSize + style.FramePadding.y * 2 + style.ItemSpacing.y (distance in pixels between 2 consecutive lines of framed
// widgets)
static float fl_cursor_get_frame_height_with_spacing();

#include "layout.inl"
#ifdef __cplusplus
}
#endif
