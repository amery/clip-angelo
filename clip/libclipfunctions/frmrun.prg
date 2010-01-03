/***
*
*  Frmrun.prg
*
*  Clipper REPORT FORM runtime system
*
*  Copyright (c) 1990-1993, Computer Associates International, Inc.
*  All rights reserved.
*
*  Compile: /m /n /w
*
*/
**********************************************************************

#include "ci_frmdef.ch"
#include "ci_error.ch"
**********************************************************************

/***
*  Nation Message Constants
*  These constants are used with the NationMsg(<msg>) function.
*  The <msg> parameter can range from 1-12 and returns the national
*  version of the system message.
*/
#define _RF_PAGENO       3     // "Page No."
#define _RF_SUBTOTAL     4     // "** Subtotal **"
#define _RF_SUBSUBTOTAL  5     // "* Subsubtotal *"
#define _RF_TOTAL        6     // "*** Total ***"

#define S87_COMPAT					// preserve Summer '87 compatability
**********************************************************************

STATIC aReportData, nPageNumber, nLinesLeft, aReportTotals
STATIC aGroupTotals, lFirstPass, lFormFeeds, nMaxLinesAvail
**********************************************************************
#include "frmrun/static_EjectPage.ch"
#include "frmrun/static_ExecuteReport.ch"
#include "frmrun/static_MakeAStr.ch"
#include "frmrun/static_Occurs.ch"
#include "frmrun/static_PrintIt.ch"
#include "frmrun/static_ReportHeader.ch"
#include "frmrun/static_XMEMOLINE.ch"
#include "frmrun/static_XMLCOUNT.ch"
**********************************************************************
#include "frmrun/__ReportForm.ch"
**********************************************************************











