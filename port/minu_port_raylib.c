#include "minu_port.h"
#include "minu_disp.h"
#include "minu_conf.h"

#include <assert.h>
#include <stdint.h>
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

extern void minu_disp_set(void *disp);

static void port_beginDrawing(void);
static void port_endDrawing(void);

static void port_setWindow(int16_t x, int16_t y, uint16_t w, uint16_t h);
static void port_drawStr(int16_t x, int16_t y, const char *str);
static void port_setFontDatum(uint8_t datum);
static void port_setFont(void *font);
static uint16_t port_getStrWidth(const char *str);
static int8_t port_getFontHeight(void);
static void port_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon);
static void port_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
static void port_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h);
static void port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h);
static void port_drawPixel(int16_t x, int16_t y);
static void port_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

static minu_ops_t disp_ops = {
    .beginDrawing = port_beginDrawing,
    .endDrawing = port_endDrawing,
    .drawStr = port_drawStr,
    .setFont = port_setFont,
    .drawIcon = port_drawIcon,
    .setFontDatum = port_setFontDatum,
    .fillRect = port_fillRect,
    .drawRect = port_drawRect,
    .fillRectInDiff = port_fillRectInDiff,
    .getStrWidth = port_getStrWidth,
    .getFontHeight = port_getFontHeight,
    .setWindow = port_setWindow,
    .drawPixel = port_drawPixel,
    .drawLine = port_drawLine,
};

static RenderTexture2D frameBuf;
static Font g_font;
static int g_fontSize = 20;
static const char *fragCode =
    "#version 330\n"
    "in vec2 fragTexCoord;\n"
    "out vec4 finalColor;\n"
    "uniform sampler2D screenTexture;\n"
    "uniform vec2 areaPos;\n"
    "uniform vec2 areaSize;\n"
    "uniform float cornerRadius;\n"
    "uniform vec2 screenSize;\n"
    "void main() {\n"
    " vec2 screenCoord = vec2(fragTexCoord.x * screenSize.x,\n"
    " fragTexCoord.y * screenSize.y);\n"
    " vec4 screenColor = texture(screenTexture, fragTexCoord);\n"
    " if (screenCoord.x >= areaPos.x && screenCoord.x <= areaPos.x + areaSize.x\n"
    "&& screenCoord.y >= areaPos.y && screenCoord.y <= areaPos.y + areaSize.y){\n"
    " vec2 localPos = screenCoord - areaPos;\n"
    " bool inRoundedRect = true;\n"
    " float radius = cornerRadius;\n"
    " if (localPos.x < radius && localPos.y < radius) {\n"
    " float dist = length(localPos - vec2(radius));\n"
    " if (dist > radius) inRoundedRect = false;\n"
    " } else if (localPos.x > areaSize.x - radius && localPos.y < radius) {\n"
    " float dist = length(localPos - vec2(areaSize.x - radius, radius));\n"
    " if (dist > radius) inRoundedRect = false;\n"
    " } else if (localPos.x < radius && localPos.y > areaSize.y - radius) {\n"
    " float dist = length(localPos - vec2(radius, areaSize.y - radius));\n"
    " if (dist > radius) inRoundedRect = false;\n"
    " } else if (localPos.x > areaSize.x - radius && localPos.y > areaSize.y - radius){\n"
    " float dist = length(localPos - vec2(areaSize.x - radius, areaSize.y - radius));\n"
    " if (dist > radius) inRoundedRect = false;\n"
    " }\n"
    " if (inRoundedRect) {\n"
    " finalColor = vec4(1.0 - screenColor.rgb, screenColor.a);\n"
    " return;\n"
    " }\n"
    " }\n"
    " finalColor = screenColor;\n"
    "}";

/* Note: call this function after the u8g2 has been initialized! */
void minu_port_new_disp_raylib(int32_t hor_res, int32_t ver_res)
{
    InitWindow(hor_res, ver_res, "minu");
    SetTargetFPS(60);
    g_font = GetFontDefault();
    /* g_fontSize = g_font.baseSize; */
    printf("font_size: %d", g_fontSize);
    frameBuf = LoadRenderTexture(hor_res, ver_res);

    minu_disp_set(&disp_ops);
}

