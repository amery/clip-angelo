/*
    Copyright (C) 2002  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html

    class INIFILE
    control for Windows-like .ini files

   :) �������� �� ��������� ����� ��������� ��� ��������, ���� ��
   ���� ����� �������� ������� ������ :)
*/
**********************************************************************
#include "ci_fileio.ch"
**********************************************************************
#define DOS_CRLF "&\r&\n"
**********************************************************************
#include "inifile/static___check_name.ch"
#include "inifile/static___IF_getValue.ch"
#include "inifile/static___IF_keys.ch"
#include "inifile/static___IF_load.ch"
#include "inifile/static___IF_new.ch"
#include "inifile/static___IF_save.ch"
#include "inifile/static___IF_sections.ch"
#include "inifile/static___IF_setValue.ch"
#include "inifile/static___stringToData.ch"
#include "inifile/static___xDataToString.ch"
**********************************************************************
#include "inifile/_recover_iniFile.ch"
#include "inifile/iniFile.ch"
#include "inifile/iniFileNew.ch"
**********************************************************************
