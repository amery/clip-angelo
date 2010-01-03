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
#include <ci_config.ch>
#include <ci_set.ch>
#include <ci_box.ch>
#include <ci_pgch.ch>
**********************************************************************

#define LIST_COLORS	"0/3,0/2,15/3,0/3,0/2,15/2,15/2"
#define DOP	replicate(" ", 256)
#define DRIVE_PATH	"/usr/home"
#define FA_NORMAL	0
#define FA_READONLY	1
#define FA_HIDDEN	2
#define FA_SYSTEM	4
#define FA_VOLUME	8
#define FA_DIRECTORY	16
#define FA_ARCHIVE	32
//#define FD_DELIM	translate_charset(__CHARSET__, host_charset(), "�")
/*#define FD_DELIM 	chr(PGCH_VLINE)*/
#define FD_DELIM	iif(set(_SET_DISPBOX),translate_charset("cp437", host_charset(), CHR(179)),chr(PGCH_VLINE))
**********************************************************************
**********************************************************************
#include <filedd/static_i_driver.ch>
#include <filedd/static_initItem.ch>
#include <filedd/static_lastdir.ch>
#include <filedd/static_showview.ch>
**********************************************************************
#include <filedd/DiskFileDialog.ch>
**********************************************************************
