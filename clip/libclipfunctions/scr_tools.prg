/*
	Copyright (C) 2002  ITK
	Author  : Tsarenko P  <tpe@dongu.donetsk.ua>
	License : (GPL) http://www.itk.ru/clipper/license.html
*/
**************************************************************************
#include <ci_config.ch>
**************************************************************************
**************************************************************************
**************************************************************************
**************************************************************************
**************************************************************************
#include <scr_tools/CharWin.prg>
#include <scr_tools/ClearEOL.prg>
#include <scr_tools/ClearSlow.prg>
#include <scr_tools/ClearWin.prg>
#include <scr_tools/ClEOL.prg>
#include <scr_tools/ClWin.prg>
#include <scr_tools/ColorRepl.prg>
#include <scr_tools/ColorWin.prg>
#include <scr_tools/SayDown.prg>
#include <scr_tools/SayMoveIn.prg>
#include <scr_tools/SaySpread.prg>
#include <scr_tools/SayScreen.prg>
**************************************************************************
/*
***************************************************************
Function ScreenMix(cStr, cAttr, nRow, nCol)
	Local nLen := Min(len(cStr), len(cAttr))
	Local cScr := '', ser
	if nRow == nil
		nRow := Row()
	endif
	if nCol == nil
		nCol := Col()
	endif
	for ser := 1 to nLen
		cScr += Substr(cStr, ser, 1) + Substr(cAttr, ser, 1) + Chr(0)
	next
	RestScreen(nRow, nCol, nRow, nCol+nLen-1, cScr)
Return ''
*/

***************************************************************

***********************************

***********************************
