/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************
#include "ci_Set.ch"
#include "ci_Inkey.ch"
#include "ci_Getexit.ch"
#include "ci_config.ch"
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
#include "getsys50/static___getSysInit.ch"
#include "getsys50/static_ClearGetSysVars.ch"
#include "getsys50/static_DateMsg.ch"
#include "getsys50/static_GetReadVar.ch"
#include "getsys50/static_PostActiveGet.ch"
#include "getsys50/static_RestoreGetSysVars.ch"
#include "getsys50/static_SaveGetSysVars.ch"
#include "getsys50/static_Settle.ch"
#include "getsys50/static_ShowScoreboard.ch"
**********************************************************
#include "getsys50/__KillRead.ch"
#include "getsys50/__SetFormat.ch"
#include "getsys50/countGets.ch"
#include "getsys50/currentGet.ch"
#include "getsys50/GetActive.ch"
#include "getsys50/GetApplyKey.ch"
#include "getsys50/GetDoSetKey.ch"
#include "getsys50/getFldCol.ch"
#include "getsys50/getFldRow.ch"
#include "getsys50/getFldVar.ch"
#include "getsys50/GetPostValidate.ch"
#include "getsys50/GetPreValidate.ch"
#include "getsys50/GetReader.ch"
#include "getsys50/getSysInit.ch"
#include "getsys50/RangeCheck.ch"
#include "getsys50/ReadExit.ch"
#include "getsys50/ReadInsert.ch"
#include "getsys50/ReadModal.ch"
#include "getsys50/restGets.ch"
#include "getsys50/saveGets.ch"
#include "getsys50/Updated.ch"
**********************************************************
