/**
 * @file Bridgetek_EVE2.h
 * @brief Support FT810/FT811/FT812/FT813
 */
/*
 * ============================================================================
 * (C) Copyright,  Bridgetek Pte. Ltd.
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * https://brtchip.com/wp-content/uploads/2021/11/BRT_Software_License_Agreement.pdf ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

#ifndef _BTEVE_H_
#define _BTEVE_H_

#include <stdint.h> // for Uint8/16/32 and Int8/16/32 data types
#include <stdarg.h>


/* Clash with Arduino API */
#undef DISPLAY

#define EVE_API 2
#define EVE_SUB_API 1

//@{
/** @brief Library Includes
 * It will be possible to make a library with these files.
 * Include EVE.h (which is the API as a C++ file which is part of this class.
 * Include HAL and MCU files as "C" files.
 */
//@{
#ifdef __cplusplus
extern "C" {
#endif
#include <EVE2.h>
#include <HAL.h>
#include <MCU.h>
#ifdef __cplusplus
}
#endif
//@}

class Bridgetek_EVE2
{
  public:
    Bridgetek_EVE2(void);

  void setup(uint32_t panel);

  public:
    int LIB_AwaitCoProEmpty(void) { return ::EVE_LIB_AwaitCoProEmpty(); };
    uint16_t LIB_MemRead16(uint32_t address) { return ::EVE_LIB_MemRead16(address); };
    uint16_t LIB_SendString(const char* string) { return ::EVE_LIB_SendString(string); };
    uint32_t LIB_GetResult(int offset) { return ::EVE_LIB_GetResult(offset); };
    uint32_t LIB_MemRead32(uint32_t address) { return ::EVE_LIB_MemRead32(address); };
    uint8_t LIB_MemRead8(uint32_t address) { return ::EVE_LIB_MemRead8(address); };
    void LIB_BeginCoProList(void) { ::EVE_LIB_BeginCoProList(); };
    void LIB_EndCoProList(void) { ::EVE_LIB_EndCoProList(); };
    void LIB_GetMatrix(uint32_t *a,  uint32_t *b,  uint32_t *c,  uint32_t *d,  uint32_t *e,  uint32_t *f) { ::EVE_LIB_GetMatrix(a, b, c, d, e, f); };
    void LIB_GetProps(uint32_t *addr,  uint32_t *width,  uint32_t *height) { ::EVE_LIB_GetProps(addr, width, height); };
    void LIB_GetPtr(uint32_t *addr) { ::EVE_LIB_GetPtr(addr); };
    void LIB_MemCrc(uint32_t ptr,  uint32_t num,  uint32_t *result) { ::EVE_LIB_MemCrc(ptr, num, result); };
    void LIB_MemWrite16(uint32_t addr,  uint16_t value) { ::EVE_LIB_MemWrite16(addr, value); };
    void LIB_MemWrite32(uint32_t addr,  uint32_t value) { ::EVE_LIB_MemWrite32(addr, value); };
    void LIB_MemWrite8(uint32_t addr,  uint8_t value) { ::EVE_LIB_MemWrite8(addr, value); };
    void LIB_ReadDataFromRAMG(uint8_t *ImgData,  uint32_t DataSize,  uint32_t SrcAddress) { ::EVE_LIB_ReadDataFromRAMG(ImgData, DataSize, SrcAddress); };
    void LIB_WriteDataToCMD(const uint8_t *ImgData,  uint32_t DataSize) { ::EVE_LIB_WriteDataToCMD(ImgData, DataSize); };
    void LIB_WriteDataToRAMG(const uint8_t *ImgData,  uint32_t DataSize,  uint32_t DestAddress) { ::EVE_LIB_WriteDataToRAMG(ImgData, DataSize, DestAddress); };

