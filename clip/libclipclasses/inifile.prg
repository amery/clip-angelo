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
#include <ci_fileio.ch>
**********************************************************************
#define DOS_CRLF "&\r&\n"
**********************************************************************
#include <inifile/static___check_name.prg>
#include <inifile/static___IF_getValue.prg>
#include <inifile/static___IF_keys.prg>
#include <inifile/static___IF_load.prg>
#include <inifile/static___IF_new.prg>
#include <inifile/static___IF_save.prg>
#include <inifile/static___IF_sections.prg>
#include <inifile/static___IF_setValue.prg>
#include <inifile/static___stringToData.prg>
#include <inifile/static___xDataToString.prg>
**********************************************************************
#include <inifile/_recover_iniFile.prg>
#include <inifile/iniFile.prg>
#include <inifile/iniFileNew.prg>
**********************************************************************
