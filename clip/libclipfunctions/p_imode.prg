*********** Source code for CA-Clipper Tools / 5.2 functions **************
*
* Program     : P_IMODE.PRG
* Copyright   : (c) 1991-93, Computer Associates International.
*               All rights reserved.
* Date        : 07/06/91
* Author      : Lothar Bongartz
* Description : Source code for INPUTMODE()
* Compiling   : /l /m /n /w /v /r
*
* Linking     : P_IMODE can be added to the .OBJ file list during
*               linking and replaces the module with the same name in
*               CT.LIB.
*
* Remarks     : INKEY() and debugger inputs cannot be trapped, and a
*               value cannot be determined. There is no difference
*               between an INPUT and an ACCEPT.
*               An 'earlier input' is unknown. A parameter (lDummy)
*               is ineffective.
*
**************************************************************************
**********************************************************

#define IM_WAIT          1
#define IM_ACCEPT        2
#define IM_INPUT         3
#define IM_READ          4
#define IM_MEMOEDIT      5
#define IM_MENU          6
#define IM_INKEY         7
#define IM_DEBUG         8
#define IM_GETINPUT      9
#define IM_GETSECRET    10
#define IM_ACHOICE      11
**********************************************************
**********************************************************
**********************************************************
**********************************************************
#include <p_imode/INPUTMODE.prg>
**********************************************************
