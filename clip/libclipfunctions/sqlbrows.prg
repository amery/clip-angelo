**************************************************************************
**************************************************************************
#include <ci_box.ch>
#include <ci_Inkey.ch>
#include <ci_Setcurs.ch>
#include <ci_Error.ch>
#include <ci_config.ch>
**************************************************************************
#define MY_HEADSEP      translate_charset(__CHARSET__,host_charset(),"��")
#define MY_COLSEP       translate_charset(__CHARSET__,host_charset(),"� ")

#define MSG_RECORDS [Record N ]
**************************************************************************
**************************************************************************
#include <sqlbrows/static_ApplyKey.prg>
#include <sqlbrows/static_DoGet.prg>
#include <sqlbrows/static_FancyColors.prg>
#include <sqlbrows/static_InsToggle.prg>
#include <sqlbrows/static_Skipper.prg>
**************************************************************************
#include <sqlbrows/SQLBrowse.prg>
**************************************************************************
