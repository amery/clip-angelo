/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************
#include <ci_Set.ch>
#include <ci_Inkey.ch>
#include <ci_Getexit.ch>
#include <ci_config.ch>
**********************************************************

#define K_UNDO          K_CTRL_U

#ifdef LANG_RUSSIAN
      #define MSG_INSERT     "���"
      #define MSG_OVERWRITE  "���"
      #define MSG_DATE_ERROR "������ ���� "
      #define MSG_RANGE_ERROR "��������: "
#else
      #define MSG_INSERT    "Ins"
      #define MSG_OVERWRITE "Ovr"
      #define MSG_DATE_ERROR " Date error "
      #define MSG_RANGE_ERROR "In range: "
#endif

// ���������� SCOREBOARD - ������� ��������� �� ������
#define SCORE_ROW		0
#define SCORE_COL		60

// ���������� ���������� ��������� ��� ��������� READ
**********************************************************
static status
static __getlist
**********************************************************
#include <getsys50/static___getSysInit.prg>
#include <getsys50/static_ClearGetSysVars.prg>
#include <getsys50/static_DateMsg.prg>
#include <getsys50/static_GetReadVar.prg>
#include <getsys50/static_PostActiveGet.prg>
#include <getsys50/static_RestoreGetSysVars.prg>
#include <getsys50/static_SaveGetSysVars.prg>
#include <getsys50/static_Settle.prg>
#include <getsys50/static_ShowScoreboard.prg>
**********************************************************
#include <getsys50/__KillRead.prg>
#include <getsys50/__SetFormat.prg>
#include <getsys50/countGets.prg>
#include <getsys50/currentGet.prg>
#include <getsys50/GetActive.prg>
#include <getsys50/GetApplyKey.prg>
#include <getsys50/GetDoSetKey.prg>
#include <getsys50/getFldCol.prg>
#include <getsys50/getFldRow.prg>
#include <getsys50/getFldVar.prg>
#include <getsys50/GetPostValidate.prg>
#include <getsys50/GetPreValidate.prg>
#include <getsys50/GetReader.prg>
#include <getsys50/getSysInit.prg>
#include <getsys50/RangeCheck.prg>
#include <getsys50/ReadExit.prg>
#include <getsys50/ReadInsert.prg>
#include <getsys50/ReadModal.prg>
#include <getsys50/restGets.prg>
#include <getsys50/saveGets.prg>
#include <getsys50/Updated.prg>
**********************************************************
