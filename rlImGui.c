/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2024 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/
#include "rlImGui.h"

#include "imgui_impl_raylib.h"

#include "raylib.h"
#include "rlgl.h"

#include <math.h>
#include <stdint.h>
#include <string.h>

#ifndef NO_FONT_AWESOME
#include "extras/FA6FreeSolidFontData.h"
#endif

#define nullptr 0

static ImGuiMouseCursor CurrentMouseCursor = ImGuiMouseCursor_COUNT;
static MouseCursor MouseCursorMap[ImGuiMouseCursor_COUNT];

ImGuiContext* GlobalContext = nullptr;

typedef struct
{
    KeyboardKey RaylibKey;
    ImGuiKey ImGuiKey;
} KeyPairing;

static KeyPairing RaylibKeyMap[] = {
    { KEY_APOSTROPHE, ImGuiKey_Apostrophe},
    { KEY_COMMA, ImGuiKey_Comma},
    { KEY_MINUS, ImGuiKey_Minus},
    { KEY_PERIOD, ImGuiKey_Period},
    { KEY_SLASH, ImGuiKey_Slash},
    { KEY_ZERO, ImGuiKey_0},
    { KEY_ONE, ImGuiKey_1},
    { KEY_TWO, ImGuiKey_2},
    { KEY_THREE, ImGuiKey_3},
    { KEY_FOUR, ImGuiKey_4},
    { KEY_FIVE, ImGuiKey_5},
    { KEY_SIX, ImGuiKey_6},
    { KEY_SEVEN, ImGuiKey_7},
    { KEY_EIGHT, ImGuiKey_8},
    { KEY_NINE, ImGuiKey_9},
    { KEY_SEMICOLON, ImGuiKey_Semicolon},
    { KEY_EQUAL, ImGuiKey_Equal},
    { KEY_A, ImGuiKey_A},
    { KEY_B, ImGuiKey_B},
    { KEY_C, ImGuiKey_C},
    { KEY_D, ImGuiKey_D},
    { KEY_E, ImGuiKey_E},
    { KEY_F, ImGuiKey_F},
    { KEY_G, ImGuiKey_G},
    { KEY_H, ImGuiKey_H},
    { KEY_I, ImGuiKey_I},
    { KEY_J, ImGuiKey_J},
    { KEY_K, ImGuiKey_K},
    { KEY_L, ImGuiKey_L},
    { KEY_M, ImGuiKey_M},
    { KEY_N, ImGuiKey_N},
    { KEY_O, ImGuiKey_O},
    { KEY_P, ImGuiKey_P},
    { KEY_Q, ImGuiKey_Q},
    { KEY_R, ImGuiKey_R},
    { KEY_S, ImGuiKey_S},
    { KEY_T, ImGuiKey_T},
    { KEY_U, ImGuiKey_U},
    { KEY_V, ImGuiKey_V},
    { KEY_W, ImGuiKey_W},
    { KEY_X, ImGuiKey_X},
    { KEY_Y, ImGuiKey_Y},
    { KEY_Z, ImGuiKey_Z},
    { KEY_SPACE, ImGuiKey_Space},
    { KEY_ESCAPE, ImGuiKey_Escape},
    { KEY_ENTER, ImGuiKey_Enter},
    { KEY_TAB, ImGuiKey_Tab},
    { KEY_BACKSPACE, ImGuiKey_Backspace},
    { KEY_INSERT, ImGuiKey_Insert},
    { KEY_DELETE, ImGuiKey_Delete},
    { KEY_RIGHT, ImGuiKey_RightArrow},
    { KEY_LEFT, ImGuiKey_LeftArrow},
    { KEY_DOWN, ImGuiKey_DownArrow},
    { KEY_UP, ImGuiKey_UpArrow},
    { KEY_PAGE_UP, ImGuiKey_PageUp},
    { KEY_PAGE_DOWN, ImGuiKey_PageDown},
    { KEY_HOME, ImGuiKey_Home},
    { KEY_END, ImGuiKey_End},
    { KEY_CAPS_LOCK, ImGuiKey_CapsLock},
    { KEY_SCROLL_LOCK, ImGuiKey_ScrollLock},
    { KEY_NUM_LOCK, ImGuiKey_NumLock},
    { KEY_PRINT_SCREEN, ImGuiKey_PrintScreen},
    { KEY_PAUSE, ImGuiKey_Pause},
    { KEY_F1, ImGuiKey_F1},
    { KEY_F2, ImGuiKey_F2},
    { KEY_F3, ImGuiKey_F3},
    { KEY_F4, ImGuiKey_F4},
    { KEY_F5, ImGuiKey_F5},
    { KEY_F6, ImGuiKey_F6},
    { KEY_F7, ImGuiKey_F7},
    { KEY_F8, ImGuiKey_F8},
    { KEY_F9, ImGuiKey_F9},
    { KEY_F10, ImGuiKey_F10},
    { KEY_F11, ImGuiKey_F11},
    { KEY_F12, ImGuiKey_F12},
    { KEY_LEFT_SHIFT, ImGuiKey_LeftShift},
    { KEY_LEFT_CONTROL, ImGuiKey_LeftCtrl},
    { KEY_LEFT_ALT, ImGuiKey_LeftAlt},
    { KEY_LEFT_SUPER, ImGuiKey_LeftSuper},
    { KEY_RIGHT_SHIFT, ImGuiKey_RightShift},
    { KEY_RIGHT_CONTROL, ImGuiKey_RightCtrl},
    { KEY_RIGHT_ALT, ImGuiKey_RightAlt},
    { KEY_RIGHT_SUPER, ImGuiKey_RightSuper},
    { KEY_KB_MENU, ImGuiKey_Menu},
    { KEY_LEFT_BRACKET, ImGuiKey_LeftBracket},
    { KEY_BACKSLASH, ImGuiKey_Backslash},
    { KEY_RIGHT_BRACKET, ImGuiKey_RightBracket},
    { KEY_GRAVE, ImGuiKey_GraveAccent},
    { KEY_KP_0, ImGuiKey_Keypad0},
    { KEY_KP_1, ImGuiKey_Keypad1},
    { KEY_KP_2, ImGuiKey_Keypad2},
    { KEY_KP_3, ImGuiKey_Keypad3},
    { KEY_KP_4, ImGuiKey_Keypad4},
    { KEY_KP_5, ImGuiKey_Keypad5},
    { KEY_KP_6, ImGuiKey_Keypad6},
    { KEY_KP_7, ImGuiKey_Keypad7},
    { KEY_KP_8, ImGuiKey_Keypad8},
    { KEY_KP_9, ImGuiKey_Keypad9},
    { KEY_KP_DECIMAL, ImGuiKey_KeypadDecimal},
    { KEY_KP_DIVIDE, ImGuiKey_KeypadDivide},
    { KEY_KP_MULTIPLY, ImGuiKey_KeypadMultiply},
    { KEY_KP_SUBTRACT, ImGuiKey_KeypadSubtract},
    { KEY_KP_ADD, ImGuiKey_KeypadAdd},
    { KEY_KP_ENTER, ImGuiKey_KeypadEnter},
    { KEY_KP_EQUAL, ImGuiKey_KeypadEqual},
};

