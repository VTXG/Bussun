#ifndef RVL_FACE_LIBRARY_INTENRAL_CONFIG_H
#define RVL_FACE_LIBRARY_INTERNAL_CONFIG_H
#include <RFLi_Types.h>
#include <revolution/gx.h>
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Char data constraints
 */

#define RFLi_MAX_FACELINE_TYPE 7
#define RFLi_MAX_FACELINE_COLOR 5
#define RFLi_MAX_FACELINE_TEXTURE 11

#define RFLi_MAX_HAIR_TYPE 71
#define RFLi_MAX_HAIR_COLOR 7

#define RFLi_MAX_EYE_TYPE 47
#define RFLi_MAX_EYE_COLOR 5
#define RFLi_MAX_EYE_SCALE 7
#define RFLi_MAX_EYE_ROTATE 7
#define RFLi_MAX_EYE_X 12
#define RFLi_MAX_EYE_Y 18

#define RFLi_MAX_EYEBROW_TYPE 23
#define RFLi_MAX_EYEBROW_COLOR 7
#define RFLi_MAX_EYEBROW_SCALE 8
#define RFLi_MAX_EYEBROW_ROTATE 11
#define RFLi_MAX_EYEBROW_X 12
#define RFLi_MIN_EYEBROW_Y 3
#define RFLi_MAX_EYEBROW_Y 18

#define RFLi_MAX_NOSE_TYPE 11
#define RFLi_MAX_NOSE_SCALE 8
#define RFLi_MAX_NOSE_Y 18

#define RFLi_MAX_MOUTH_TYPE 23
#define RFLi_MAX_MOUTH_COLOR 2
#define RFLi_MAX_MOUTH_SCALE 8
#define RFLi_MAX_MOUTH_Y 18

#define RFLi_MAX_BEARD_MUSTACHE 3
#define RFLi_MAX_BEARD_TYPE 3
#define RFLi_MAX_BEARD_COLOR 7
#define RFLi_MAX_BEARD_SCALE 8
#define RFLi_MAX_BEARD_Y 16

#define RFLi_MAX_GLASS_TYPE 8
#define RFLi_MAX_GLASS_COLOR 5
#define RFLi_MAX_GLASS_SCALE 7
#define RFLi_MAX_GLASS_Y 20

#define RFLi_MAX_MOLE_TYPE 1
#define RFLi_MAX_MOLE_SCALE 8
#define RFLi_MAX_MOLE_X 16
#define RFLi_MAX_MOLE_Y 30

#define RFLi_MAX_BODY_HEIGHT 128
#define RFLi_MAX_BODY_BUILD 128

/**
 * Create ID
 */

#define RFLi_CREATE_ID_MASK_TEMPORARY 0x20000000
#define RFLi_CREATE_ID_MASK_NOT_SPECIAL 0x80000000

/**
 * Face texture
 */

// TODO: A few values need to be converted to fractions/expressions
// Once RFLi_TEX_SCALE_Y becomes a fraction, replace the 1.1600001 with 1.16

#define RFLi_TEX_SCALE_X (0.88961464f)
#define RFLi_TEX_SCALE_Y (0.9276675f)

#define RFLi_TEX_SCALE2DIM(scale) (1.0 + 0.4f * (scale))
#define RFLi_TEX_ROTATE2ANG(rotate) ((360.0f / 32.0f) * ((rotate) % 32))
#define RFLi_TEX_UNIT(x) ((x) / 64.0f)

#define RFLi_TEX_EYE_BASE_X RFLi_TEX_UNIT(0)
#define RFLi_TEX_EYE_BASE_Y (18.451525f)
#define RFLi_TEX_EYE_BASE_W RFLi_TEX_UNIT(342)
#define RFLi_TEX_EYE_BASE_H RFLi_TEX_UNIT(288)

#define RFLi_TEX_EYEBROW_BASE_X RFLi_TEX_UNIT(0)
#define RFLi_TEX_EYEBROW_BASE_Y (16.549807f)
#define RFLi_TEX_EYEBROW_BASE_W RFLi_TEX_UNIT(324)
#define RFLi_TEX_EYEBROW_BASE_H RFLi_TEX_UNIT(288)

#define RFLi_TEX_MOUTH_BASE_Y (29.25885f)
#define RFLi_TEX_MOUTH_BASE_W RFLi_TEX_UNIT(396)
#define RFLi_TEX_MOUTH_BASE_H RFLi_TEX_UNIT(288)

#define RFLi_TEX_MUSTACHE_BASE_Y (31.763554f)
#define RFLi_TEX_MUSTACHE_BASE_W RFLi_TEX_UNIT(288)
#define RFLi_TEX_MUSTACHE_BASE_H RFLi_TEX_UNIT(576)

#define RFLi_TEX_MOLE_BASE_X (17.766165f)
#define RFLi_TEX_MOLE_BASE_Y (17.95986f)
#define RFLi_TEX_MOLE_BASE_W RFLi_TEX_UNIT(0)
#define RFLi_TEX_MOLE_BASE_H RFLi_TEX_UNIT(0)

/**
 * Tables
 */

// Two unused eye types?
extern const u8 RFLi_EYE_ROT_OFFSET[];

extern const u8 RFLi_EYEBROW_ROT_OFFSET[];

/**
 * Colors for TEV
 */
extern const GXColor RFLi_MOLE_COLOR0;

extern const GXColor RFLi_EYE_COLOR1[];

extern const GXColor RFLi_EYEBROW_COLOR0[];

extern const GXColor RFLi_MOUTH_COLOR0[];
extern const GXColor RFLi_MOUTH_COLOR1[];

extern const GXColor RFLi_BEARD_COLOR0[];

#ifdef __cplusplus
}
#endif
#endif
