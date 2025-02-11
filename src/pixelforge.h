/**
 *  Copyright (c) 2024 Le Juez Victor
 *
 *  This software is provided "as-is", without any express or implied warranty. In no event 
 *  will the authors be held liable for any damages arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose, including commercial 
 *  applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not claim that you 
 *  wrote the original software. If you use this software in a product, an acknowledgment 
 *  in the product documentation would be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *  as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#ifndef PIXEL_FORGE_H
#define PIXEL_FORGE_H

#include <stdint.h>

#if defined(_WIN32)
#   if defined(__TINYC__)
#       define __declspec(x) __attribute__((x))
#   endif
#   if defined(PF_BUILD_SHARED)
#       define PF_API __declspec(dllexport)
#   elif defined(USE_LIBTYPE_SHARED)
#       define PF_API __declspec(dllimport)
#   endif
#else
#   if defined(PF_BUILD_SHARED)
#       define PF_API __attribute__((visibility("default")))
#   endif
#endif //OS

#ifndef PF_API
    #define PF_API
#endif //PF_API

#ifndef PF_MALLOC
#   define PF_MALLOC(size) malloc(size)
#endif //PF_MALLOC

#ifndef PF_CALLOC
#   define PF_CALLOC(count, size) calloc(count, size)
#endif //PF_CALLOC

#ifndef PF_FREE
#   define PF_FREE(size) free(size)
#endif //PF_FREE

#ifndef PF_MAX_MATRIX_STACK_SIZE
#   define PF_MAX_MATRIX_STACK_SIZE 8
#endif //PF_MAX_MATRIX_STACK_SIZE

#ifndef PF_MAX_LIGHTS
#   define PF_MAX_LIGHTS 8
#endif //PF_MAX_LIGHTS

#ifndef PF_MAX_CLIPPED_POLYGON_VERTICES
#   define PF_MAX_CLIPPED_POLYGON_VERTICES 12
#endif //PF_MAX_CLIPPED_POLYGON_VERTICES

#ifndef PF_PIXELFORMAT_R5G5B5A1_ALPHA_THRESHOLD
    #define PF_PIXELFORMAT_R5G5B5A1_ALPHA_THRESHOLD 50  // Threshold over 255 to set alpha as 0
#endif //PF_PIXELFORMAT_R5G5B5A1_ALPHA_THRESHOLD

#ifndef PF_CLIP_EPSILON
#   define PF_CLIP_EPSILON 1e-5f
#endif //PF_CLIP_EPSILON

#ifdef PF_USE_OPENMP
//  Pixel threshold for parallelizing the rasterization loop
#   ifndef PF_OPENMP_PIXEL_RASTER_THRESHOLD
#       define PF_OPENMP_PIXEL_RASTER_THRESHOLD 1024
#   endif //PF_OPENMP_PIXEL_RASTER_THRESHOLD
#endif //PF_USE_OPENMP

#ifndef INV_255
#   define INV_255 (1.0 / 255)
#endif //INV_255

#ifndef MIN_255
#   define MIN_255(n) (n | ((255 - n) >> 31))
#endif //MIN_255

#ifndef MAX_0
#   define MAX_0(n) (n & -(n >= 0))
#endif //MAX_0

#ifndef MIN
#   define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif //MIN

#ifndef MAX
#   define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif //MAX

#ifndef CLAMP
#   define CLAMP(x, lower, upper) (MAX((lower), MIN((x), (upper))))
#endif // CLAMP

/* Types definitions */

typedef enum {
    PF_FALSE = 0,
    PF_TRUE = 1
} PFboolean;

typedef int8_t      PFbyte;
typedef uint8_t     PFubyte;
typedef int16_t     PFshort;
typedef uint16_t    PFushort;
typedef int32_t     PFint;
typedef uint32_t    PFuint;
typedef int64_t     PFint64;
typedef uint64_t    PFuint64;
typedef uint32_t    PFsizei;
typedef uint32_t    PFenum;
typedef intptr_t    PFintptr;
typedef uintptr_t   PFsizeiptr;
typedef float       PFfloat;
typedef double      PFdouble;

/* Context defintions */

typedef struct PFctx PFctx;     // NOTE: This type is opaque, API functions are used to modify its state

typedef enum {
    PF_TEXTURE_2D   = 0x01,
    PF_DEPTH_TEST   = 0x02,
    PF_WIRE_MODE    = 0x04,
    PF_CULL_FACE    = 0x08,
    PF_LIGHTING     = 0x10
} PFstate;

/* Error enum */