static bool LastFrameFocused = false;

static bool LastControlPressed = false;
static bool LastShiftPressed = false;
static bool LastAltPressed = false;
static bool LastSuperPressed = false;

// internal only functions
bool rlImGuiIsControlDown() { return IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL); }
bool rlImGuiIsShiftDown() { return IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT); }
bool rlImGuiIsAltDown() { return IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT); }
bool rlImGuiIsSuperDown() { return IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER); }

void ReloadFonts(void)
{
    ImGuiIO* io = igGetIO();
    unsigned char* pixels = nullptr;

    int width;
    int height;
    ImFontAtlas_GetTexDataAsRGBA32(io->Fonts, &pixels, &width, &height, nullptr);
    Image image = GenImageColor(width, height, BLANK);
    memcpy(image.data, pixels, width * height * 4);

    Texture2D* fontTexture = (Texture2D*)io->Fonts->TexID;
    if (fontTexture && fontTexture->id != 0)
    {
        UnloadTexture(*fontTexture);
        MemFree(fontTexture);
    }

    fontTexture = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *fontTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    io->Fonts->TexID = (ImTextureID)fontTexture;
}

static const char* GetClipTextCallback(ImGuiContext* context)
{
    return GetClipboardText();
}

static void SetClipTextCallback(ImGuiContext* context, const char* text)
{
    SetClipboardText(text);
}