    void CMD_APPEND(uint32_t ptr,  uint32_t num) { ::EVE_CMD_APPEND(ptr, num); };
    void CMD_BGCOLOR(uint32_t c) { ::EVE_CMD_BGCOLOR(c); };
    void CMD_BITMAP_TRANSFORM(int32_t x0,  int32_t y0,  int32_t x1,  int32_t y1,  int32_t x2,  int32_t y2,  int32_t tx0,  int32_t ty0,  int32_t tx1,  int32_t ty1,  int32_t tx2,  int32_t ty2,  uint16_t result) { ::EVE_CMD_BITMAP_TRANSFORM(x0, y0, x1, y1, x2, y2, tx0, ty0, tx1, ty1, tx2, ty2, result); };
    void CMD_BUTTON(int16_t x,  int16_t y,  int16_t w,  int16_t h,  int16_t font,  uint16_t options,  const char* string, ...) { va_list myargs; va_start(myargs, string); ::EVE_CMD_BUTTON(x, y, w, h, font, options, string, myargs); va_end(myargs); };
    void CMD_CALIBRATE(uint32_t result) { ::EVE_CMD_CALIBRATE(result); };
    void CMD_CLOCK(int16_t x,  int16_t y,  int16_t r,  uint16_t options,  uint16_t h,  uint16_t m,  uint16_t s,  uint16_t ms) { ::EVE_CMD_CLOCK(x, y, r, options, h, m, s, ms); };
    void CMD_COLDSTART(void) { ::EVE_CMD_COLDSTART(); };
    void CMD_CSKETCH(int16_t x,  int16_t y,  uint16_t w,  uint16_t h,  uint32_t ptr,  uint16_t format,  uint16_t freq) { ::EVE_CMD_CSKETCH(x, y, w, h, ptr, format, freq); };
    void CMD_DIAL(int16_t x,  int16_t y,  int16_t r,  uint16_t options,  uint16_t val) { ::EVE_CMD_DIAL(x, y, r, options, val); };
    void CMD_DLSTART(void) { ::EVE_CMD_DLSTART(); };
    void CMD_FGCOLOR(uint32_t c) { ::EVE_CMD_FGCOLOR(c); };
    void CMD_GAUGE(int16_t x,  int16_t y,  int16_t r,  uint16_t options,  uint16_t major,  uint16_t minor,  uint16_t val,  uint16_t range) { ::EVE_CMD_GAUGE(x, y, r, options, major, minor, val, range); };
    void CMD_GETMATRIX(int32_t a,  int32_t b,  int32_t c,  int32_t d,  int32_t e,  int32_t f) { ::EVE_CMD_GETMATRIX(a, b, c, d, e, f); };
    void CMD_GETPROPS(uint32_t ptr,  uint32_t w,  uint32_t h) { ::EVE_CMD_GETPROPS(ptr, w, h); };
    void CMD_GETPTR(uint32_t result) { ::EVE_CMD_GETPTR(result); };
    void CMD_GRADCOLOR(uint32_t c) { ::EVE_CMD_GRADCOLOR(c); };
    void CMD_GRADIENT(int16_t x0,  int16_t y0,  uint32_t rgb0,  int16_t x1,  int16_t y1,  uint32_t rgb1) { ::EVE_CMD_GRADIENT(x0, y0, rgb0, x1, y1, rgb1); };
    void CMD_INFLATE(uint32_t ptr) { ::EVE_CMD_INFLATE(ptr); };
    void CMD_INTERRUPT(uint32_t ms) { ::EVE_CMD_INTERRUPT(ms); };
    void CMD_KEYS(int16_t x,  int16_t y,  int16_t w,  int16_t h,  int16_t font,  uint16_t options,  const char* string) { ::EVE_CMD_KEYS(x, y, w, h, font, options, string); };
    void CMD_LOADIDENTITY(void) { ::EVE_CMD_LOADIDENTITY(); };
    void CMD_LOADIMAGE(uint32_t ptr,  uint32_t options) { ::EVE_CMD_LOADIMAGE(ptr, options); };
    void CMD_LOGO(void) { ::EVE_CMD_LOGO(); };
    void CMD_MEDIAFIFO(uint32_t ptr,  uint32_t size) { ::EVE_CMD_MEDIAFIFO(ptr, size); };
    void CMD_MEMCPY(uint32_t dest,  uint32_t src,  uint32_t num) { ::EVE_CMD_MEMCPY(dest, src, num); };
    void CMD_MEMCRC(uint32_t ptr,  uint32_t num,  uint32_t result) { ::EVE_CMD_MEMCRC(ptr, num, result); };
    void CMD_MEMSET(uint32_t ptr,  uint32_t value,  uint32_t num) { ::EVE_CMD_MEMSET(ptr, value, num); };
    void CMD_MEMWRITE(uint32_t ptr,  uint32_t num) { ::EVE_CMD_MEMWRITE(ptr, num); };
    void CMD_MEMZERO(uint32_t ptr,  uint32_t num) { ::EVE_CMD_MEMZERO(ptr, num); };
    void CMD_NUMBER(int16_t x,  int16_t y,  int16_t font,  uint16_t options,  int32_t n) { ::EVE_CMD_NUMBER(x, y, font, options, n); };
    void CMD_PLAYVIDEO(uint32_t options) { ::EVE_CMD_PLAYVIDEO(options); };
    void CMD_PROGRESS(int16_t x,  int16_t y,  int16_t w,  int16_t h,  uint16_t options,  uint16_t val,  uint16_t range) { ::EVE_CMD_PROGRESS(x, y, w, h, options, val, range); };
    void CMD_REGREAD(uint32_t ptr,  uint32_t result) { ::EVE_CMD_REGREAD(ptr, result); };
    void CMD_ROMFONT(uint32_t font,  uint32_t romslot) { ::EVE_CMD_ROMFONT(font, romslot); };
    void CMD_ROTATE(int32_t a) { ::EVE_CMD_ROTATE(a); };
    void CMD_SCALE(int32_t sx,  int32_t sy) { ::EVE_CMD_SCALE(sx, sy); };
    void CMD_SCREENSAVER(void) { ::EVE_CMD_SCREENSAVER(); };
    void CMD_SCROLLBAR(int16_t x,  int16_t y,  int16_t w,  int16_t h,  uint16_t options,  uint16_t val,  uint16_t size,  uint16_t range) { ::EVE_CMD_SCROLLBAR(x, y, w, h, options, val, size, range); };
    void CMD_SETBASE(uint32_t base) { ::EVE_CMD_SETBASE(base); };
    void CMD_SETBITMAP(uint32_t source,  uint16_t fmt,  uint16_t w,  uint16_t h) { ::EVE_CMD_SETBITMAP(source, fmt, w, h); };
    void CMD_SETFONT(uint32_t font,  uint32_t ptr) { ::EVE_CMD_SETFONT(font, ptr); };
    void CMD_SETFONT2(uint32_t font,  uint32_t ptr,  uint32_t firstchar) { ::EVE_CMD_SETFONT2(font, ptr, firstchar); };
    void CMD_SETMATRIX(void) { ::EVE_CMD_SETMATRIX(); };
    void CMD_SETROTATE(uint32_t r) { ::EVE_CMD_SETROTATE(r); };
    void CMD_SETSCRATCH(uint32_t handle) { ::EVE_CMD_SETSCRATCH(handle); };
    void CMD_SKETCH(int16_t x,  int16_t y,  uint16_t w,  uint16_t h,  uint32_t ptr,  uint16_t format) { ::EVE_CMD_SKETCH(x, y, w, h, ptr, format); };
    void CMD_SLIDER(int16_t x,  int16_t y,  int16_t w,  int16_t h,  uint16_t options,  uint16_t val,  uint16_t range) { ::EVE_CMD_SLIDER(x, y, w, h, options, val, range); };
    void CMD_SNAPSHOT(uint32_t ptr) { ::EVE_CMD_SNAPSHOT(ptr); };
    void CMD_SNAPSHOT2(uint32_t fmt,  uint32_t ptr,  int16_t x,  int16_t y,  int16_t w,  int16_t h) { ::EVE_CMD_SNAPSHOT2(fmt, ptr, x, y, w, h); };
    void CMD_SPINNER(int16_t x,  int16_t y,  uint16_t style,  uint16_t scale) { ::EVE_CMD_SPINNER(x, y, style, scale); };
    void CMD_STOP(void) { ::EVE_CMD_STOP(); };
    void CMD_SWAP(void) { ::EVE_CMD_SWAP(); };
    void CMD_SYNC(void) { ::EVE_CMD_SYNC(); };
    void CMD_TEXT(int16_t x,  int16_t y,  int16_t font,  uint16_t options,  const char* string, ...) { va_list myargs; va_start(myargs, string); ::EVE_CMD_TEXT(x, y, font, options, string, myargs); va_end(myargs); };
    void CMD_TOGGLE(int16_t x,  int16_t y,  int16_t w,  int16_t font,  uint16_t options,  uint16_t state,  const char* string, ...) { va_list myargs; va_start(myargs, string); ::EVE_CMD_TOGGLE(x, y, w, font, options, state, string, myargs); va_end(myargs); };
    void CMD_TRACK(int16_t x,  int16_t y,  int16_t w,  int16_t h,  int16_t tag) { ::EVE_CMD_TRACK(x, y, w, h, tag); };
    void CMD_TRANSLATE(int32_t tx,  int32_t ty) { ::EVE_CMD_TRANSLATE(tx, ty); };
    void CMD_VIDEOFRAME(uint32_t dst,  uint32_t ptr) { ::EVE_CMD_VIDEOFRAME(dst, ptr); };
    void CMD_VIDEOSTART(void) { ::EVE_CMD_VIDEOSTART(); };