typedef enum {
    PF_NO_ERROR,
    PF_INVALID_ENUM,
    PF_STACK_OVERFLOW,
    PF_INVALID_OPERATION,
    PF_ERROR_OUT_OF_MEMORY
} PFerrcode;

/* Render definitions */

typedef enum {
    PF_COLOR_BUFFER_BIT = 0x01,
    PF_DEPTH_BUFFER_BIT = 0x02
} PFclearflag;

typedef enum {
    PF_VERTEX_ARRAY         = 0x01,
    PF_NORMAL_ARRAY         = 0x02,
    PF_COLOR_ARRAY          = 0x04,
    PF_TEXTURE_COORD_ARRAY  = 0x08
} PFarraytype;

typedef enum {
    PF_MODELVIEW,
    PF_PROJECTION,
} PFmatrixmode;

typedef enum {
    PF_POINTS    = 1,
    PF_LINES     = 2,
    PF_TRIANGLES = 3,
    PF_QUADS     = 4,
} PFdrawmode;

typedef enum {
    PF_FRONT,
    PF_BACK,
    PF_FRONT_AND_BACK
} PFface;

typedef enum {
    PF_LIGHT0 = 0,
    PF_LIGHT1,
    PF_LIGHT2,
    PF_LIGHT3,
    PF_LIGHT4,
    PF_LIGHT5,
    PF_LIGHT6,
    PF_LIGHT7,
    PF_LIGHT8
} PFlights;

typedef enum {  // NOTE: Common to light and material
    PF_AMBIENT                  = 1,
    PF_DIFFUSE                  = 2,
    PF_SPECULAR                 = 3,
    PF_AMBIENT_AND_DIFFUSE      = 4
} PFrenderparam;

typedef enum {
    PF_POSITION                 = 5,
    PF_SPOT_DIRECTION           = 6,
    //PF_SPOT_EXPONENT          = 7,
    //PF_SPOT_CUTOFF            = 8,
    //PF_CONSTANT_ATTENUATION   = 9,
    //PF_LINEAR_ATTENUATION     = 10,
    //PF_QUADRATIC_ATTENUATION  = 11
} PFlightparam;

typedef enum {
    PF_EMISSION                 = 12,
    PF_SHININESS                = 13
} PFmaterialparam;

typedef struct {
    PFubyte r, g, b, a;
} PFcolor;

typedef PFcolor (*PFblendfunc)(PFcolor, PFcolor);

/* Texture definitions */

typedef struct PFtexture PFtexture;

typedef void (*PFpixelsetter)(void*, PFsizei, PFcolor);
typedef PFcolor (*PFpixelgetter)(const void*, PFsizei);

typedef enum {
    PF_PIXELFORMAT_UNKNOWN = 0,
    PF_PIXELFORMAT_GRAYSCALE,
    PF_PIXELFORMAT_GRAY_ALPHA,
    PF_PIXELFORMAT_R5G6B5,
    PF_PIXELFORMAT_R8G8B8,
    PF_PIXELFORMAT_R5G5B5A1,
    PF_PIXELFORMAT_R4G4B4A4,
    PF_PIXELFORMAT_R8G8B8A8,
    PF_PIXELFORMAT_R32,
    PF_PIXELFORMAT_R32G32B32,
    PF_PIXELFORMAT_R32G32B32A32,
    PF_PIXELFORMAT_R16,
    PF_PIXELFORMAT_R16G16B16,
    PF_PIXELFORMAT_R16G16B16A16,
} PFpixelformat;

struct PFtexture {
    PFpixelsetter pixelSetter;
    PFpixelgetter pixelGetter;
    void *pixels;
    PFsizei width;
    PFsizei height;
    PFpixelformat format;
};

/* Framebuffer defintions */

typedef struct {
    PFtexture texture;
    PFfloat *zbuffer;
} PFframebuffer;

