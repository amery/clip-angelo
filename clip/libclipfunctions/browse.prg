/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_box.ch>
#include <ci_Inkey.ch>
#include <ci_Setcurs.ch>
#include <ci_Error.ch>
#include <ci_config.ch>
**********************************************************************


#define TURN_ON_APPEND_MODE(b)      (b:cargo := .T.)
#define TURN_OFF_APPEND_MODE(b)     (b:cargo := .F.)
#define IS_APPEND_MODE(b)           (b:cargo)
#define MY_HEADSEP      translate_charset(__CHARSET__,host_charset(),"��")
#define MY_COLSEP       translate_charset(__CHARSET__,host_charset(),"� ")

#define MSG_RECORDS [Record N ]
**********************************************************************
#include <browse/static_ApplyKey.prg>
#include <browse/static_DoGet.prg>
#include <browse/static_FancyColors.prg>
#include <browse/static_InsToggle.prg>
#include <browse/static_MyBrowse.prg>
#include <browse/static_Skipper.prg>
#include <browse/static_StockBrowseNew.prg>
**********************************************************************
#include <browse/browse.prg>
**********************************************************************
