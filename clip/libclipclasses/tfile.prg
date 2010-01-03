**********************************************************************
#include "ci_FileIO.ch"
**********************************************************************

#define SEEK_FORWARD   0
#define SEEK_BACKWARD  1

#define SEEK_NOCASE    0
#define SEEK_UPCASE    1
#define SEEK_LOCASE    2
**********************************************************************
#include "tfile/static_NoExt.ch"
#include "tfile/static_tf_AppByte.ch"
#include "tfile/static_tf_AppStr.ch"
#include "tfile/static_tf_AppWord.ch"
#include "tfile/static_tf_bof.ch"
#include "tfile/static_tf_close.ch"
#include "tfile/static_tf_Count.ch"
#include "tfile/static_tf_Create.ch"
#include "tfile/static_tf_DelBytes.ch"
#include "tfile/static_tf_eof.ch"
#include "tfile/static_tf_GetByte.ch"
#include "tfile/static_tf_GetStr.ch"
#include "tfile/static_tf_GetWord.ch"
#include "tfile/static_tf_goBottom.ch"
#include "tfile/static_tf_goto.ch"
#include "tfile/static_tf_goTop.ch"
#include "tfile/static_tf_InsBytes.ch"
#include "tfile/static_tf_new.ch"
#include "tfile/static_tf_Open.ch"
#include "tfile/static_tf_PutByte.ch"
#include "tfile/static_tf_putStr.ch"
#include "tfile/static_tf_PutWord.ch"
#include "tfile/static_tf_recno.ch"
#include "tfile/static_tf_Seek.ch"
#include "tfile/static_tf_skip.ch"
**********************************************************************
#include "tfile/TFile.ch"
**********************************************************************