    void ALPHA_FUNC(uint8_t func,  uint8_t ref) { ::EVE_ALPHA_FUNC(func, ref); };
    void BEGIN(uint8_t prim) { ::EVE_BEGIN(prim); };
    void BITMAP_HANDLE(uint8_t handle) { ::EVE_BITMAP_HANDLE(handle); };
    void BITMAP_LAYOUT(uint8_t format,  uint16_t linestride,  uint16_t height) { ::EVE_BITMAP_LAYOUT(format, linestride, height); };
    void BITMAP_LAYOUT_H(uint8_t linestride,  uint8_t height) { ::EVE_BITMAP_LAYOUT_H(linestride, height); };
    void BITMAP_SIZE(uint8_t filter,  uint8_t wrapx,  uint8_t wrapy,  uint16_t width,  uint16_t height) { ::EVE_BITMAP_SIZE(filter, wrapx, wrapy, width, height); };
    void BITMAP_SIZE_H(uint8_t width,  uint8_t height) { ::EVE_BITMAP_SIZE_H(width, height); };
    void BITMAP_SOURCE(int32_t addr) { ::EVE_BITMAP_SOURCE(addr); };
    void BITMAP_TRANSFORM_A(long a) { ::EVE_BITMAP_TRANSFORM_A(a); };
    void BITMAP_TRANSFORM_B(long b) { ::EVE_BITMAP_TRANSFORM_B(b); };
    void BITMAP_TRANSFORM_C(long c) { ::EVE_BITMAP_TRANSFORM_C(c); };
    void BITMAP_TRANSFORM_D(long d) { ::EVE_BITMAP_TRANSFORM_D(d); };
    void BITMAP_TRANSFORM_E(long e) { ::EVE_BITMAP_TRANSFORM_E(e); };
    void BITMAP_TRANSFORM_F(long f) { ::EVE_BITMAP_TRANSFORM_F(f); };
    void BLEND_FUNC(uint8_t src,  uint8_t dst) { ::EVE_BLEND_FUNC(src, dst); };
    void CALL(uint16_t dest) { ::EVE_CALL(dest); };
    void CELL(uint8_t cell) { ::EVE_CELL(cell); };
    void CLEAR(uint8_t c,  uint8_t s,  uint8_t t) { ::EVE_CLEAR(c, s, t); };
    void CLEAR_COLOR(uint32_t c) { ::EVE_CLEAR_COLOR(c); };
    void CLEAR_COLOR_A(uint8_t alpha) { ::EVE_CLEAR_COLOR_A(alpha); };
    void CLEAR_COLOR_RGB(uint8_t r,  uint8_t g,  uint8_t b) { ::EVE_CLEAR_COLOR_RGB(r, g, b); };
    void CLEAR_STENCIL(uint8_t s) { ::EVE_CLEAR_STENCIL(s); };
    void CLEAR_TAG(uint8_t s) { ::EVE_CLEAR_TAG(s); };
    void CMD(uint32_t c) { ::EVE_CMD(c); };
    void COLOR(uint32_t c) { ::EVE_COLOR(c); };
    void COLOR_A(uint8_t alpha) { ::EVE_COLOR_A(alpha); };
    void COLOR_MASK(uint8_t r,  uint8_t g,  uint8_t b,  uint8_t a) { ::EVE_COLOR_MASK(r, g, b, a); };
    void COLOR_RGB(uint8_t r,  uint8_t g,  uint8_t b) { ::EVE_COLOR_RGB(r, g, b); };
    void DISPLAY(void) { ::EVE_DISPLAY(); };
    void END(void) { ::EVE_END(); };
    void Init(void) { ::EVE_Init(); };
    void JUMP(uint16_t dest) { ::EVE_JUMP(dest); };
    void LINE_WIDTH(uint16_t width) { ::EVE_LINE_WIDTH(width); };
    void MACRO(uint8_t m) { ::EVE_MACRO(m); };
    void PALETTE_SOURCE(uint32_t addr) { ::EVE_PALETTE_SOURCE(addr); };
    void POINT_SIZE(uint16_t size) { ::EVE_POINT_SIZE(size); };
    void RESTORE_CONTEXT(void) { ::EVE_RESTORE_CONTEXT(); };
    void RETURN(void) { ::EVE_RETURN(); };
    void SAVE_CONTEXT(void) { ::EVE_SAVE_CONTEXT(); };
    void SCISSOR_SIZE(uint16_t width,  uint16_t height) { ::EVE_SCISSOR_SIZE(width, height); };
    void SCISSOR_XY(uint16_t x,  uint16_t y) { ::EVE_SCISSOR_XY(x, y); };
    void STENCIL_FUNC(uint8_t func,  uint8_t ref,  uint8_t mask) { ::EVE_STENCIL_FUNC(func, ref, mask); };
    void STENCIL_MASK(uint8_t mask) { ::EVE_STENCIL_MASK(mask); };
    void STENCIL_OP(uint8_t sfail,  uint8_t spass) { ::EVE_STENCIL_OP(sfail, spass); };
    void TAG(uint8_t s) { ::EVE_TAG(s); };
    void TAG_MASK(uint8_t mask) { ::EVE_TAG_MASK(mask); };
    void VERTEX2F(int16_t x,  int16_t y) { ::EVE_VERTEX2F(x, y); };
    void VERTEX2II(uint16_t x,  uint16_t y,  uint8_t handle, uint8_t cell) { ::EVE_VERTEX2II(x, y, handle, cell); };
    void VERTEX_FORMAT(uint8_t frac) { ::EVE_VERTEX_FORMAT(frac); };
    void VERTEX_TRANSLATE_X(uint32_t x) { ::EVE_VERTEX_TRANSLATE_X(x); };
    void VERTEX_TRANSLATE_Y(uint32_t y) { ::EVE_VERTEX_TRANSLATE_Y(y); };

