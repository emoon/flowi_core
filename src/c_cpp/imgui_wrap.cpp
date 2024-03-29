#include <flowi/ui.h>
#include <flowi/style.h>
#include <flowi/window.h>
#include <flowi/application_settings.h>
#include <flowi/text.h>
#include <flowi/menu.h>
#include <flowi/button.h>
#include <flowi/item.h>
#include <flowi/painter.h>
#include <flowi/input.h>
#include "image_private.h"
#include "internal.h"
//#include "primitives.h"
#include "allocator.h"
#include "string_allocator.h"
#include "layer.h"
#include <dear-imgui/imgui.h>
#include <stdio.h>
//#include "imgui_impl_glfw.h"
#include "internal.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Malloc based allocator. We should use tslf or similar in a sandbox, but this is atleast in one place

static void* alloc_malloc(void* user_data, u64 size) {
    FL_UNUSED(user_data);
    return malloc(size);
}

static void* realloc_malloc(void* user_data, void* ptr, u64 size) {
    FL_UNUSED(user_data);
    return realloc(ptr, size);
}

static void free_malloc(void* user_data, void* ptr) {
    FL_UNUSED(user_data);
    free(ptr);
}

static void memory_error(void* user_data, const char* text, int text_len) {
    FL_UNUSED(user_data);
    FL_UNUSED(text);
    FL_UNUSED(text_len);
    printf("Ran out of memory! :(\n");
}

