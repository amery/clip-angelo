/*
Start total new system v. 0.0
with hard coded long name variables to have clear system
Angelo GIRARDI
*/
/*
   gen-tbl: uptbl, lowtbl, cmptbl, alphatbl, pgcharstbl generator
   Generate all tables from charset->unicode mapping and
   standard unicode data.
   Copyleft Paul Lasarev <paul@itk.ru>, 2000
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <ci_charset.h>
#include <ci_coll.h>
#include <ci_screen.h>

/*#define DBG*/
/*#define DBG1*/
/*#define DBG3*/

char *CLIPROOT = ".";

#include <gen_tbl/typedef_struct_UniRecord.h>

/* shorten names */
#define PG_H 	PG_HLINE
#define PG_UR 	PG_LLCORNER
#define PG_UL 	PG_LRCORNER
#define PG_HD 	PG_TTEE
#define PG_VL 	PG_RTEE
#define PG_VR 	PG_LTEE
#define PG_HU 	PG_BTEE
#define PG_DR 	PG_ULCORNER
#define PG_DL 	PG_URCORNER
#define PG_V 	PG_VLINE

#define PG_H2 	PG_HLINE2
#define PG_UR2 	PG_LLCORNER2
#define PG_UL2 	PG_LRCORNER2
#define PG_HD2 	PG_TTEE2
#define PG_VL2 	PG_RTEE2
#define PG_VR2 	PG_LTEE2
#define PG_HU2 	PG_BTEE2
#define PG_DR2 	PG_ULCORNER2
#define PG_DL2 	PG_URCORNER2
#define PG_V2	PG_VLINE2

#define PG_HV	PG_PLUS
#define PG_HV2	PG_PLUS2
#define PG_B	PG_BLOCK

#define PG_DP PG_DARROW
#define PG_LP PG_LARROW
#define PG_RP PG_RARROW
#define PG_UP PG_UARROW

/* unicode BOX DRAWING  (U+2500 -:- U+25FF ) to PG_CHARS mapping */
static unsigned char pg_chars[256] = {
/* U+2500 */ PG_H, PG_H, PG_V, PG_V, PG_H2, PG_H2, PG_V2, PG_V2,
/* U+2508 */ PG_H2, PG_H2, PG_V2, PG_V2, PG_DR, PG_DR, PG_DR, PG_DR2,
/* U+2510 */ PG_DL, PG_DL, PG_DL, PG_DL2, PG_UR, PG_UR, PG_UR, PG_UR2,
/* U+2518 */ PG_UL, PG_UL, PG_UL, PG_UL2, PG_VR, PG_VR, PG_VR, PG_VR2,
/* U+2520 */ PG_VR, PG_VR, PG_VR, PG_VR2, PG_VL, PG_VL, PG_VL, PG_VL,
/* U+2528 */ PG_VL, PG_VL, PG_VL, PG_VL2, PG_HD, PG_HD, PG_HD, PG_HD,
/* U+2530 */ PG_HD, PG_HD, PG_HD, PG_HD2, PG_HU, PG_HU, PG_HU, PG_HU,
/* U+2538 */ PG_HU, PG_HU, PG_HU, PG_HU2, PG_HV, PG_HV, PG_HV, PG_HV,
/* U+2540 */ PG_HV, PG_HV, PG_HV, PG_HV, PG_HV, PG_HV, PG_HV, PG_HV,
/* U+2548 */ PG_HV, PG_HV, PG_HV, PG_HV2, PG_H2, PG_H2, PG_V2, PG_V2,
/* U+2550 */ PG_H2, PG_V2, PG_DR, PG_DR, PG_DR2, PG_DL, PG_DL, PG_DL2,
/* U+2558 */ PG_UR, PG_UR, PG_UR2, PG_UL, PG_UL, PG_UL2, PG_VR, PG_VR,
/* U+2560 */ PG_VR2, PG_VL, PG_VL, PG_VL2, PG_HD, PG_HD, PG_HD2, PG_HU,
/* U+2568 */ PG_HU, PG_HU2, PG_HV, PG_HV, PG_HV2, PG_DR, PG_DL, PG_UL,
/* U+2570 */ PG_UR, PG_HV, PG_HV, PG_HV, PG_H, PG_V, PG_H, PG_V,
/* U+2578 */ PG_H2, PG_V2, PG_H2, PG_V2, PG_H, PG_V, PG_H, PG_V,
/* U+2580 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+2588 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+2590 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+2598 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+25a0 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+25a8 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+25b0 */ PG_B, PG_B, PG_UP, PG_UP, PG_UP, PG_UP, PG_RP, PG_RP,
/* U+25b8 */ PG_RP, PG_RP, PG_RP, PG_RP, PG_DP, PG_DP, PG_DP, PG_DP,
/* U+25c0 */ PG_LP, PG_LP, PG_LP, PG_LP, PG_LP, PG_LP, PG_DIAMOND,
   PG_DIAMOND,
/* U+25c8 */ PG_DIAMOND, 0, 0, 0, 0, 0, 0, 0,
/* U+25d0 */ 0, 0, 0, 0, 0, 0, 0, 0,
/* U+25d8 */ 0, 0, 0, 0, 0, 0, 0, 0,
/* U+25e0 */ 0, 0, 0, 0, 0, 0, 0, PG_B,
/* U+25e8 */ PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B, PG_B,
/* U+25f0 */ 0, 0, 0, 0, 0, 0, 0, 0,
/* U+25f8 */ 0, 0, 0, 0, 0, 0, 0, 0,
};

#include <gen_tbl/static_cmp_UniRecord.c>
#include <gen_tbl/static_cmp_CSRecord.c>
#include <gen_tbl/static_in_map.c>
#include <gen_tbl/static_find_uni.c>
#include <gen_tbl/static_find_ch.c>
#include <gen_tbl/static_find_cmp.c>
#include <gen_tbl/static_remove_char.c>
#include <gen_tbl/main.c>
