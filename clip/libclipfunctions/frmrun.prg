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

#include <ci_frmdef.ch>
#include <ci_error.ch>
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
#include <frmrun/static_EjectPage.prg>
#include <frmrun/static_ExecuteReport.prg>
#include <frmrun/static_MakeAStr.prg>
#include <frmrun/static_Occurs.prg>
#include <frmrun/static_PrintIt.prg>
#include <frmrun/static_ReportHeader.prg>
#include <frmrun/static_XMEMOLINE.prg>
#include <frmrun/static_XMLCOUNT.prg>
**********************************************************************
#include <frmrun/__ReportForm.prg>
**********************************************************************