static FlAllocator malloc_allocator = {
    FlAllocatorError_Exit, NULL, memory_error, alloc_malloc, NULL, realloc_malloc, free_malloc,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// translate from FlColor to ImGuiCol_

static int s_color_lut[ImGuiCol_COUNT * 4];
static int s_single_style_lut[ImGuiStyleVar_COUNT];
static int s_vec2_style_lut[ImGuiStyleVar_COUNT];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct AppState {
    FlInternalData* state;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %d:%s\n", error, description);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FontAtlas {
    uint16_t width;
    uint16_t height;
    uint32_t data_size;
    void* data;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FontAtlas imgui_build_rgba32_texture() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->Build();

    uint8_t* data;
    int32_t width;
    int32_t height;
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

    FontAtlas atlas = { (uint16_t)width, (uint16_t)height, (uint32_t)(width * height * 4), data };
    return atlas;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FontAtlas imgui_build_r8_texture() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->Build();

    uint8_t* data;
    int32_t width;
    int32_t height;
    io.Fonts->GetTexDataAsAlpha8(&data, &width, &height);

    FontAtlas atlas = { (uint16_t)width, (uint16_t)height, (uint32_t)(width * height * 1), data };
    return atlas;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" bool c_should_close(FlInternalData* state) { 
    FL_UNUSED(state);
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void c_pre_update(FlInternalData* state) {
    FL_UNUSED(state);
    ImGui::NewFrame();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void c_post_update(FlInternalData* state) {
    ImGui::Render();
    LinearAllocator_rewind(&state->frame_allocator);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void c_pre_update_create(FlInternalData* state) {
    FL_UNUSED(state);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" ImDrawData imgui_get_draw_data() {
    return *ImGui::GetDrawData();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void c_destroy(FlInternalData* data) {
    FL_UNUSED(data);
    ImGui::DestroyContext();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_set_display_size_impl(FlInternalData* data, uint32_t width, uint32_t height) {
    FL_UNUSED(data);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)width, (float)height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_set_display_buffer_scale_impl(FlInternalData* data, float width, float height) {
    FL_UNUSED(data);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplayFramebufferScale = ImVec2(width, height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_set_delta_time_impl(FlInternalData* data, float delta_time) {
    FL_UNUSED(data);
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = delta_time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void image_show(FlInternalData* ctx, FlImage image) {
    FL_UNUSED(ctx);
    FL_UNUSED(image);
    /*
    ImagePrivate* image_data = (ImagePrivate*)Handles_get_data(&ctx->global->image_handles, image);

    if (!image_data) {
        ERROR_ADD(FlError_Image, "Invalid handle %s", "todo name");
        return;
    }

    // TODO: Better way to do this?
    PrimitiveImage* prim = (PrimitiveImage*)CommandBuffer_alloc_cmd(
            &ctx->layers[0].primitive_commands, 
            Primitive_DrawImage, 
            sizeof(PrimitiveImage));
    prim->image = image_data;
    prim->size.x = image_data->info.width;
    prim->size.y = image_data->info.height;

    ImGui::Image((ImTextureID)image, ImVec2(image_data->info.width, image_data->info.height),
                 ImVec2(image_data->u0, image_data->v0), ImVec2(image_data->u1, image_data->v1));
    */
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Permantly set a color

FL_PUBLIC_SYMBOL void fl_style_set_color_impl(FlInternalData* ctx, FlStyleColor color, FlColor value) {
    FL_UNUSED(ctx);
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    int color_index = s_color_lut[color];
    colors[color_index] = ImVec4(value.r, value.g, value.b, value.a);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Permantly set a color (RGBA)

FL_PUBLIC_SYMBOL void fl_style_set_color_u32_impl(FlInternalData* ctx, FlStyleColor color, uint32_t value) {
    FL_UNUSED(ctx);
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    int color_index = s_color_lut[color];
    colors[color_index] = ImGui::ColorConvertU32ToFloat4(value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary push a color change (RGBA)

FL_PUBLIC_SYMBOL void fl_style_push_color_u32_impl(FlInternalData* ctx, FlStyleColor color, uint32_t value) {
    FL_UNUSED(ctx);
    int color_index = s_color_lut[color];
    ImGui::PushStyleColor(color_index, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary push a color change

FL_PUBLIC_SYMBOL void fl_style_push_color_impl(FlInternalData* ctx, FlStyleColor color, FlColor value) {
    FL_UNUSED(ctx);
    int color_index = s_color_lut[color];
    ImGui::PushStyleColor(color_index, ImVec4(value.r, value.g, value.b, value.a));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Temporary push a color change

FL_PUBLIC_SYMBOL void fl_style_pop_color_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::PopStyleColor();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pushes a single style change

FL_PUBLIC_SYMBOL void fl_style_push_single_impl(FlInternalData* ctx, FlStyleSingle style, float value) {
    FL_UNUSED(ctx);
    int style_index = s_single_style_lut[style];
    ImGui::PushStyleVar(style_index, value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pushes a Vec2 style change

FL_PUBLIC_SYMBOL void fl_style_push_vec2_impl(FlInternalData* ctx, FlStyleVec2 style, FlVec2 value) {
    FL_UNUSED(ctx);
    int style_index = s_vec2_style_lut[style];
    ImGui::PushStyleVar(style_index, ImVec2(value.x, value.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pops single style change

FL_PUBLIC_SYMBOL void fl_style_pop_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::PopStyleVar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlStyleApi g_style_funcs = {
    NULL,
    fl_style_set_color_impl,
    fl_style_set_color_u32_impl,
    fl_style_push_color_u32_impl,
    fl_style_push_color_impl,
    fl_style_pop_color_impl,
    fl_style_push_single_impl,
    fl_style_push_vec2_impl,
    fl_style_pop_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlUiApi g_ui_funcs = {
    NULL,
    NULL, // TODO: Fix me
    image_show,
    NULL,
    NULL, 
    NULL,
    NULL,
    NULL,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlUiApi* fl_ui_get_api(AppState* app_state, int api_version) {
    FL_UNUSED(api_version);
    return &app_state->state->ui_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_window_set_pos_impl(FlInternalData* ctx, FlVec2 pos) {
    FL_UNUSED(ctx);
    ImGui::SetNextWindowPos(ImVec2(pos.x, pos.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_window_set_size_impl(FlInternalData* ctx, FlVec2 pos) {
    FL_UNUSED(ctx);
    ImGui::SetNextWindowSize(ImVec2(pos.x, pos.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_begin_impl(FlInternalData* ctx, FlString name, FlWindowFlags flags) {
    char temp_buffer[2048];

    const char* window_name =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), name);

    return ImGui::Begin(window_name, NULL, (ImGuiWindowFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_window_end_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::End();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_begin_child_impl(FlInternalData* ctx, FlString id, FlVec2 size, bool border, FlWindowFlags flags) {
    char temp_buffer[2048];

    const char* window_name =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), id);

    return ImGui::BeginChild(window_name, ImVec2(size.x, size.y), border, (ImGuiWindowFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_window_end_child_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::EndChild();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_is_appearing_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsWindowAppearing();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_is_collapsed_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsWindowCollapsed();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_is_focused_impl(FlInternalData* ctx, FlFocusedFlags flags) {
    FL_UNUSED(ctx);
    return ImGui::IsWindowFocused((ImGuiFocusedFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_window_is_hovered_impl(FlInternalData* ctx, FlHoveredFlags flags) {
    FL_UNUSED(ctx);
    return ImGui::IsWindowHovered((ImGuiHoveredFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_window_dpi_scale_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetWindowDpiScale();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_window_pos_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 pos = ImGui::GetWindowPos();
    return { pos.x, pos.y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_window_size_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 size = ImGui::GetWindowSize();
    return { size.x, size.y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlWindowApi g_window_funcs = {
    NULL,
    fl_window_set_pos_impl,
    //fl_window_set_size_impl,
    fl_window_begin_impl,
    fl_window_end_impl,
    fl_window_begin_child_impl,
    fl_window_end_child_impl,
    fl_window_is_appearing_impl,
    fl_window_is_collapsed_impl,
    fl_window_is_focused_impl,
    fl_window_is_hovered_impl,
    fl_window_dpi_scale_impl,
    fl_window_pos_impl,
    fl_window_size_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_separator_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::Separator();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_same_line_impl(FlInternalData* ctx, float offset_from_start_x, float spacing) {
    FL_UNUSED(ctx);
    ImGui::SameLine(offset_from_start_x, spacing);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_new_line_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::NewLine();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_spacing_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::Spacing();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_dummy_impl(FlInternalData* ctx, FlVec2 size) {
    FL_UNUSED(ctx);
    ImGui::Dummy(ImVec2(size.x, size.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_indent_impl(FlInternalData* ctx, float indent_w) {
    FL_UNUSED(ctx);
    ImGui::Indent(indent_w);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_unindent_impl(FlInternalData* ctx, float indent_w) {
    FL_UNUSED(ctx);
    ImGui::Unindent(indent_w);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_begin_group_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::BeginGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_end_group_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::EndGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_cursor_get_pos_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 pos = ImGui::GetCursorPos();
    return { pos.x, pos.y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_x_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetCursorPosX();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_y_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetCursorPosY();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_set_pos_impl(FlInternalData* ctx, FlVec2 local_pos) {
    FL_UNUSED(ctx);
    ImGui::SetCursorPos(ImVec2(local_pos.x, local_pos.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_set_x_impl(FlInternalData* ctx, float x) {
    FL_UNUSED(ctx);
    ImGui::SetCursorPosX(x);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_set_y_impl(FlInternalData* ctx, float y) {
    FL_UNUSED(ctx);
    ImGui::SetCursorPosY(y);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_cursor_screen_pos_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    return { pos.x, pos.y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_set_screen_pos_impl(FlInternalData* ctx, FlVec2 screen_pos) {
    FL_UNUSED(ctx);
    ImGui::SetCursorScreenPos(ImVec2(screen_pos.x, screen_pos.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_cursor_align_text_to_frame_padding_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::AlignTextToFramePadding();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_text_line_height_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetTextLineHeight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_text_line_height_with_spacing_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetTextLineHeightWithSpacing();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_frame_height_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetFrameHeight();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL float fl_cursor_get_frame_height_with_spacing_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::GetFrameHeightWithSpacing();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FlCursorApi g_cursor_funcs = {
    NULL,
    fl_cursor_separator_impl,
    fl_cursor_same_line_impl,
    fl_cursor_new_line_impl,
    fl_cursor_spacing_impl,
    fl_cursor_dummy_impl,
    fl_cursor_indent_impl,
    fl_cursor_unindent_impl,
    fl_cursor_begin_group_impl,
    fl_cursor_end_group_impl,
    fl_cursor_get_pos_impl,
    fl_cursor_get_x_impl,
    fl_cursor_get_y_impl,
    fl_cursor_set_pos_impl,
    fl_cursor_set_x_impl,
    fl_cursor_set_y_impl,
    fl_cursor_screen_pos_impl,
    fl_cursor_set_screen_pos_impl,
    fl_cursor_align_text_to_frame_padding_impl,
    fl_cursor_get_text_line_height_impl,
    fl_cursor_get_text_line_height_with_spacing_impl,
    fl_cursor_get_frame_height_impl,
    fl_cursor_get_frame_height_with_spacing_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_text_calc_size_impl(FlInternalData* ctx, FlString text) {
    FL_UNUSED(ctx);
    ImVec2 size = ImGui::CalcTextSize(text.str, text.str + text.len);
    return { size.x, size.y };
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_text_show_impl(FlInternalData* ctx, FlString text) {
    FL_UNUSED(ctx);
    printf("fl_text_show_impl\n");
    ImGui::TextUnformatted(text.str, text.str + text.len);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_text_bullet_impl(FlInternalData* ctx, FlString text) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), text);

    ImGui::BulletText("%s", temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_text_label_impl(FlInternalData* ctx, FlString label, FlString text) {
    char temp_buffer[2048];
    char temp_buffer_2[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    const char* temp_text2 =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer_2, sizeof(temp_buffer), text);

    ImGui::LabelText(temp_text, "%s", temp_text2);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_text_show_colored_impl(FlInternalData* ctx, FlColor color, FlString text) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), text);

    ImGui::TextColored(ImVec4(color.r, color.g, color.b, color.a), "%s", temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_text_show_disabled_impl(FlInternalData* ctx, FlString text) {
    FL_UNUSED(ctx);

    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), text);

    ImGui::TextDisabled("%s", temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
static void text_show_wrapped(FlInternalData* ctx, FlString text) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), text);

    ImGui::TextWrapped("%s", temp_text);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FlTextApi g_text_funcs = {
    NULL,
    fl_text_calc_size_impl,
    fl_text_bullet_impl,
    fl_text_label_impl,
    fl_text_show_colored_impl,
    fl_text_show_impl,
    fl_text_show_disabled_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_begin_bar_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::BeginMenuBar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_menu_end_bar_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::EndMenuBar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_begin_main_bar_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::BeginMainMenuBar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_menu_end_main_bar_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::EndMainMenuBar();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_begin_impl(FlInternalData* ctx, FlString label, bool enabled) {
    FL_UNUSED(ctx);
    FL_UNUSED(enabled);

    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::BeginMenu(temp_text, enabled);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_menu_end_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::EndMenu();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_item_impl(FlInternalData* ctx, FlString label) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::MenuItem(temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_item_ex_impl(FlInternalData* ctx, FlString label, FlString shortcut, bool selected, bool enabled) {
    FL_UNUSED(ctx);
    FL_UNUSED(enabled);

    char temp_buffer[2048];
    char temp_buffer_2[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    const char* temp_text2 =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer_2, sizeof(temp_buffer_2), shortcut);

    return ImGui::MenuItem(temp_text, temp_text2, selected, enabled);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_menu_item_toogle_impl(FlInternalData* ctx, FlString label, FlString shortcut, bool* selected, bool enabled) {
    char temp_buffer[2048];
    char temp_buffer_2[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    const char* temp_text2 =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer_2, sizeof(temp_buffer_2), shortcut);

    return ImGui::MenuItem(temp_text, temp_text2, selected, enabled);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FlMenuApi g_menu_funcs = {
    NULL,
    fl_menu_begin_bar_impl,
    fl_menu_end_bar_impl,
    fl_menu_begin_main_bar_impl,
    fl_menu_end_main_bar_impl,
    fl_menu_begin_impl,
    fl_menu_end_impl,
    fl_menu_item_impl,
    fl_menu_item_ex_impl,
    fl_menu_item_toogle_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_regular_impl(FlInternalData* ctx, FlString label) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::Button(temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_regular_size_impl(FlInternalData* ctx, FlString label, FlVec2 size) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::Button(temp_text, ImVec2(size.x, size.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_small_impl(FlInternalData* ctx, FlString label) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::SmallButton(temp_text);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_invisible_impl(FlInternalData* ctx, FlString label, FlVec2 size, FlButtonFlags flags) {
    FL_UNUSED(ctx);
    FL_UNUSED(flags);

    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::InvisibleButton(temp_text, ImVec2(size.x, size.y), (ImGuiButtonFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_check_box_impl(FlInternalData* ctx, FlString label, bool* checked) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::Checkbox(temp_text, checked);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_radio_impl(FlInternalData* ctx, FlString label, bool state) {
    char temp_buffer[2048];

    const char* temp_text =
        StringAllocator_temp_string_to_cstr(&ctx->string_allocator, temp_buffer, sizeof(temp_buffer), label);

    return ImGui::RadioButton(temp_text, state);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_button_bullet_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::Bullet();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_button_image_with_label_impl(FlInternalData* ctx, FlImage image, FlString label) {
    image_show(ctx, image);
    ImGui::SameLine();
    ImGui::TextUnformatted(label.str, label.str + label.len);
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlButtonApi g_button_funcs = {
    NULL,
    fl_button_regular_impl,
    fl_button_regular_size_impl,
    fl_button_small_impl,
    fl_button_invisible_impl,
    fl_button_check_box_impl,
    fl_button_radio_impl,
    fl_button_bullet_impl,
    fl_button_image_with_label_impl,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_hovered_impl(FlInternalData* ctx, FlHoveredFlags flags) {
    FL_UNUSED(ctx);
    return ImGui::IsItemHovered((ImGuiHoveredFlags)flags);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_active_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemActive();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_focused_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemFocused();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_clicked_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemClicked();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_visible_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemVisible();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_edited_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemEdited();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_activated_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemActivated();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_deactivated_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemDeactivated();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_deactivated_after_edit_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemDeactivatedAfterEdit();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_toggled_open_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsItemToggledOpen();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_any_hovered_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsAnyItemHovered();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_any_active_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsAnyItemActive();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL bool fl_item_is_any_focused_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    return ImGui::IsAnyItemFocused();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_item_get_rect_min_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 v = ImGui::GetItemRectMin();
    FlVec2 temp = {v.x, v.y};
    return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_item_get_rect_max_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 v = ImGui::GetItemRectMax();
    FlVec2 temp = {v.x, v.y};
    return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL FlVec2 fl_item_get_rect_size_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImVec2 v = ImGui::GetItemRectSize();
    FlVec2 temp = {v.x, v.y};
    return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FL_PUBLIC_SYMBOL void fl_item_set_allow_overlap_impl(FlInternalData* ctx) {
    FL_UNUSED(ctx);
    ImGui::SetItemAllowOverlap();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FlItemApi g_item_funcs = {
    NULL,
    fl_item_is_hovered_impl,
    fl_item_is_active_impl,
    fl_item_is_focused_impl,
    fl_item_is_clicked_impl,
    fl_item_is_visible_impl,
    fl_item_is_edited_impl,
    fl_item_is_activated_impl,
    fl_item_is_deactivated_impl,
    fl_item_is_deactivated_after_edit_impl,
    fl_item_is_toggled_open_impl,
    fl_item_is_any_hovered_impl,
    fl_item_is_any_active_impl,
    fl_item_is_any_focused_impl,
    fl_item_get_rect_min_impl,
    fl_item_get_rect_max_impl,
    fl_item_get_rect_size_impl,
    fl_item_set_allow_overlap_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int s_imgui_colors[] = {
    ImGuiCol_Text,
    ImGuiCol_TextDisabled,
    ImGuiCol_WindowBg,
    ImGuiCol_ChildBg,
    ImGuiCol_PopupBg,
    ImGuiCol_Border,
    ImGuiCol_BorderShadow,
    ImGuiCol_FrameBg,
    ImGuiCol_FrameBgHovered,
    ImGuiCol_FrameBgActive,
    ImGuiCol_TitleBg,
    ImGuiCol_TitleBgActive,
    ImGuiCol_TitleBgCollapsed,
    ImGuiCol_MenuBarBg,
    ImGuiCol_ScrollbarBg,
    ImGuiCol_ScrollbarGrab,
    ImGuiCol_ScrollbarGrabHovered,
    ImGuiCol_ScrollbarGrabActive,
    ImGuiCol_CheckMark,
    ImGuiCol_SliderGrab,
    ImGuiCol_SliderGrabActive,
    ImGuiCol_Button,
    ImGuiCol_ButtonHovered,
    ImGuiCol_ButtonActive,
    ImGuiCol_Header,
    ImGuiCol_HeaderHovered,
    ImGuiCol_HeaderActive,
    ImGuiCol_Separator,
    ImGuiCol_SeparatorHovered,
    ImGuiCol_SeparatorActive,
    ImGuiCol_ResizeGrip,
    ImGuiCol_ResizeGripHovered,
    ImGuiCol_ResizeGripActive,
    ImGuiCol_Tab,
    ImGuiCol_TabHovered,
    ImGuiCol_TabActive,
    ImGuiCol_TabUnfocused,
    ImGuiCol_TabUnfocusedActive,
    ImGuiCol_DockingPreview,
    ImGuiCol_DockingEmptyBg,
    ImGuiCol_PlotLines,
    ImGuiCol_PlotLinesHovered,
    ImGuiCol_PlotHistogram,
    ImGuiCol_PlotHistogramHovered,
    ImGuiCol_TableHeaderBg,
    ImGuiCol_TableBorderStrong,
    ImGuiCol_TableBorderLight,
    ImGuiCol_TableRowBg,
    ImGuiCol_TableRowBgAlt,
    ImGuiCol_TextSelectedBg,
    ImGuiCol_DragDropTarget,
    ImGuiCol_NavHighlight,
    ImGuiCol_NavWindowingHighlight,
    ImGuiCol_NavWindowingDimBg,
    ImGuiCol_ModalWindowDimBg,
};

static int s_flowi_colors[] = {
    FlStyleColor_Text,
    FlStyleColor_TextDisabled,
    FlStyleColor_WindowBg,
    FlStyleColor_ChildBg,
    FlStyleColor_PopupBg,
    FlStyleColor_Border,
    FlStyleColor_BorderShadow,
    FlStyleColor_FrameBg,
    FlStyleColor_FrameBgHovered,
    FlStyleColor_FrameBgActive,
    FlStyleColor_TitleBg,
    FlStyleColor_TitleBgActive,
    FlStyleColor_TitleBgCollapsed,
    FlStyleColor_MenuBarBg,
    FlStyleColor_ScrollbarBg,
    FlStyleColor_ScrollbarGrab,
    FlStyleColor_ScrollbarGrabHovered,
    FlStyleColor_ScrollbarGrabActive,
    FlStyleColor_CheckMark,
    FlStyleColor_SliderGrab,
    FlStyleColor_SliderGrabActive,
    FlStyleColor_Button,
    FlStyleColor_ButtonHovered,
    FlStyleColor_ButtonActive,
    FlStyleColor_Header,
    FlStyleColor_HeaderHovered,
    FlStyleColor_HeaderActive,
    FlStyleColor_Separator,
    FlStyleColor_SeparatorHovered,
    FlStyleColor_SeparatorActive,
    FlStyleColor_ResizeGrip,
    FlStyleColor_ResizeGripHovered,
    FlStyleColor_ResizeGripActive,
    FlStyleColor_Tab,
    FlStyleColor_TabHovered,
    FlStyleColor_TabActive,
    FlStyleColor_TabUnfocused,
    FlStyleColor_TabUnfocusedActive,
    FlStyleColor_DockingPreview,
    FlStyleColor_DockingEmptyBg,
    FlStyleColor_PlotLines,
    FlStyleColor_PlotLinesHovered,
    FlStyleColor_PlotHistogram,
    FlStyleColor_PlotHistogramHovered,
    FlStyleColor_TableHeaderBg,
    FlStyleColor_TableBorderStrong,
    FlStyleColor_TableBorderLight,
    FlStyleColor_TableRowBg,
    FlStyleColor_TableRowBgAlt,
    FlStyleColor_TextSelectedBg,
    FlStyleColor_DragDropTarget,
    FlStyleColor_NavHighlight,
    FlStyleColor_NavWindowingHighlight,
    FlStyleColor_NavWindowingDimBg,
    FlStyleColor_ModalWindowDimBg,
};

static int s_fl_single_styles[] = {
    FlStyleSingle_Alpha,
    FlStyleSingle_DisabledAlpha,
    FlStyleSingle_WindowRounding,
    FlStyleSingle_WindowBorderSize,
    FlStyleSingle_ChildRounding,
    FlStyleSingle_ChildBorderSize,
    FlStyleSingle_PopupRounding,
    FlStyleSingle_PopupBorderSize,
    FlStyleSingle_FrameRounding,
    FlStyleSingle_FrameBorderSize,
    FlStyleSingle_IndentSpacing,
    FlStyleSingle_ScrollbarSize,
    FlStyleSingle_ScrollbarRounding,
    FlStyleSingle_GrabMinSize,
    FlStyleSingle_GrabRounding,
    FlStyleSingle_TabRounding,
};

static int s_imgui_single_styles[] = {
    ImGuiStyleVar_Alpha,
    ImGuiStyleVar_DisabledAlpha,
    ImGuiStyleVar_WindowRounding,
    ImGuiStyleVar_WindowBorderSize,
    ImGuiStyleVar_ChildRounding,
    ImGuiStyleVar_ChildBorderSize,
    ImGuiStyleVar_PopupRounding,
    ImGuiStyleVar_PopupBorderSize,
    ImGuiStyleVar_FrameRounding,
    ImGuiStyleVar_FrameBorderSize,
    ImGuiStyleVar_IndentSpacing,
    ImGuiStyleVar_ScrollbarSize,
    ImGuiStyleVar_ScrollbarRounding,
    ImGuiStyleVar_GrabMinSize,
    ImGuiStyleVar_GrabRounding,
    ImGuiStyleVar_TabRounding,
};

static int s_fl_vec2_styles[] = {
    FlStyleVec2_WindowPadding,
    FlStyleVec2_WindowMinSize,
    FlStyleVec2_WindowTitleAlign,
    FlStyleVec2_FramePadding,
    FlStyleVec2_ItemSpacing,
    FlStyleVec2_ItemInnerSpacing,
    FlStyleVec2_CellPadding,
    FlStyleVec2_ButtonTextAlign,
    FlStyleVec2_SelectableTextAlign,
};

static int s_imgui_vec2_styles[] = {
    ImGuiStyleVar_WindowPadding,
    ImGuiStyleVar_WindowMinSize,
    ImGuiStyleVar_WindowTitleAlign,
    ImGuiStyleVar_FramePadding,
    ImGuiStyleVar_ItemSpacing,
    ImGuiStyleVar_ItemInnerSpacing,
    ImGuiStyleVar_CellPadding,
    ImGuiStyleVar_ButtonTextAlign,
    ImGuiStyleVar_SelectableTextAlign,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fl_style_init_priv() {
    for (int i = 0; i < ImGuiCol_COUNT; ++i) {
        s_color_lut[s_imgui_colors[i]] = s_flowi_colors[i];
    }

    for (uint32_t i = 0; i < FL_SIZEOF_ARRAY(s_fl_single_styles); ++i) {
        s_single_style_lut[s_fl_single_styles[i]] = s_imgui_single_styles[i];
    }

    for (uint32_t i = 0; i < FL_SIZEOF_ARRAY(s_fl_vec2_styles); ++i) {
        s_vec2_style_lut[s_fl_vec2_styles[i]] = s_imgui_vec2_styles[i];
    }
}


//extern FlFontApi g_font_funcs;
//extern FlImageApi g_f
//extern "C" FlImageApi g_image_funcs;
//extern FlButtonApi g_button_funcs;
//extern FlCursorApi g_cursor_funcs;
//extern FlItemApi g_item_funcs;
//extern FlMenuApi g_menu_funcs;
//extern FlStyleApi g_style_funcs;
//extern FlTextApi g_text_funcs;
//extern FlUiApi g_ui_funcs;
//extern FlWindowApi g_window_funcs;
//extern FlIoApi g_io_funcs;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlImageApi* fl_get_image_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->image_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlUiApi* fl_get_ui_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->ui_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlWindowApi* fl_get_window_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->window_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlCursorApi* fl_get_cursor_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->cursor_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlTextApi* fl_get_text_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->text_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlMenuApi* fl_get_menu_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->menu_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlFontApi* fl_get_font_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->font_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlButtonApi* fl_get_button_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->button_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlItemApi* fl_get_item_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->item_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlStyleApi* fl_get_style_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    return &app_state->state->style_api;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlPainterApi* fl_get_painter_api(AppState* app_state, int version) {
    FL_UNUSED(version);
    FL_UNUSED(app_state);
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" FlImage fl_image_create_from_file_impl(struct FlInternalData* priv, FlString filename);
extern "C" FlImage fl_image_create_from_file_block_impl(struct FlInternalData* priv, FlString filename);
extern "C" FlImageInfo* fl_image_get_info_impl(struct FlInternalData* priv, FlImage image);

FlImageApi g_image_funcs = {
    nullptr,
    fl_image_create_from_file_impl,
    fl_image_create_from_file_block_impl,
    fl_image_get_info_impl
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_mouse_pos_event_impl(struct FlInternalData* priv, float x, float y) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_mouse_button_event_impl(struct FlInternalData* priv, int button, bool down) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    printf("fl_input_add_mouse_button_event_impl %d %d\n", button, down);
    io.AddMouseButtonEvent(button, down);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_mouse_wheel_event_impl(struct FlInternalData* priv, float x, float y) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseWheelEvent(x, y);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_mouse_source_event_impl(struct FlInternalData* priv, FlMouseSource source) {
    FL_UNUSED(priv);
    FL_UNUSED(source);
    //ImGuiIO& io = ImGui::GetIO();
    //io.AddMouseSourceEvent((ImGuiMouseSource)source);
    printf("fl_input_add_mouse_source_event_impl - not implemented\n");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_focus_event_impl(struct FlInternalData* priv, bool focused) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddFocusEvent(focused);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_char_event_impl(struct FlInternalData* priv, int c) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(c);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_update_screen_size_time_impl(struct FlInternalData* priv, float display_w, float display_h,
                                           float w, float h, float delta_time) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / (float)w, (float)display_h / (float)h);

    io.DeltaTime = delta_time;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_key_event_impl(struct FlInternalData* priv, FlKey key, bool down);
extern "C" void fl_input_add_key_analog_event_impl(struct FlInternalData* priv, FlKey key, bool down, float value);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FlInputApi g_input_funcs = {
    nullptr,
    fl_input_add_key_event_impl,
    fl_input_add_key_analog_event_impl,
    fl_input_add_mouse_pos_event_impl,
    fl_input_add_mouse_button_event_impl,
    fl_input_add_mouse_wheel_event_impl,
    fl_input_add_mouse_source_event_impl,
    fl_input_add_focus_event_impl,
    fl_input_add_char_event_impl,
    fl_input_update_screen_size_time_impl,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void* c_create(const FlApplicationSettings* settings, void* rust_state) {
    FL_UNUSED(settings);

    ImGui::CreateContext();

    // Setup Dear ImGui context
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
    io.DisplaySize = ImVec2(settings->width, settings->height);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = NULL;

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border]                 = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding                     = ImVec2(8.00f, 8.00f);
    style.FramePadding                      = ImVec2(5.00f, 2.00f);
    style.CellPadding                       = ImVec2(6.00f, 6.00f);
    style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
    style.IndentSpacing                     = 25;
    style.ScrollbarSize                     = 15;
    style.GrabMinSize                       = 10;
    style.WindowBorderSize                  = 1;
    style.ChildBorderSize                   = 1;
    style.PopupBorderSize                   = 1;
    style.FrameBorderSize                   = 1;
    style.TabBorderSize                     = 1;
    style.WindowRounding                    = 7;
    style.ChildRounding                     = 4;
    style.FrameRounding                     = 3;
    style.PopupRounding                     = 4;
    style.ScrollbarRounding                 = 9;
    style.GrabRounding                      = 3;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 4;

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //ImGui_ImplGlfw_InitForOther(window, true);
    //glfwSetKeyCallback(window, key_callback);

    FlInternalData* state = new FlInternalData;
    state->rust_state = rust_state;
    //state->window = window;

    state->button_api = g_button_funcs;
    state->cursor_api = g_cursor_funcs;
    //state->font_api = g_font_funcs;
    state->image_api = g_image_funcs;
    state->input_api = g_input_funcs;
    state->item_api = g_item_funcs;
    state->menu_api = g_menu_funcs;
    state->style_api = g_style_funcs;
    state->text_api = g_text_funcs;
    state->ui_api = g_ui_funcs;
    state->window_api = g_window_funcs;

    state->button_api.priv = state;
    state->cursor_api.priv = state;
    state->font_api.priv = state;
    state->image_api.priv = state;
    state->input_api.priv = state;
    state->item_api.priv = state;
    state->menu_api.priv = state;
    state->style_api.priv = state;
    state->text_api.priv = state;
    state->ui_api.priv = state;
    state->window_api.priv = state;
    
    // TODO: Move
    g_flowi_button_api = &state->button_api;
    g_flowi_cursor_api = &state->cursor_api;
    g_flowi_input_api = &state->input_api;
    g_flowi_image_api = &state->image_api; 
    g_flowi_io_api = &state->io_api;
    g_flowi_item_api = &state->item_api;
    g_flowi_menu_api = &state->menu_api;
    //g_flowi_painter_api = nullptr;//&state->painter;
    g_flowi_style_api = &state->style_api;
    g_flowi_text_api = &state->text_api;
    g_flowi_ui_api = &state->ui_api;
    g_flowi_window_api = &state->window_api;

    LinearAllocator_create_with_allocator(&state->frame_allocator, "string tracking allocator", &malloc_allocator,
                                          10 * 1024, true);

    StringAllocator_create(&state->string_allocator, &malloc_allocator, &state->frame_allocator);

    return state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern "C" void Errors_add(FlError err, const char* filename, int line, const char* fmt, ...) {
    FL_UNUSED(err);
    FL_UNUSED(line);
    FL_UNUSED(filename);
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    // printf("ERROR:%d | %s:%d: %s\n", err, filename, line, buffer);
    va_end(args);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ImGuiKey translate_flkey_to_imgui(FlKey key) {
    ImGuiKey imgui_key = ImGuiKey_COUNT;

    // the compiler will translate this to a jump table
    switch (key) {
        case FlKey_Tab: imgui_key = ImGuiKey_Tab; break;
        case FlKey_LeftArrow: imgui_key = ImGuiKey_LeftArrow; break;
        case FlKey_RightArrow: imgui_key = ImGuiKey_RightArrow; break;
        case FlKey_UpArrow: imgui_key = ImGuiKey_UpArrow; break;
        case FlKey_DownArrow: imgui_key = ImGuiKey_DownArrow; break;
        case FlKey_PageUp: imgui_key = ImGuiKey_PageUp; break;
        case FlKey_PageDown: imgui_key = ImGuiKey_PageDown; break;
        case FlKey_Home: imgui_key = ImGuiKey_Home; break;
        case FlKey_End: imgui_key = ImGuiKey_End; break;
        case FlKey_Insert: imgui_key = ImGuiKey_Insert; break;
        case FlKey_Delete : imgui_key = ImGuiKey_Delete; break;
        case FlKey_Backspace : imgui_key = ImGuiKey_Backspace; break;
        case FlKey_Space : imgui_key = ImGuiKey_Space; break;
        case FlKey_Enter : imgui_key = ImGuiKey_Enter; break;
        case FlKey_Escape : imgui_key = ImGuiKey_Escape; break;
        case FlKey_LeftCtrl : imgui_key = ImGuiKey_LeftCtrl; break;
        case FlKey_LeftShift : imgui_key = ImGuiKey_LeftShift; break;
        case FlKey_LeftAlt : imgui_key = ImGuiKey_LeftAlt; break;
        case FlKey_LeftSuper : imgui_key = ImGuiKey_LeftSuper; break;
        case FlKey_RightCtrl : imgui_key = ImGuiKey_RightCtrl; break;
        case FlKey_RightShift : imgui_key = ImGuiKey_RightShift; break;
        case FlKey_RightAlt : imgui_key = ImGuiKey_RightAlt; break;
        case FlKey_RightSuper : imgui_key = ImGuiKey_RightSuper; break;
        case FlKey_Menu : imgui_key = ImGuiKey_Menu; break;
        case FlKey_A : imgui_key = ImGuiKey_A; break;
        case FlKey_B : imgui_key = ImGuiKey_B; break;
        case FlKey_C : imgui_key = ImGuiKey_C; break;
        case FlKey_D : imgui_key = ImGuiKey_D; break;
        case FlKey_E : imgui_key = ImGuiKey_E; break;
        case FlKey_F : imgui_key = ImGuiKey_F; break;
        case FlKey_G : imgui_key = ImGuiKey_G; break;
        case FlKey_H : imgui_key = ImGuiKey_H; break;
        case FlKey_I : imgui_key = ImGuiKey_I; break;
        case FlKey_J : imgui_key = ImGuiKey_J; break;
        case FlKey_K : imgui_key = ImGuiKey_K; break;
        case FlKey_L : imgui_key = ImGuiKey_L; break;
        case FlKey_M : imgui_key = ImGuiKey_M; break;
        case FlKey_N : imgui_key = ImGuiKey_N; break;
        case FlKey_O : imgui_key = ImGuiKey_O; break;
        case FlKey_P : imgui_key = ImGuiKey_P; break;
        case FlKey_Q : imgui_key = ImGuiKey_Q; break;
        case FlKey_R : imgui_key = ImGuiKey_R; break;
        case FlKey_S : imgui_key = ImGuiKey_S; break;
        case FlKey_T : imgui_key = ImGuiKey_T; break;
        case FlKey_U : imgui_key = ImGuiKey_U; break;
        case FlKey_V : imgui_key = ImGuiKey_V; break;
        case FlKey_W : imgui_key = ImGuiKey_W; break;
        case FlKey_X : imgui_key = ImGuiKey_X; break;
        case FlKey_Y : imgui_key = ImGuiKey_Y; break;
        case FlKey_Z : imgui_key = ImGuiKey_Z; break;
        case FlKey_F1 : imgui_key = ImGuiKey_F1; break;
        case FlKey_F2 : imgui_key = ImGuiKey_F2; break;
        case FlKey_F3 : imgui_key = ImGuiKey_F3; break;
        case FlKey_F4 : imgui_key = ImGuiKey_F4; break;
        case FlKey_F5 : imgui_key = ImGuiKey_F5; break;
        case FlKey_F6 : imgui_key = ImGuiKey_F6; break;
        case FlKey_F7 : imgui_key = ImGuiKey_F7; break;
        case FlKey_F8 : imgui_key = ImGuiKey_F8; break;
        case FlKey_F9 : imgui_key = ImGuiKey_F9; break;
        case FlKey_F10 : imgui_key = ImGuiKey_F10; break;
        case FlKey_F11 : imgui_key = ImGuiKey_F11; break;
        case FlKey_F12 : imgui_key = ImGuiKey_F12; break;
        case FlKey_Apostrophe : imgui_key = ImGuiKey_Apostrophe; break;
        case FlKey_Comma : imgui_key = ImGuiKey_Comma; break;
        case FlKey_Minus : imgui_key = ImGuiKey_Minus; break;
        case FlKey_Period : imgui_key = ImGuiKey_Period; break;
        case FlKey_Slash : imgui_key = ImGuiKey_Slash; break;
        case FlKey_Semicolon : imgui_key = ImGuiKey_Semicolon; break;
        case FlKey_Equal : imgui_key = ImGuiKey_Equal; break;
        case FlKey_LeftBracket : imgui_key = ImGuiKey_LeftBracket; break;
        case FlKey_Backslash : imgui_key = ImGuiKey_Backslash; break;
        case FlKey_RightBracket : imgui_key = ImGuiKey_RightBracket; break;
        case FlKey_GraveAccent : imgui_key = ImGuiKey_GraveAccent; break;
        case FlKey_CapsLock : imgui_key = ImGuiKey_CapsLock; break;
        case FlKey_ScrollLock : imgui_key = ImGuiKey_ScrollLock; break;
        case FlKey_NumLock : imgui_key = ImGuiKey_NumLock; break;
        case FlKey_PrintScreen : imgui_key = ImGuiKey_PrintScreen; break;
        case FlKey_Pause : imgui_key = ImGuiKey_Pause; break;
        case FlKey_Keypad0 : imgui_key = ImGuiKey_Keypad0; break;
        case FlKey_Keypad1 : imgui_key = ImGuiKey_Keypad1; break;
        case FlKey_Keypad2 : imgui_key = ImGuiKey_Keypad2; break;
        case FlKey_Keypad3 : imgui_key = ImGuiKey_Keypad3; break;
        case FlKey_Keypad4 : imgui_key = ImGuiKey_Keypad4; break;
        case FlKey_Keypad5 : imgui_key = ImGuiKey_Keypad5; break;
        case FlKey_Keypad6 : imgui_key = ImGuiKey_Keypad6; break;
        case FlKey_Keypad7 : imgui_key = ImGuiKey_Keypad7; break;
        case FlKey_Keypad8 : imgui_key = ImGuiKey_Keypad8; break;
        case FlKey_Keypad9 : imgui_key = ImGuiKey_Keypad9; break;
        case FlKey_KeypadDecimal : imgui_key = ImGuiKey_KeypadDecimal; break;
        case FlKey_KeypadDivide : imgui_key = ImGuiKey_KeypadDivide; break;
        case FlKey_KeypadMultiply : imgui_key = ImGuiKey_KeypadMultiply; break; 
        case FlKey_KeypadSubtract: imgui_key = ImGuiKey_KeypadSubtract; break;
        case FlKey_KeypadAdd: imgui_key = ImGuiKey_KeypadAdd; break;
        case FlKey_KeypadEnter: imgui_key = ImGuiKey_KeypadEnter; break;
        case FlKey_KeypadEqual: imgui_key = ImGuiKey_KeypadEqual; break;
        case FlKey_GamepadStart: imgui_key = ImGuiKey_GamepadStart; break;
        case FlKey_GamepadBack: imgui_key = ImGuiKey_GamepadBack; break;
        case FlKey_GamepadFaceLeft: imgui_key = ImGuiKey_GamepadFaceLeft; break;
        case FlKey_GamepadFaceRight: imgui_key = ImGuiKey_GamepadFaceRight; break;
        case FlKey_GamepadFaceUp: imgui_key = ImGuiKey_GamepadFaceUp; break;
        case FlKey_GamepadFaceDown: imgui_key = ImGuiKey_GamepadFaceDown; break;
        case FlKey_GamepadDpadLeft: imgui_key = ImGuiKey_GamepadDpadLeft; break;
        case FlKey_GamepadDpadRight: imgui_key = ImGuiKey_GamepadDpadRight; break;
        case FlKey_GamepadDpadUp: imgui_key = ImGuiKey_GamepadDpadUp; break;
        case FlKey_GamepadDpadDown: imgui_key = ImGuiKey_GamepadDpadDown; break;
        case FlKey_GamepadL1: imgui_key = ImGuiKey_GamepadL1; break;
        case FlKey_GamepadR1: imgui_key = ImGuiKey_GamepadR1; break;
        case FlKey_GamepadL2: imgui_key = ImGuiKey_GamepadL2; break;
        case FlKey_GamepadR2: imgui_key = ImGuiKey_GamepadR2; break;
        case FlKey_GamepadL3: imgui_key = ImGuiKey_GamepadL3; break;
        case FlKey_GamepadR3: imgui_key = ImGuiKey_GamepadR3; break;
        case FlKey_GamepadLStickLeft: imgui_key = ImGuiKey_GamepadLStickLeft; break;
        case FlKey_GamepadLStickRight: imgui_key = ImGuiKey_GamepadLStickRight; break;
        case FlKey_GamepadLStickUp: imgui_key = ImGuiKey_GamepadLStickUp; break;
        case FlKey_GamepadLStickDown: imgui_key = ImGuiKey_GamepadLStickDown; break;
        case FlKey_GamepadRStickLeft: imgui_key = ImGuiKey_GamepadRStickLeft; break;
        case FlKey_GamepadRStickRight: imgui_key = ImGuiKey_GamepadRStickRight; break;
        case FlKey_GamepadRStickUp: imgui_key = ImGuiKey_GamepadRStickUp; break;
        case FlKey_GamepadRStickDown: imgui_key = ImGuiKey_GamepadRStickDown; break;
        case FlKey_MouseLeft: imgui_key = ImGuiKey_MouseLeft; break;
        case FlKey_MouseRight: imgui_key = ImGuiKey_MouseRight; break;
        case FlKey_MouseMiddle: imgui_key = ImGuiKey_MouseMiddle; break;
        case FlKey_MouseX1: imgui_key = ImGuiKey_MouseX1; break;
        case FlKey_MouseX2: imgui_key = ImGuiKey_MouseX2; break;
        case FlKey_MouseWheelX: imgui_key = ImGuiKey_MouseWheelX; break;
        case FlKey_MouseWheelY: imgui_key = ImGuiKey_MouseWheelY; break;
        case FlKey_ModCtrl: imgui_key = ImGuiMod_Ctrl; break;
        case FlKey_ModShift: imgui_key = ImGuiMod_Shift; break;
        case FlKey_ModAlt: imgui_key = ImGuiMod_Alt; break;
        case FlKey_ModSuper: imgui_key = ImGuiMod_Super; break;
        case FlKey_ModShortcut: imgui_key = ImGuiMod_Super; break;
    }

    return imgui_key;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_key_event_impl(struct FlInternalData* priv, FlKey key, bool down) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(translate_flkey_to_imgui(key), down);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" void fl_input_add_key_analog_event_impl(struct FlInternalData* priv, FlKey key, bool down, float value) {
    FL_UNUSED(priv);
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyAnalogEvent(translate_flkey_to_imgui(key), down, value);
}


