#IfNDef cn_STD_CH
#define cn_STD_CH
#include "cn_command.ch"
#Include "cn_comment.ch"
#include "cn_set.ch"

#define  _DFSET(x, y)  Set( _SET_DATEFORMAT, iif(__SetCentury(), x, y) )

#translate millisec( <v1> )					=> sleep(<v1>/1000)

