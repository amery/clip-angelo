/***
*
*  Frmback.prg
*
*  Create a report array from a (.frm) file
*
*  Copyright (c) 1990-1993, Computer Associates International, Inc.
*  All rights reserved.
*
*  Compile: /m /n /w
*
*/
**********************************************************************

#include <ci_frmdef.ch>
#include <ci_error.ch>
**********************************************************************

// Definitions for buffer sizes
#define  SIZE_FILE_BUFF             1990       // Size of report file
#define  SIZE_LENGTHS_BUFF          110
#define  SIZE_OFFSETS_BUFF          110
#define  SIZE_EXPR_BUFF             1440
#define  SIZE_FIELDS_BUFF           300
#define  SIZE_PARAMS_BUFF           24

// Definitions for offsets into the FILE_BUFF string
#define  LENGTHS_OFFSET             5          // Start of expression length array
#define  OFFSETS_OFFSET             115        // Start of expression position array
#define  EXPR_OFFSET                225        // Start of expression data area
#define  FIELDS_OFFSET              1665       // Start of report columns (fields)
#define  PARAMS_OFFSET              1965       // Start of report parameters block

// These are offsets into the FIELDS_BUFF string to actual values
// Values are added to a block offset FLD_OFFSET that is moved in
// increments of 12
#define  FIELD_WIDTH_OFFSET         1
#define  FIELD_TOTALS_OFFSET        6
#define  FIELD_DECIMALS_OFFSET      7

// These are offsets into FIELDS_BUFF which are used to 'point' into
// the EXPR_BUFF string which contains the textual data
#define  FIELD_CONTENT_EXPR_OFFSET  9
#define  FIELD_HEADER_EXPR_OFFSET   11

// These are actual offsets into the PARAMS_BUFF string which
// are used to 'point' into the EXPR_BUFF string
#define  PAGE_HDR_OFFSET            1
#define  GRP_EXPR_OFFSET            3
#define  SUB_EXPR_OFFSET            5
#define  GRP_HDR_OFFSET             7
#define  SUB_HDR_OFFSET             9

// These are actual offsets into the PARAMS_BUFF string to actual values
#define  PAGE_WIDTH_OFFSET          11
#define  LNS_PER_PAGE_OFFSET        13
#define  LEFT_MRGN_OFFSET           15
#define  RIGHT_MGRN_OFFSET          17
#define  COL_COUNT_OFFSET           19
#define  DBL_SPACE_OFFSET           21
#define  SUMMARY_RPT_OFFSET         22
#define  PE_OFFSET                  23
#define  OPTION_OFFSET              24

// File error definitions
#define  F_OK                       0          // No error
#define  F_EMPTY                   -3          // File is empty
#define  F_ERROR                   -1          // Some kind of error
#define  F_NOEXIST                  2          // File does not exist

**********************************************************************
// Declare file-wide statics
STATIC cExprBuff
STATIC cOffsetsBuff
STATIC cLengthsBuff
**********************************************************************
#include <frmback/static_GetColumn.prg>
#include <frmback/static_GetExpr.prg>
#include <frmback/static_ListAsArray.prg>
**********************************************************************
#include <frmback/__FrmLoad.prg>
#include <frmback/ParseHeader.prg>
**********************************************************************
