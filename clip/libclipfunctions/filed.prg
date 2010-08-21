/*
  All right reserved. (C) ITK,Izhevsk,Russia
  Author - Kornilova Elena
  E-mail - alena@itk.ru
  Home ftp - ftp.itk.ru/pub/clip
  Date  - 15/05/2000 year
  License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_inkey.ch>
#include <ci_clipcfg.h>
#include <ci_pgch.ch>
**********************************************************************
#define FA_NORMAL	0
#define FA_READONLY	1
#define FA_HIDDEN	2
#define FA_SYSTEM	4
#define FA_VOLUME	8
#define FA_DIRECTORY	16
#define FA_ARCHIVE	32
#define HISTORY_SIZE	25
#define DOP		replicate(" ", 256)
#define HISTORY_COLORS	"15/3,15/7,0/7,0/3,0/2"
#define DIALOG_COLORS	"0/7"
#define LIST_COLORS	"0/3,0/2,15/3,0/3,14/3,14/2,15/2"
#define CNT_COLUMNS	3
/*#define DELIMITER	translate_charset(__CHARSET__, host_charset(), "�")*/
/*#define DELIMITER 	chr(PGCH_VLINE)*/
#define DELIMITER	iif(set(_SET_DISPBOX),translate_charset("cp437", host_charset(), CHR(179)),chr(PGCH_VLINE))
**********************************************************************
/***********************************/
/* pDir  - current directory       */
/* pMask - files template 	   */
/* History - history object        */
/***********************************/
static nLeft, nRight, nTop, nBottom, lItem, lenCol, pHistory, fdp:=map()
**********************************************************************
#include <filed/static___get_handler.prg>
#include <filed/static___list_handler.prg>
#include <filed/static_initItem.prg>
#include <filed/static_lastdir.prg>
#include <filed/static_showview.prg>
**********************************************************************
#include <filed/fileDialog.prg>
**********************************************************************
