
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

struct FlWindow;

// Sets the position of the next window, call before begin()
static void fl_window_set_pos(FlVec2 pos);

// Always call a matching end() for each begin() call, regardless of its return
// value!
static bool fl_window_begin(const char *name, FlWindowFlags flags);

// End call for various types such as windows, lists, etc.
static void fl_window_end();

// Call between begin() and end() to create a child window. Child windows can
// embed their own child.
static bool fl_window_begin_child(const char *id, FlVec2 size, bool border,
                                  FlWindowFlags flags);

// End call for various types such as windows, lists, etc.
static void fl_window_end_child();

// Returns true if the window is appearing after being hidden/inactive (or the
// first time)
static bool fl_window_is_appearing();

// Is current window collpased?
static bool fl_window_is_collapsed();

// is current window focused? or its root/child, depending on flags. see flags
// for options.
static bool fl_window_is_focused(FlFocusedFlags flags);

// is current window hovered (and typically: not blocked by a popup/modal)? see
// flags for options. nb: if you are trying to check whether your mouse should
// be dispatched to imgui or to your app, you should use the
// 'io.wantcapturemouse' boolean for that! please read the faq!
static bool fl_window_is_hovered(FlHoveredFlags flags);

// get dpi scale currently associated to the current window's viewport.
static float fl_window_dpi_scale();

// get current window position in screen space (useful if you want to do your
// own drawing via the drawlist api)
static FlVec2 fl_window_pos();

// get current window size
static FlVec2 fl_window_size();

#include "window.inl"

#ifdef __cplusplus
}
#endif
