/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_config.ch>
#include <ci_button.ch>
**********************************************************************
#ifdef __UNIX__
  #define SB_UPSYM      "�"
  #define SB_DOWNSYM    "�"
  #define SB_LEFTSYM    "<"
  #define SB_RIGHTSYM   ">"
  #define SB_THUMBSYM   "#"
#else
  #define SB_UPSYM      chr(24)
  #define SB_DOWNSYM    chr(25)
  #define SB_LEFTSYM    chr(27)
  #define SB_RIGHTSYM   chr(26)
  #define SB_THUMBSYM   "�"
#endif
#define SB_FILLSYM    "�"
**********************************************************************
#include <scrolbar/static_sbv_display.prg>
#include <scrolbar/static_sbv_hitTest.prg>
#include <scrolbar/static_scrollBarHorizontal.prg>
#include <scrolbar/static_scrollBarVertical.prg>
#include <scrolbar/static_sbh_display.prg>
#include <scrolbar/static_sbh_hitTest.prg>
**********************************************************************
#include <scrolbar/_recover_scrollbar.prg>
#include <scrolbar/scrollBar.prg>
**********************************************************************
