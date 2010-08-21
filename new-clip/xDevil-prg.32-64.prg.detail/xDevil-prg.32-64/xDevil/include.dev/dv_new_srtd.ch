#IfNDef DV_STD_CH
#define DV_STD_CH
#include "dv_command.ch"
#Include "dv_comment.ch"
#include "dv_set.ch"

#define  _DFSET(x, y)  Set( _SET_DATEFORMAT, iif(__SetCentury(), x, y) )

#translate millisec( <v1> )					=> sleep(<v1>/1000)