static void port_drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    DrawLine(x1, y1, x2, y2, WHITE);
}

static void port_drawPixel(int16_t x, int16_t y)
{
    DrawPixel(x, y, WHITE);
}

static void port_setWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    BeginScissorMode(x, y, w, h);
}

static void port_fillRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    DrawRectangle(x, y, w, h, WHITE);
}

static void port_drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
    DrawRectangleLines(x, y, w, h, WHITE);
}

static void port_fillRectInDiff(int16_t x, int16_t y, int16_t w, int16_t h)
{
    static Shader roundedInvertShader = {0};
    if (roundedInvertShader.id == 0)
        roundedInvertShader = LoadShaderFromMemory(NULL, fragCode);

    /* to opengl coord */
    float areaPos[2] = {x - 2, frameBuf.texture.height - y - h};
    float areaSize[2] = {w + 4, h};
    float screenSize[2] = {(float)frameBuf.texture.width, (float)frameBuf.texture.height};
    SetShaderValue(roundedInvertShader,
                   GetShaderLocation(roundedInvertShader, "areaPos"),
                   areaPos,
                   SHADER_UNIFORM_VEC2);
    SetShaderValue(roundedInvertShader,
                   GetShaderLocation(roundedInvertShader, "areaSize"),
                   areaSize,
                   SHADER_UNIFORM_VEC2);
    float radius = 10;
    float maxR = 0.5f * (w < h ? w : h);
    if (radius > maxR)
        radius = maxR;
    SetShaderValue(roundedInvertShader,
                   GetShaderLocation(roundedInvertShader, "cornerRadius"),
                   &radius,
                   SHADER_UNIFORM_FLOAT);
    SetShaderValue(roundedInvertShader,
                   GetShaderLocation(roundedInvertShader, "screenSize"),
                   screenSize,
                   SHADER_UNIFORM_VEC2);

    BeginShaderMode(roundedInvertShader);
    DrawTextureRec(
        frameBuf.texture,
        (Rectangle){0, 0, (float)frameBuf.texture.width, (float)-frameBuf.texture.height},
        (Vector2){0, 0},
        WHITE);
    EndShaderMode();
}

static void port_drawIcon(int16_t x, int16_t y, uint16_t w, uint16_t h, void *icon)
{
    const uint8_t *bits = (const uint8_t *)icon;

    for (uint16_t iy = 0; iy < h; iy++)
    {
        for (uint16_t ix = 0; ix < w; ix++)
        {
            uint16_t byteIndex = (iy * ((w + 7) / 8)) + (ix >> 3);
            uint8_t bitMask = 1 << (ix & 7);

            if (bits[byteIndex] & bitMask)
            {
                DrawPixel(x + ix, y + iy, WHITE);
            }
        }
    }
}

static int8_t port_getFontHeight(void)
{
    return (int8_t)g_fontSize;
}

static uint16_t port_getStrWidth(const char *str)
{
    Vector2 size = MeasureTextEx(g_font, str, g_fontSize, 1);
    return (uint16_t)size.x;
}

static void port_setFont(void *font)
{
    (void)font;
}

// Set text datum position, @ref enum minu_font_datum_t
static void port_setFontDatum(uint8_t datum)
{
    (void)datum;
}

static void port_drawStr(int16_t x, int16_t y, const char *str)
{
    DrawTextEx(g_font, str, (Vector2){x, y}, g_fontSize, 1, WHITE);
}

static void port_beginDrawing(void)
{
    BeginDrawing();
    BeginTextureMode(frameBuf);
    ClearBackground(BLACK);
}

static void port_endDrawing(void)
{
    EndTextureMode();
    DrawTextureRec(
        frameBuf.texture,
        (Rectangle){0, 0, (float)frameBuf.texture.width, (float)-frameBuf.texture.height},
        (Vector2){0, 0},
        WHITE);
    EndDrawing();
}