static void ImGuiNewFrame(float deltaTime)
{
    ImGuiIO* io = igGetIO();

    Vector2 resolutionScale = GetWindowScaleDPI();

#ifndef PLATFORM_DRM
    if (IsWindowFullscreen())
    {
        int monitor = GetCurrentMonitor();
        io->DisplaySize.x = (float)GetMonitorWidth(monitor);
        io->DisplaySize.y = (float)GetMonitorHeight(monitor);
    }
    else
    {
        io->DisplaySize.x = (float)GetScreenWidth();
        io->DisplaySize.y = (float)GetScreenHeight();
    }

#if !defined(__APPLE__)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI))
        resolutionScale = (Vector2){ 1,1 };
#endif
#else
    io->DisplaySize.x = float(GetScreenWidth());
    io->DisplaySize.y = float(GetScreenHeight());
#endif

    io->DisplayFramebufferScale = (ImVec2){resolutionScale.x, resolutionScale.y};

    if (deltaTime <= 0)
        deltaTime = 0.001f;

    io->DeltaTime = deltaTime;

    if (igGetIO()->BackendFlags & ImGuiBackendFlags_HasMouseCursors)
    {
        if ((io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0)
        {
            ImGuiMouseCursor imgui_cursor = igGetMouseCursor();
            if (imgui_cursor != CurrentMouseCursor || io->MouseDrawCursor)
            {
                CurrentMouseCursor = imgui_cursor;
                if (io->MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
                {
                    HideCursor();
                }
                else
                {
                    ShowCursor();

                    if (!(io->ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange))
                    {
                        SetMouseCursor((imgui_cursor > -1 && imgui_cursor < ImGuiMouseCursor_COUNT) ? MouseCursorMap[imgui_cursor] : MOUSE_CURSOR_DEFAULT);
                    }
                }
            }
        }
    }
}

static void ImGuiTriangleVert(const ImDrawVert* idx_vert)
{
    Color* c;
    c = (Color*)&idx_vert->col;
    rlColor4ub(c->r, c->g, c->b, c->a);
    rlTexCoord2f(idx_vert->uv.x, idx_vert->uv.y);
    rlVertex2f(idx_vert->pos.x, idx_vert->pos.y);
}

static void ImGuiRenderTriangles(unsigned int count, int indexStart, const ImDrawIdx* indexBuffer, const ImDrawVert* vertBuffer, void* texturePtr)
{
    if (count < 3)
        return;

    Texture* texture = (Texture*)texturePtr;

    unsigned int textureId = (texture == nullptr) ? 0 : texture->id;

    rlBegin(RL_TRIANGLES);
    rlSetTexture(textureId);

    for (unsigned int i = 0; i <= (count - 3); i += 3)
    {
        const ImDrawIdx indexA = indexBuffer[indexStart + i];
        const ImDrawIdx indexB = indexBuffer[indexStart + i + 1];
        const ImDrawIdx indexC = indexBuffer[indexStart + i + 2];

        const ImDrawVert* vertexA = vertBuffer + indexA;
        const ImDrawVert* vertexB = vertBuffer + indexB;
        const ImDrawVert* vertexC = vertBuffer + indexC;

        ImGuiTriangleVert(vertexA);
        ImGuiTriangleVert(vertexB);
        ImGuiTriangleVert(vertexC);
    }
    rlEnd();
}

static void EnableScissor(float x, float y, float width, float height)
{
    rlEnableScissorTest();
    ImGuiIO* io = igGetIO();

    ImVec2 scale = io->DisplayFramebufferScale;
#if !defined(__APPLE__)
    if (!IsWindowState(FLAG_WINDOW_HIGHDPI))
    {
        scale.x = 1;
        scale.y = 1;
    }
#endif

    rlScissor((int)(x * scale.x),
        (int)((io->DisplaySize.y - (int)(y + height)) * scale.y),
        (int)(width * scale.x),
        (int)(height * scale.y));
}

static void SetupMouseCursors(void)
{
    MouseCursorMap[ImGuiMouseCursor_Arrow] = MOUSE_CURSOR_ARROW;
    MouseCursorMap[ImGuiMouseCursor_TextInput] = MOUSE_CURSOR_IBEAM;
    MouseCursorMap[ImGuiMouseCursor_Hand] = MOUSE_CURSOR_POINTING_HAND;
    MouseCursorMap[ImGuiMouseCursor_ResizeAll] = MOUSE_CURSOR_RESIZE_ALL;
    MouseCursorMap[ImGuiMouseCursor_ResizeEW] = MOUSE_CURSOR_RESIZE_EW;
    MouseCursorMap[ImGuiMouseCursor_ResizeNESW] = MOUSE_CURSOR_RESIZE_NESW;
    MouseCursorMap[ImGuiMouseCursor_ResizeNS] = MOUSE_CURSOR_RESIZE_NS;
    MouseCursorMap[ImGuiMouseCursor_ResizeNWSE] = MOUSE_CURSOR_RESIZE_NWSE;
    MouseCursorMap[ImGuiMouseCursor_NotAllowed] = MOUSE_CURSOR_NOT_ALLOWED;
}

void SetupFontAwesome(void)
{
#ifndef NO_FONT_AWESOME
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.FontDataOwnedByAtlas = false;

    icons_config.GlyphMaxAdvanceX = std::numeric_limits<float>::max();
    icons_config.RasterizerMultiply = 1.0f;
    icons_config.OversampleH = 2;
    icons_config.OversampleV = 1;

    icons_config.GlyphRanges = icons_ranges;

    ImGuiIO* io = igGetIO();

    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, (void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, FONT_AWESOME_ICON_SIZE, &icons_config, icons_ranges);
#endif

}

void SetupBackend(void)
{
    ImGuiIO* io = igGetIO();
    io->BackendPlatformName = "imgui_impl_raylib";
    io->BackendFlags |= ImGuiBackendFlags_HasGamepad | ImGuiBackendFlags_HasSetMousePos;

#ifndef PLATFORM_DRM
    io->BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
#endif

    io->MousePos = (ImVec2){0, 0};

    ImGuiPlatformIO* platformIO = igGetPlatformIO();

    platformIO->Platform_SetClipboardTextFn = SetClipTextCallback;
    platformIO->Platform_GetClipboardTextFn = GetClipTextCallback;
    platformIO->Platform_ClipboardUserData = nullptr;
}

void rlImGuiEndInitImGui(void)
{
    igSetCurrentContext(GlobalContext);

    SetupFontAwesome();

    SetupMouseCursors();

    SetupBackend();

    ReloadFonts();
}

static void SetupGlobals(void)
{
    LastFrameFocused = IsWindowFocused();
    LastControlPressed = false;
    LastShiftPressed = false;
    LastAltPressed = false;
    LastSuperPressed = false;
}

void rlImGuiBeginInitImGui(void)
{
    SetupGlobals();
    if (GlobalContext == nullptr)
        GlobalContext = igCreateContext(nullptr);

    ImGuiIO* io = igGetIO();

    ImFontAtlas_AddFontDefault(io->Fonts, nullptr);
}

void rlImGuiSetup(bool dark)
{
    rlImGuiBeginInitImGui();

    ImGuiStyle* style = igGetStyle();

    if (dark)
        igStyleColorsDark(style);
    else
        igStyleColorsLight(style);

    rlImGuiEndInitImGui();
}

void rlImGuiSetContext(ImGuiContext *imgui_context)
{
    GlobalContext = imgui_context;
}

ImGuiContext* rlImGuiGetContext()
{
    return GlobalContext;
}

void rlImGuiReloadFonts(void)
{
    igSetCurrentContext(GlobalContext);

    ReloadFonts();
}

void rlImGuiBegin(void)
{
    igSetCurrentContext(GlobalContext);
    rlImGuiBeginDelta(GetFrameTime());
}

void rlImGuiBeginDelta(float deltaTime)
{
    igSetCurrentContext(GlobalContext);

    ImGuiNewFrame(deltaTime);
    ImGui_ImplRaylib_ProcessEvents();
    igNewFrame();
}

void rlImGuiEnd(void)
{
    igSetCurrentContext(GlobalContext);
    igRender();
    ImGui_ImplRaylib_RenderDrawData(igGetDrawData());
}

void rlImGuiShutdown(void)
{
    if (GlobalContext == nullptr)
        return;

    igSetCurrentContext(GlobalContext);
    ImGui_ImplRaylib_Shutdown();

    igDestroyContext(GlobalContext);
    GlobalContext = nullptr;
}

void rlImGuiImage(const Texture* image)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    igImage((ImTextureID)image, (ImVec2){(float)image->width, (float)image->height}, (ImVec2){0, 0}, (ImVec2){1, 1}, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

bool rlImGuiImageButton(const char* name, const Texture* image)
{
    if (!image)
        return false;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    return igImageButton(name, (ImTextureID)image, (ImVec2){(float)image->width, (float)image->height}, (ImVec2){0, 0}, (ImVec2){1, 1}, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

bool rlImGuiImageButtonSize(const char* name, const Texture* image, Vector2 size)
{
    if (!image)
        return false;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    return igImageButton(name, (ImTextureID)image, (ImVec2){size.x, size.y}, (ImVec2){0, 0}, (ImVec2){1, 1}, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

void rlImGuiImageSize(const Texture* image, int width, int height)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    igImage((ImTextureID)image, (ImVec2){(float)width, (float)height}, (ImVec2){0, 0}, (ImVec2){1, 1}, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

void rlImGuiImageSizeV(const Texture* image, Vector2 size)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    igImage((ImTextureID)image, (ImVec2){size.x, size.y}, (ImVec2){0, 0}, (ImVec2){1, 1}, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0)
    {
        uv0.x = -((float)sourceRect.x / image->width);
        uv1.x = (uv0.x - (float)(fabs(sourceRect.width) / image->width));
    }
    else
    {
        uv0.x = (float)sourceRect.x / image->width;
        uv1.x = uv0.x + (float)(sourceRect.width / image->width);
    }

    if (sourceRect.height < 0)
    {
        uv0.y = -((float)sourceRect.y / image->height);
        uv1.y = (uv0.y - (float)(fabs(sourceRect.height) / image->height));
    }
    else
    {
        uv0.y = (float)sourceRect.y / image->height;
        uv1.y = uv0.y + (float)(sourceRect.height / image->height);
    }

    igImage((ImTextureID)image, (ImVec2){(float)destWidth, (float)destHeight}, uv0, uv1, (ImVec4){1, 1, 1, 1}, (ImVec4){0, 0, 0, 0});
}

void rlImGuiImageRenderTexture(const RenderTexture* image)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    rlImGuiImageRect(&image->texture, image->texture.width, image->texture.height, (Rectangle){ 0,0, (float)image->texture.width, -(float)image->texture.height });
}

void rlImGuiImageRenderTextureFit(const RenderTexture* image, bool center)
{
    if (!image)
        return;

    if (GlobalContext)
        igSetCurrentContext(GlobalContext);

    ImVec2 area;
    igGetContentRegionAvail(&area);

    float scale =  area.x / image->texture.width;

    float y = image->texture.height * scale;
    if (y > area.y)
    {
        scale = area.y / image->texture.height;
    }

    int sizeX = (int)(image->texture.width * scale);
    int sizeY = (int)(image->texture.height * scale);

    if (center)
    {
        igSetCursorPosX(0);
        igSetCursorPosX(area.x/2 - sizeX/2);
        igSetCursorPosY(igGetCursorPosY() + (area.y / 2 - sizeY / 2));
    }

    rlImGuiImageRect(&image->texture, sizeX, sizeY, (Rectangle){ 0,0, (float)(image->texture.width), -(float)(image->texture.height) });
}

// raw ImGui backend API
bool ImGui_ImplRaylib_Init(void)
{
    SetupGlobals();

    SetupMouseCursors();

    SetupBackend();

    return true;
}

void ImGui_ImplRaylib_BuildFontAtlas(void)
{
    ReloadFonts();
}

void ImGui_ImplRaylib_Shutdown()
{
    ImGuiIO* io =igGetIO();
    Texture2D* fontTexture = (Texture2D*)io->Fonts->TexID;

    if (fontTexture)
    {
        UnloadTexture(*fontTexture);
        MemFree(fontTexture);
    }

    io->Fonts->TexID = 0;
}

void ImGui_ImplRaylib_NewFrame(void)
{
    ImGuiNewFrame(GetFrameTime());
}

void ImGui_ImplRaylib_RenderDrawData(ImDrawData* draw_data)
{
    rlDrawRenderBatchActive();
    rlDisableBackfaceCulling();

    for (int l = 0; l < draw_data->CmdListsCount; ++l)
    {
        const ImDrawList* commandList = draw_data->CmdLists.Data[l];

        for (uint32_t idx = 0; idx < commandList->CmdBuffer.Size; ++idx)
        {
            ImDrawCmd* cmd = commandList->CmdBuffer.Data + idx;
            EnableScissor(cmd->ClipRect.x - draw_data->DisplayPos.x, cmd->ClipRect.y - draw_data->DisplayPos.y, cmd->ClipRect.z - (cmd->ClipRect.x - draw_data->DisplayPos.x), cmd->ClipRect.w - (cmd->ClipRect.y - draw_data->DisplayPos.y));
            if (cmd->UserCallback != nullptr)
            {
                cmd->UserCallback(commandList, cmd);

                continue;
            }

            ImGuiRenderTriangles(cmd->ElemCount, cmd->IdxOffset, commandList->IdxBuffer.Data, commandList->VtxBuffer.Data, (Texture2D*)cmd->TextureId);
            rlDrawRenderBatchActive();
        }
    }

    rlSetTexture(0);
    rlDisableScissorTest();
    rlEnableBackfaceCulling();
}

void HandleGamepadButtonEvent(ImGuiIO* io, GamepadButton button, ImGuiKey key)
{
    if (IsGamepadButtonPressed(0, button))
        ImGuiIO_AddKeyEvent(io, key, true);
    else if (IsGamepadButtonReleased(0, button))
        ImGuiIO_AddKeyEvent(io, key, false);
}

void HandleGamepadStickEvent(ImGuiIO* io, GamepadAxis axis, ImGuiKey negKey, ImGuiKey posKey)
{
    static const float deadZone = 0.20f;

    float axisValue = GetGamepadAxisMovement(0, axis);

    ImGuiIO_AddKeyAnalogEvent(io, negKey, axisValue < -deadZone, axisValue < -deadZone ? -axisValue : 0);
    ImGuiIO_AddKeyAnalogEvent(io, posKey, axisValue > deadZone, axisValue > deadZone ? axisValue : 0);
}

bool ImGui_ImplRaylib_ProcessEvents(void)
{
    ImGuiIO* io = igGetIO();

    bool focused = IsWindowFocused();
    if (focused != LastFrameFocused)
        ImGuiIO_AddFocusEvent(io, focused);
    LastFrameFocused = focused;

    // handle the modifyer key events so that shortcuts work
    bool ctrlDown = rlImGuiIsControlDown();
    if (ctrlDown != LastControlPressed)
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Ctrl, ctrlDown);
    LastControlPressed = ctrlDown;

    bool shiftDown = rlImGuiIsShiftDown();
    if (shiftDown != LastShiftPressed)
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Shift, shiftDown);
    LastShiftPressed = shiftDown;

    bool altDown = rlImGuiIsAltDown();
    if (altDown != LastAltPressed)
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Alt, altDown);
    LastAltPressed = altDown;

    bool superDown = rlImGuiIsSuperDown();
    if (superDown != LastSuperPressed)
        ImGuiIO_AddKeyEvent(io, ImGuiMod_Super, superDown);
    LastSuperPressed = superDown;

    // walk the keymap and check for up and down events
    enum { KeyCount = sizeof(RaylibKeyMap) / sizeof(RaylibKeyMap[0]) };
    for (uint32_t i = 0; i < KeyCount; ++i)
    {
        const KeyPairing key_pair = RaylibKeyMap[i];
        if (IsKeyReleased(key_pair.RaylibKey))
            ImGuiIO_AddKeyEvent(io, key_pair.ImGuiKey, false);
        else if(IsKeyPressed(key_pair.RaylibKey))
            ImGuiIO_AddKeyEvent(io, key_pair.ImGuiKey, true);
    }

    if (io->WantCaptureKeyboard)
    {
        // add the text input in order
        unsigned int pressed = GetCharPressed();
        while (pressed != 0)
        {
            ImGuiIO_AddInputCharacter(io, pressed);
            pressed = GetCharPressed();
        }
    }

    if (!io->WantSetMousePos)
    {
        ImGuiIO_AddMousePosEvent(io, (float)GetMouseX(), (float)GetMouseY());
    }

    #define setMouseEvent(rayMouse, imGuiMouse)                     \
        do                                                          \
        {                                                           \
            if (IsMouseButtonPressed(rayMouse))                     \
                ImGuiIO_AddMouseButtonEvent(io, imGuiMouse, true);  \
            else if (IsMouseButtonReleased(rayMouse))               \
                ImGuiIO_AddMouseButtonEvent(io, imGuiMouse, false); \
        } while (0)

    setMouseEvent(MOUSE_BUTTON_LEFT, ImGuiMouseButton_Left);
    setMouseEvent(MOUSE_BUTTON_RIGHT, ImGuiMouseButton_Right);
    setMouseEvent(MOUSE_BUTTON_MIDDLE, ImGuiMouseButton_Middle);
    setMouseEvent(MOUSE_BUTTON_FORWARD, ImGuiMouseButton_Middle + 1);
    setMouseEvent(MOUSE_BUTTON_BACK, ImGuiMouseButton_Middle + 2);

    {
        Vector2 mouseWheel = GetMouseWheelMoveV();
        ImGuiIO_AddMouseWheelEvent(io, mouseWheel.x, mouseWheel.y);
    }

    if (io->ConfigFlags & ImGuiConfigFlags_NavEnableGamepad && IsGamepadAvailable(0))
    {
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_UP, ImGuiKey_GamepadDpadUp);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_RIGHT, ImGuiKey_GamepadDpadRight);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_DOWN, ImGuiKey_GamepadDpadDown);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_FACE_LEFT, ImGuiKey_GamepadDpadLeft);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_UP, ImGuiKey_GamepadFaceUp);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT, ImGuiKey_GamepadFaceLeft);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_DOWN, ImGuiKey_GamepadFaceDown);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_FACE_LEFT, ImGuiKey_GamepadFaceRight);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_TRIGGER_1, ImGuiKey_GamepadL1);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_TRIGGER_2, ImGuiKey_GamepadL2);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_TRIGGER_1, ImGuiKey_GamepadR1);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_TRIGGER_2, ImGuiKey_GamepadR2);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_LEFT_THUMB, ImGuiKey_GamepadL3);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_RIGHT_THUMB, ImGuiKey_GamepadR3);

        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_MIDDLE_LEFT, ImGuiKey_GamepadStart);
        HandleGamepadButtonEvent(io, GAMEPAD_BUTTON_MIDDLE_RIGHT, ImGuiKey_GamepadBack);

        // left stick
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_LEFT_X, ImGuiKey_GamepadLStickLeft, ImGuiKey_GamepadLStickRight);
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_LEFT_Y, ImGuiKey_GamepadLStickUp, ImGuiKey_GamepadLStickDown);

        // right stick
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_RIGHT_X, ImGuiKey_GamepadRStickLeft, ImGuiKey_GamepadRStickRight);
        HandleGamepadStickEvent(io, GAMEPAD_AXIS_RIGHT_Y, ImGuiKey_GamepadRStickUp, ImGuiKey_GamepadRStickDown);
    }

    return true;
}