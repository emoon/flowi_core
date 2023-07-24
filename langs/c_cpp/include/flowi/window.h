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

typedef enum FlWindowFlags {
  // Default flags
  FlWindowFlags_None = 0,
  // Disable title-bar
  FlWindowFlags_NoTitleBar = 1 << 0,
  // Disable user resizing with the lower-right grip
  FlWindowFlags_NoResize = 1 << 1,
  // Disable user moving the window
  FlWindowFlags_NoMove = 1 << 2,
  // Disable scrollbars (window can still scroll with mouse or programmatically)
  FlWindowFlags_NoScrollbar = 1 << 3,
  // Disable user vertically scrolling with mouse wheel. On child window, mouse
  // wheel will be forwarded to the parent unless NoScrollbar is also set.
  FlWindowFlags_NoScrollWithMouse = 1 << 4,
  // Disable user collapsing window by double-clicking on it. Also referred to
  // as Window Menu Button (e.g. within a docking node).
  FlWindowFlags_NoCollapse = 1 << 5,
  // Resize every window to its content every frame
  FlWindowFlags_AlwaysAutoResize = 1 << 6,
  // Disable drawing background color (WindowBg, etc.) and outside border.
  // Similar as using SetNextWindowBgAlpha(0.0f).
  FlWindowFlags_NoBackground = 1 << 7,
  // Never load/save settings in .ini file
  FlWindowFlags_NoSavedSettings = 1 << 8,
  // Disable catching mouse, hovering test with pass through.
  FlWindowFlags_NoMouseInputs = 1 << 9,
  // Has a menu-bar
  FlWindowFlags_MenuBar = 1 << 10,
  // Allow horizontal scrollbar to appear (off by default).
  FlWindowFlags_HorizontalScrollbar = 1 << 11,
  // Disable taking focus when transitioning from hidden to visible state
  FlWindowFlags_NoFocusOnAppearing = 1 << 12,
  // Disable bringing window to front when taking focus (e.g. clicking on it or
  // programmatically giving it focus)
  FlWindowFlags_NoBringToFrontOnFocus = 1 << 13,
  // Always show vertical scrollbar (even if content_size.y < size.y)
  FlWindowFlags_AlwaysVerticalScrollbar = 1 << 14,
  // Always show horizontal scrollbar (even if content_size.x < size.x)
  FlWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
  // Ensure child windows without border uses style.WindowPadding (ignored by
  // default for non-bordered child windows,
  FlWindowFlags_AlwaysUseWindowPadding = 1 << 16,
  // No gamepad/keyboard navigation within the window
  FlWindowFlags_NoNavInputs = 1 << 17,
  // No focusing toward this window with gamepad/keyboard navigation (e.g.
  // skipped by CTRL+TAB)
  FlWindowFlags_NoNavFocus = 1 << 18,
  // Display a dot next to the title. When used in a tab/docking context, tab is
  // selected when clicking the X +// closure is not assumed (will wait for user
  // to stop submitting the tab). Otherwise closure is assumed when// pressing
  // the X, so if you keep submitting the tab may reappear at end of tab bar.
  FlWindowFlags_UnsavedDocument = 1 << 19,

  FlWindowFlags_NoNav = FlNoNav_NoNavInputs | FlWindowFlags_NoNavFocus,

  FlWindowFlags_NoDecoration =
      FlNoDecoration_NoTitleBar | FlWindowFlags_NoResize |
      FlWindowFlags_NoScrollbar | FlWindowFlags_NoCollapse,

  FlWindowFlags_NoInputs = FlNoInputs_NoMouseInputs |
                           FlWindowFlags_NoNavInputs | FlWindowFlags_NoNavFocus,
} FlWindowFlags;

typedef enum FlFocusedFlags {

  FlFocusedFlags_None = 0,
  // Return true if any children of the window is focused
  FlFocusedFlags_ChildWindows = 1 << 0,
  // Test from root window (top most parent of the current hierarchy)
  FlFocusedFlags_RootWindow = 1 << 1,
  // Return true if any window is focused. Important: If you are trying to tell
  // how to dispatch your low-level inputs, do NOT use this. Use
  // 'io.WantCaptureMouse' instead! Please read the FAQ!
  FlFocusedFlags_AnyWindow = 1 << 2,
  // Do not consider popup hierarchy (do not treat popup emitter as parent of
  // popup) (when used with _ChildWindows or _RootWindow)
  FlFocusedFlags_NoPopupHierarchy = 1 << 3,
  // Consider docking hierarchy (treat dockspace host as parent of docked
  // window) (when used with _ChildWindows or _RootWindow)
  FlFocusedFlags_DockHierarchy = 1 << 4,

  FlFocusedFlags_RootAndChildWindows =
      FlRootAndChildWindows_RootWindow | FlFocusedFlags_ChildWindows,
} FlFocusedFlags;

typedef enum FlHoveredFlags {
  // Return true if directly over the item/window, not obstructed by another
  // window, not obstructed by an active popup or modal blocking inputs under
  // them.
  FlHoveredFlags_None = 0,
  // is_window_hovered() only: Return true if any children of the window is
  // hovered
  FlHoveredFlags_ChildWindows = 1 << 0,
  // is_window_hovered() only: Test from root window (top most parent of the
  // current hierarchy)
  FlHoveredFlags_RootWindow = 1 << 1,
  // is_window_hovered() only: Return true if any window is hovered
  FlHoveredFlags_AnyWindow = 1 << 2,
  // is_window_hovered() only: Do not consider popup hierarchy (do not treat
  // popup emitter as parent of popup) (when used with _ChildWindows or
  // _RootWindow)
  FlHoveredFlags_NoPopupHierarchy = 1 << 3,
  // is_window_hovered() only: Consider docking hierarchy (treat dockspace host
  // as parent of docked window) (when used with _ChildWindows or _RootWindow)
  FlHoveredFlags_DockHierarchy = 1 << 4,
  // Return true even if a popup window is normally blocking access to this
  // item/window
  FlHoveredFlags_AllowWhenBlockedByPopup = 1 << 5,
  // Return true even if an active item is blocking access to this item/window.
  // Useful for Drag and Drop patterns.
  FlHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 7,
  // is_item_hovered() only: Return true even if the position is obstructed or
  // overlapped by another window
  FlHoveredFlags_AllowWhenOverlapped = 1 << 8,
  // is_item_hovered() only: Return true even if the item is disabled
  FlHoveredFlags_AllowWhenDisabled = 1 << 9,
  // Disable using gamepad/keyboard navigation state when active, always query
  // mouse.
  FlHoveredFlags_NoNavOverride = 1 << 10,

  FlHoveredFlags_RectOnly = FlRectOnly_AllowWhenBlockedByPopup |
                            FlHoveredFlags_AllowWhenBlockedByActiveItem |
                            FlHoveredFlags_AllowWhenOverlapped,

  FlHoveredFlags_RootAndChildWindows =
      FlRootAndChildWindows_RootWindow | FlHoveredFlags_ChildWindows,
  // Hovering delays (for tooltips)// Return true after io.HoverDelayNormal
  // elapsed (~0.30 sec)
  FlHoveredFlags_DelayNormal = 1 << 11,
  // Return true after io.HoverDelayShort elapsed (~0.10 sec)
  FlHoveredFlags_DelayShort = 1 << 12,
  // Disable shared delay system where moving from one item to the next keeps
  // the previous timer for a short time (standard for tooltips with long
  // delays)
  FlHoveredFlags_NoSharedDelay = 1 << 13,
} FlHoveredFlags;

typedef struct FlWindow {
} FlWindow;

#ifdef __cplusplus
}
#endif