    const uint8_t ROMFONT_FORMATS[35] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L1,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4,(uint8_t)FORMAT_L4};
    const uint8_t ROMFONT_HEIGHTS[35] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,16,16,13,17,20,22,29,38,16,20,25,28,36,49,63,83,108};
    const uint8_t ROMFONT_WIDTHS[35] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,8,8,11,13,17,18,25,34,13,15,19,21,28,37,49,63,82};

    enum {
      ADC_DIFFERENTIAL = 0x1ul,
      ADC_SINGLE_ENDED = 0x0ul,
      ADPCM_SAMPLES = 0x2ul,
      BEGIN_BITMAPS = 0x1ul,
      BEGIN_EDGE_STRIP_A = 0x7ul,
      BEGIN_EDGE_STRIP_B = 0x8ul,
      BEGIN_EDGE_STRIP_L = 0x6ul,
      BEGIN_EDGE_STRIP_R = 0x5ul,
      BEGIN_LINES = 0x3ul,
      BEGIN_LINE_STRIP = 0x4ul,
      BEGIN_POINTS = 0x2ul,
      BEGIN_RECTS = 0x9ul,
      BLEND_DST_ALPHA = 0x3ul,
      BLEND_ONE = 0x1ul,
      BLEND_ONE_MINUS_DST_ALPHA = 0x5ul,
      BLEND_ONE_MINUS_SRC_ALPHA = 0x4ul,
      BLEND_SRC_ALPHA = 0x2ul,
      BLEND_ZERO = 0x0ul,
      DLSWAP_DONE = 0x0ul,
      DLSWAP_FRAME = 0x2ul,
      DLSWAP_LINE = 0x1ul,
      FILTER_BILINEAR = 0x1ul,
      FILTER_NEAREST = 0x0ul,
      FORMAT_ARGB1555 = 0x0ul,
      FORMAT_ARGB2 = 0x5ul,
      FORMAT_ARGB4 = 0x6ul,
      FORMAT_BARGRAPH = 0xbul,
      FORMAT_L1 = 0x1ul,
      FORMAT_L4 = 0x2ul,
      FORMAT_L8 = 0x3ul,
      FORMAT_PALETTED = 0x8ul,
      FORMAT_PALETTED4444 = 0xful,
      FORMAT_PALETTED565 = 0xeul,
      FORMAT_PALETTED8 = 0x10ul,
      FORMAT_RGB332 = 0x4ul,
      FORMAT_RGB565 = 0x7ul,
      FORMAT_TEXT8X8 = 0x9ul,
      FORMAT_TEXTVGA = 0xaul,
      INT_CMDEMPTY = 0x20ul,
      INT_CMDFLAG = 0x40ul,
      INT_CONVCOMPLETE = 0x80ul,
      INT_G8 = 0x12ul,
      INT_L8C = 0xcul,
      INT_PLAYBACK = 0x10ul,
      INT_SOUND = 0x8ul,
      INT_SWAP = 0x1ul,
      INT_TAG = 0x4ul,
      INT_TOUCH = 0x2ul,
      INT_VGA = 0xdul,
      LINEAR_SAMPLES = 0x0ul,
      LOW_FREQ_BOUND = 58800000L,
      OPT_3D = 0x0ul,
      OPT_CENTER = 0x600ul,
      OPT_CENTERX = 0x200ul,
      OPT_CENTERY = 0x400ul,
      OPT_FLAT = 0x100ul,
      OPT_FULLSCREEN = 0x8ul,
      OPT_MEDIAFIFO = 0x10ul,
      OPT_MONO = 0x1ul,
      OPT_NOBACK = 0x1000ul,
      OPT_NODL = 0x2ul,
      OPT_NOHANDS = 0xc000ul,
      OPT_NOHM = 0x4000ul,
      OPT_NOPOINTER = 0x4000ul,
      OPT_NOSECS = 0x8000ul,
      OPT_NOTEAR = 0x4ul,
      OPT_NOTICKS = 0x2000ul,
      OPT_RGB565 = 0x0ul,
      OPT_RIGHTX = 0x800ul,
      OPT_SIGNED = 0x100ul,
      OPT_SOUND = 0x20ul,
      RAM_CMD = 0x308000ul,
      RAM_CMD_SIZE = (4*1024L),
      RAM_DL = 0x300000ul,
      RAM_DL_SIZE = (8*1024L),
      RAM_G = 0x0ul,
      RAM_REG = 0x302000ul,
      RAM_ROMSUB = 0x30a000ul,
      ROMFONT_MAX = 34,
      ROMFONT_TABLEADDRESS = 0x2ffffcul,
      ROM_CHIPID = 0x0c0000ul,
      STENCIL_DECR = 0x4ul,
      STENCIL_INCR = 0x3ul,
      STENCIL_INVERT = 0x5ul,
      STENCIL_KEEP = 0x1ul,
      STENCIL_REPLACE = 0x2ul,
      STENCIL_ZERO = 0x0ul,
      SUB_API = 1,
      TEST_ALWAYS = 0x7ul,
      TEST_EQUAL = 0x5ul,
      TEST_GEQUAL = 0x4ul,
      TEST_GREATER = 0x3ul,
      TEST_LEQUAL = 0x2ul,
      TEST_LESS = 0x1ul,
      TEST_NEVER = 0x0ul,
      TEST_NOTEQUAL = 0x6ul,
      TOUCHMODE_CONTINUOUS = 0x3ul,
      TOUCHMODE_FRAME = 0x2ul,
      TOUCHMODE_OFF = 0x0ul,
      TOUCHMODE_ONESHOT = 0x1ul,
      ULAW_SAMPLES = 0x1ul,
      VOL_ZERO = 0x0ul,
      WRAP_BORDER = 0x0ul,
      WRAP_REPEAT = 0x1ul,

      API = 2,

      REG_BIST_EN = 0x302174ul,
      REG_BUSYBITS = 0x3020e8ul,
      REG_CLOCK = 0x302008ul,
      REG_CMDB_SPACE = 0x302574ul,
      REG_CMDB_WRITE = 0x302578ul,
      REG_CMD_DL = 0x302100ul,
      REG_CMD_READ = 0x3020f8ul,
      REG_CMD_WRITE = 0x3020fcul,
      REG_CPURESET = 0x302020ul,
      REG_CSPREAD = 0x302068ul,
      REG_CTOUCH_EXTENDED = 0x302108ul,
      REG_CTOUCH_TOUCH0_XY = 0x302124ul,
      REG_CTOUCH_TOUCH1_XY = 0x30211cul,
      REG_CTOUCH_TOUCH2_XY = 0x30218cul,
      REG_CTOUCH_TOUCH3_XY = 0x302190ul,
      REG_CTOUCH_TOUCH4_X = 0x30216cul,
      REG_CTOUCH_TOUCH4_Y = 0x302120ul,
      REG_DITHER = 0x302060ul,
      REG_DLSWAP = 0x302054ul,
      REG_FRAMES = 0x302004ul,
      REG_FREQUENCY = 0x30200cul,
      REG_GPIO = 0x302094ul,
      REG_GPIOX = 0x30209cul,
      REG_GPIOX_DIR = 0x302098ul,
      REG_GPIO_DIR = 0x302090ul,
      REG_HCYCLE = 0x30202cul,
      REG_HOFFSET = 0x302030ul,
      REG_HSIZE = 0x302034ul,
      REG_HSYNC0 = 0x302038ul,
      REG_HSYNC1 = 0x30203cul,
      REG_ID = 0x302000ul,
      REG_INT_EN = 0x3020acul,
      REG_INT_FLAGS = 0x3020a8ul,
      REG_INT_MASK = 0x3020b0ul,
      REG_MACRO_0 = 0x3020d8ul,
      REG_MACRO_1 = 0x3020dcul,
      REG_MEDIAFIFO_READ = 0x309014ul,
      REG_MEDIAFIFO_WRITE = 0x309018ul,
      REG_OUTBITS = 0x30205cul,
      REG_PCLK = 0x302070ul,
      REG_PCLK_POL = 0x30206cul,
      REG_PLAY = 0x30208cul,
      REG_PLAYBACK_FORMAT = 0x3020c4ul,
      REG_PLAYBACK_FREQ = 0x3020c0ul,
      REG_PLAYBACK_LENGTH = 0x3020b8ul,
      REG_PLAYBACK_LOOP = 0x3020c8ul,
      REG_PLAYBACK_PLAY = 0x3020ccul,
      REG_PLAYBACK_READPTR = 0x3020bcul,
      REG_PLAYBACK_START = 0x3020b4ul,
      REG_PWM_DUTY = 0x3020d4ul,
      REG_PWM_HZ = 0x3020d0ul,
      REG_RENDERMODE = 0x302010ul,
      REG_ROMSUB_SEL = 0x3020f0ul,
      REG_ROTATE = 0x302058ul,
      REG_SNAPFORMAT = 0x30201cul,
      REG_SNAPSHOT = 0x302018ul,
      REG_SNAPY = 0x302014ul,
      REG_SOUND = 0x302088ul,
      REG_SPI_EARLY_TX = 0x30217cul,
      REG_SPI_WIDTH = 0x302188ul,
      REG_SWIZZLE = 0x302064ul,
      REG_TAG = 0x30207cul,
      REG_TAG_X = 0x302074ul,
      REG_TAG_Y = 0x302078ul,
      REG_TAP_CRC = 0x302024ul,
      REG_TAP_MASK = 0x302028ul,
      REG_TOUCH_ADC_MODE = 0x302108ul,
      REG_TOUCH_CHARGE = 0x30210cul,
      REG_TOUCH_CONFIG = 0x302168ul,
      REG_TOUCH_DIRECT_XY = 0x30218cul,
      REG_TOUCH_DIRECT_Z1Z2 = 0x302190ul,
      REG_TOUCH_FAULT = 0x302170ul,
      REG_TOUCH_MODE = 0x302104ul,
      REG_TOUCH_OVERSAMPLE = 0x302114ul,
      REG_TOUCH_RAW_XY = 0x30211cul,
      REG_TOUCH_RZ = 0x302120ul,
      REG_TOUCH_RZTHRESH = 0x302118ul,
      REG_TOUCH_SCREEN_XY = 0x302124ul,
      REG_TOUCH_SETTLE = 0x302110ul,
      REG_TOUCH_TAG = 0x30212cul,
      REG_TOUCH_TAG1 = 0x302134ul,
      REG_TOUCH_TAG1_XY = 0x302130ul,
      REG_TOUCH_TAG2 = 0x30213cul,
      REG_TOUCH_TAG2_XY = 0x302138ul,
      REG_TOUCH_TAG3 = 0x302144ul,
      REG_TOUCH_TAG3_XY = 0x302140ul,
      REG_TOUCH_TAG4 = 0x30214cul,
      REG_TOUCH_TAG4_XY = 0x302148ul,
      REG_TOUCH_TAG_XY = 0x302128ul,
      REG_TOUCH_TRANSFORM_A = 0x302150ul,
      REG_TOUCH_TRANSFORM_B = 0x302154ul,
      REG_TOUCH_TRANSFORM_C = 0x302158ul,
      REG_TOUCH_TRANSFORM_D = 0x30215cul,
      REG_TOUCH_TRANSFORM_E = 0x302160ul,
      REG_TOUCH_TRANSFORM_F = 0x302164ul,
      REG_TRACKER = 0x309000ul,
      REG_TRACKER_1 = 0x309004ul,
      REG_TRACKER_2 = 0x309008ul,
      REG_TRACKER_3 = 0x30900cul,
      REG_TRACKER_4 = 0x309010ul,
      REG_TRIM = 0x302180ul,
      REG_VCYCLE = 0x302040ul,
      REG_VOFFSET = 0x302044ul,
      REG_VOL_PB = 0x302080ul,
      REG_VOL_SOUND = 0x302084ul,
      REG_VSIZE = 0x302048ul,
      REG_VSYNC0 = 0x30204cul,
      REG_VSYNC1 = 0x302050ul,
    };

  public:
    uint16_t DISP_WIDTH(void);
    uint16_t DISP_HEIGHT(void);

  public:
    /* Font table structure */
    /* On EVE4 onwards this is called the Legacy Font Structure. */
    typedef struct
    {
        /* Width of each character font from 0 to 127 */
        uint8_t    FontWidth[128];
        /* Bitmap format of font - this is the same as the EVE_FORMAT_x, where x is L1, L2, L4 etc. */
        uint32_t    FontBitmapFormat;
        /* Font line stride in bytes */
        uint32_t    FontLineStride;
        /* Font width in pixels */
        uint32_t    FontWidthInPixels;
        /* Font height in pixels */
        uint32_t    FontHeightInPixels;
        /* Pointer to font graphics raw data */
        uint32_t    PointerToFontGraphicsData;
    } GPU_FONT_HEADER;



  private:
    void setpanel(uint32_t panel);
};

#endif    /* _BTEVE_H_ */