#if defined(__cplusplus)
extern "C" {
#endif //__cplusplus

/* Context API functions */

PF_API PFctx* pfCreateContext(void* screenBuffer, PFsizei screenWidth, PFsizei screenHeight, PFpixelformat screenFormat);
PF_API void pfDeleteContext(PFctx* ctx);

PF_API PFctx* pfGetCurrentContext(void);
PF_API void pfMakeCurrent(PFctx* ctx);

PF_API PFboolean pfIsEnabled(PFstate state);

PF_API void pfEnable(PFstate state);
PF_API void pfDisable(PFstate state);

/* Error management API functions */

PF_API PFerrcode pfGetError(void);

/* Matrix management API functions */

PF_API void pfMatrixMode(PFmatrixmode mode);

PF_API void pfPushMatrix(void);
PF_API void pfPopMatrix(void);

PF_API void pfLoadIdentity(void);

PF_API void pfTranslatef(PFfloat x, PFfloat y, PFfloat z);
PF_API void pfRotatef(PFfloat angle, PFfloat x, PFfloat y, PFfloat z);
PF_API void pfScalef(PFfloat x, PFfloat y, PFfloat z);

PF_API void pfMultMatrixf(const PFfloat* mat);

PF_API void pfFrustum(PFdouble left, PFdouble right, PFdouble bottom, PFdouble top, PFdouble znear, PFdouble zfar);
PF_API void pfOrtho(PFdouble left, PFdouble right, PFdouble bottom, PFdouble top, PFdouble znear, PFdouble zfar);

/* Render configuration API functions */

PF_API void pfGetViewport(PFint* x, PFint* y, PFsizei* width, PFsizei* height);
PF_API void pfViewport(PFint x, PFint y, PFsizei width, PFsizei height);

PF_API void pfSetDefaultPixelGetter(PFpixelgetter func);
PF_API void pfSetDefaultPixelSetter(PFpixelsetter func);

PF_API PFblendfunc pfGetBlendFunction(void);
PF_API void pfSetBlendFunction(PFblendfunc func);

PF_API PFface pfGetCullFace(void);
PF_API void pfSetCullFace(PFface face);

PF_API void pfEnableStatePointer(PFarraytype vertexAttribType, const void* buffer);
PF_API void pfDisableStatePointer(PFarraytype vertexAttribType);

PF_API PFframebuffer* pfGetActiveFramebuffer(void);
PF_API void pfEnableFramebuffer(PFframebuffer* framebuffer);
PF_API void pfDisableFramebuffer(void);

PF_API PFtexture* pfGetActiveTexture(void);
PF_API void pfBindTexture(PFtexture* texture);

PF_API void pfEnableLight(PFsizei light);
PF_API void pfDisableLight(PFsizei light);
PF_API void pfLightfv(PFsizei light, PFenum param, const void* value);

PF_API void pfMaterialf(PFface face, PFenum param, PFfloat value);
PF_API void pfMaterialfv(PFface face, PFenum param, const void* value);

PF_API void pfClear(PFclearflag flag);
PF_API void pfClearColor(PFubyte r, PFubyte g, PFubyte b, PFubyte a);

PF_API void pfDrawVertexArrayElements(PFsizei offset, PFsizei count, const void *buffer);
PF_API void pfDrawVertexArray(PFsizei offset, PFsizei count);

/* Primitives drawing API functions */

PF_API void pfBegin(PFdrawmode mode);
PF_API void pfEnd(void);

PF_API void pfVertex2i(PFint x, PFint y);
PF_API void pfVertex2f(PFfloat x, PFfloat y);
PF_API void pfVertex2fv(const PFfloat* v);

PF_API void pfVertex3i(PFint x, PFint y, PFint z);
PF_API void pfVertex3f(PFfloat x, PFfloat y, PFfloat z);
PF_API void pfVertex3fv(const PFfloat* v);

PF_API void pfVertex4i(PFint x, PFint y, PFint z, PFint w);
PF_API void pfVertex4f(PFfloat x, PFfloat y, PFfloat z, PFfloat w);
PF_API void pfVertex4fv(const PFfloat* v);

PF_API void pfColor3ub(PFubyte r, PFubyte g, PFubyte b);
PF_API void pfColor3ubv(const PFubyte* v);

PF_API void pfColor3us(PFushort r, PFushort g, PFushort b);
PF_API void pfColor3usv(const PFushort* v);

PF_API void pfColor3ui(PFuint r, PFuint g, PFuint b);
PF_API void pfColor3uiv(const PFuint* v);

PF_API void pfColor3f(PFfloat r, PFfloat g, PFfloat b);
PF_API void pfColor3fv(const PFfloat* v);

PF_API void pfColor4ub(PFubyte r, PFubyte g, PFubyte b, PFubyte a);
PF_API void pfColor4ubv(const PFubyte* v);

PF_API void pfColor4us(PFushort r, PFushort g, PFushort b, PFushort a);
PF_API void pfColor4usv(const PFushort* v);

PF_API void pfColor4ui(PFuint r, PFuint g, PFuint b, PFuint a);
PF_API void pfColor4uiv(const PFuint* v);

PF_API void pfColor4f(PFfloat r, PFfloat g, PFfloat b, PFfloat a);
PF_API void pfColor4fv(const PFfloat* v);

PF_API void pfTexCoord2f(PFfloat u, PFfloat v);
PF_API void pfTexCoordfv(const PFfloat* v);

PF_API void pfNormal3f(PFfloat x, PFfloat y, PFfloat z);
PF_API void pfNormal3fv(const PFfloat* v);

/* Supplementary primitive drawing API functions */

PF_API void pfRects(PFshort x1, PFshort y1, PFshort x2, PFshort y2);
PF_API void pfRectsv(const PFshort* v1, const PFshort* v2);

PF_API void pfRecti(PFint x1, PFint y1, PFint x2, PFint y2);
PF_API void pfRectiv(const PFint* v1, const PFint* v2);

PF_API void pfRectf(PFfloat x1, PFfloat y1, PFfloat x2, PFfloat y2);
PF_API void pfRectfv(const PFfloat* v1, const PFfloat* v2);

/* Drawing pixels API functions */

PF_API void pfDrawPixels(PFsizei width, PFsizei height, PFpixelformat format, const void* pixels);

PF_API void pfPixelZoom(PFfloat xfactor, PFfloat yfactor);

PF_API void pfRasterPos2i(PFint x, PFint y);
PF_API void pfRasterPos2f(PFfloat x, PFfloat y);
PF_API void pfRasterPos2fv(const PFfloat* v);

PF_API void pfRasterPos3i(PFint x, PFint y, PFint z);
PF_API void pfRasterPos3f(PFfloat x, PFfloat y, PFfloat z);
PF_API void pfRasterPos3fv(const PFfloat* v);

PF_API void pfRasterPos4i(PFint x, PFint y, PFint z, PFint w);
PF_API void pfRasterPos4f(PFfloat x, PFfloat y, PFfloat z, PFfloat w);
PF_API void pfRasterPos4fv(const PFfloat* v);

/* Misc API functions */

// Makes a copy of the current framebuffer to a buffer whose format must be specified.
PF_API void pfReadPixels(PFint x, PFint y, PFsizei width, PFsizei height, PFpixelformat format, void* pixels);

/* Blending functions */

PF_API PFcolor pfBlendDisabled(PFcolor source, PFcolor destination);

PF_API PFcolor pfBlend(PFcolor source, PFcolor destination);
PF_API PFcolor pfBlendAlpha(PFcolor source, PFcolor destination);
PF_API PFcolor pfBlendAdditive(PFcolor source, PFcolor destination);
PF_API PFcolor pfBlendSubtractive(PFcolor source, PFcolor destination);
PF_API PFcolor pfBlendMultiplicative(PFcolor source, PFcolor destination);

/* Framebuffer functions */

PFframebuffer pfGenFramebuffer(PFsizei width, PFsizei height, PFpixelformat format);
PF_API void pfDeleteFramebuffer(PFframebuffer* framebuffer);

PF_API PFboolean pfIsValidFramebuffer(PFframebuffer* framebuffer);

// NOTE: This function clears the color as well as depth buffer
PF_API void pfClearFramebuffer(PFframebuffer* framebuffer, PFcolor color);

PF_API void pfSetFramebufferPixelDepth(PFframebuffer* framebuffer, PFsizei x, PFsizei y, PFfloat z, PFcolor color);
PF_API PFfloat pfGetFramebufferDepth(const PFframebuffer* framebuffer, PFsizei x, PFsizei y);

PF_API void pfSetFramebufferPixel(PFframebuffer* framebuffer, PFsizei x, PFsizei y, PFcolor color);
PF_API PFcolor pfGetFramebufferPixel(const PFframebuffer* framebuffer, PFsizei x, PFsizei y);

/* Texture functions */

PF_API PFtexture pfGenTexture(void* pixels, PFsizei width, PFsizei height, PFpixelformat format);
PF_API PFtexture pfGenTextureBuffer(PFsizei width, PFsizei height, PFpixelformat format);
PF_API PFtexture pfGenColorTextureBuffer(PFsizei width, PFsizei height, PFcolor color, PFpixelformat format);

PF_API void pfDeleteTexture(PFtexture* texture);

PF_API PFboolean pfIsValidTexture(PFtexture* texture);

PF_API void pfSetTexturePixel(PFtexture* texture, PFsizei x, PFsizei y, PFcolor color);
PF_API PFcolor pfGetTexturePixel(const PFtexture* texture, PFsizei x, PFsizei y);

// NOTE: To use these functions correctly, textures must be POT
PF_API void pfSetTextureSample(PFtexture* texture, PFfloat u, PFfloat v, PFcolor color);
PF_API PFcolor pfGetTextureSample(const PFtexture* texture, PFfloat u, PFfloat v);

#if defined(__cplusplus)
}
#endif //__cplusplus

#endif //PIXEL_FORGE_H
