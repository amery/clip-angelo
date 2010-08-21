/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************

#include <ci_button.ch>
#include <ci_getexit.ch>
#include <ci_inkey.ch>
#include <ci_llibg.ch>
#include <ci_set.ch>
#include <ci_setCurs.ch>
#include <ci_tbrowse.ch>

#define K_UNDO          K_CTRL_U
//#define CTRL_END_SPECIAL

#ifndef SETCURSOR_IN_GETSYS
	#xdefine setcursor __setcursorempty
#endif

#define _GET_INSERT_ON   7     // "Ins"
#define _GET_INSERT_OFF  8     // "   "
#define _GET_INVD_DATE   9     // "Invalid Date"
#define _GET_RANGE_FROM  10    // "Range: "
#define _GET_RANGE_TO    11    // " - "
// Display coordinates for SCOREBOARD
#define SCORE_ROW      0
#define SCORE_COL      60
**********************************************************

STATIC oStatus
static aGetList
static keyExit:=15
**********************************************************
#include <getsys/static___getSysInit.prg>
#include <getsys/static_ClearGetSysVars.prg>
#include <getsys/static_DateMsg.prg>
#include <getsys/static_PostActiveGet.prg>
#include <getsys/static_RestoreGetSysVars.prg>
#include <getsys/static_Settle.prg>
#include <getsys/static_ShowScoreboard.prg>
**********************************************************
#include <getsys/__KillRead.prg>
#include <getsys/__setcursorempty.prg>
#include <getsys/__SetFormat.prg>
#include <getsys/_procreq_.prg>
#include <getsys/Accelerator.prg>
#include <getsys/CurrentGet.prg>
#include <getsys/EraseGetMsg.prg>
#include <getsys/GetActive.prg>
#include <getsys/GetActivePos.prg>
#include <getsys/GetApplyKey.prg>
#include <getsys/GetDoSetKey.prg>
#include <getsys/GetFldVar.prg>
#include <getsys/GetPreValidate.prg>
#include <getsys/GetPostValidate.prg>
#include <getsys/GetReader.prg>
#include <getsys/getSysInit.prg>
#include <getsys/GUIApplyKey.prg>
#include <getsys/GUIPostValidate.prg>
#include <getsys/GUIPreValidate.prg>
#include <getsys/GUIReader.prg>
#include <getsys/HitTest.prg>
#include <getsys/RangeCheck.prg>
#include <getsys/ReadExit.prg>
#include <getsys/ReadFormat.prg>
#include <getsys/ReadInsert.prg>
#include <getsys/ReadKey.prg>
#include <getsys/ReadKill.prg>
#include <getsys/ReadModal.prg>
#include <getsys/ReadStats.prg>
#include <getsys/ReadUpdated.prg>
#include <getsys/restGets.prg>
#include <getsys/saveGets.prg>
#include <getsys/ShowGetMsg.prg>
#include <getsys/TBApplyKey.prg>
#include <getsys/TBReader.prg>
#include <getsys/Updated.prg>
#include <getsys/UpDateGets.prg>
**********************************************************
