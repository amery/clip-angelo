/***
*
*  Lblback.prg
*
*  Clipper LABEL FORM runtime system
*
*  Copyright (c) 1990-1993, Computer Associates International, Inc.
*  All rights reserved.
*
*  Compile: /m /n /w
*
*/
**********************************************************
#include <ci_lbldef.ch>
#include <ci_error.ch>
**********************************************************
#define BUFFSIZE        1034          // Size of label file
#define FILEOFFSET      74            // Start of label content descriptions
#define FIELDSIZE       60
#define REMARKOFFSET    2
#define REMARKSIZE      60
#define HEIGHTOFFSET    62
#define HEIGHTSIZE      2
#define WIDTHOFFSET     64
#define WIDTHSIZE       2
#define LMARGINOFFSET   66
#define LMARGINSIZE     2
#define LINESOFFSET     68
#define LINESSIZE       2
#define SPACESOFFSET    70
#define SPACESSIZE      2
#define ACROSSOFFSET    72
#define ACROSSSIZE      2
// File error definitions
#define  F_OK              0          // No error
#define  F_EMPTY          -3          // File is empty
#define  F_ERROR          -1          // Some kind of error
#define  F_NOEXIST         2          // File does not exist
**********************************************************
**********************************************************
#include <lblback/static_ListAsArray.prg>
**********************************************************
#include <lblback/__LblLoad.prg>
**********************************************************
