/*
	Copyright (C) 2001  ITK
	Authors : Uri (uri@itk.ru)
	License : (GPL) http://www.itk.ru/clipper/license.html
*/
**************************************************************************
#define NO_PGCH
**************************************************************************
#include <ci_box.ch>
#include <ci_lang.ch>
#include <ci_ctwin.ch>
**************************************************************************
#ifdef __CLIP__
  #define mprompt prompt
  #define MPROMPT PROMPT
  #define MMENU   MENU
#endif
**************************************************************************
static __Xscreen:={} // for __XsaveScreen
**************************************************************************
**************************************************************************
#include <scr_func/static_in_key.prg>
**************************************************************************
#include <scr_func/__WAIT.prg>
#include <scr_func/__XRestScreen.prg>
#include <scr_func/__XSaveScreen.prg>
#include <scr_func/alert.prg>
#include <scr_func/ColorMess.prg>
#include <scr_func/copy_scr.prg>
#include <scr_func/dbgshadow.prg>
#include <scr_func/dispbox.prg>
#include <scr_func/dispboxSay.prg>
#include <scr_func/machoice.prg>
#include <scr_func/message.prg>
#include <scr_func/messagep.prg>
#include <scr_func/otvet.prg>
#include <scr_func/readkeyb.prg>
#include <scr_func/restcursor.prg>
#include <scr_func/savecursor.prg>
#include <scr_func/Say.prg>
#include <scr_func/setrc.prg>
#include <scr_func/wbox.prg>
#include <scr_func/winbuf_dispbox.prg>
***************************************************************
