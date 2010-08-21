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
#include <ci_dbedit.ch>
**********************************************************************


#define MY_HEADSEP      translate_charset(__CHARSET__,host_charset(),"���")
#define MY_COLSEP       translate_charset(__CHARSET__,host_charset()," � ")
**********************************************************************

memvar __a2_data, curr_record,__userfunc
**********************************************************************
#include <a2edit/static_ApplyKey.prg>
#include <a2edit/static_FancyColors.prg>
#include <a2edit/static_Skipper.prg>
#include <a2edit/static_StockBrowseNew.prg>
**********************************************************************
#include <a2edit/__a2_edit_get_a2_data.prg>
#include <a2edit/a2edit.prg>
**********************************************************************
