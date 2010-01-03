/*
	COBrA is CLIP Object Broker & Application Server.
	cobraService - class for connection and dialog client <-> server
*/
/*   Copyright (C) 2003  ITK
     Author  : Uri (uri@itk.ru)
     Licence : (GPL) http://www.itk.ru/clipper/licence.html
*/

#include "ci_set.ch"
#include "ci_cobra.ch"
#include "ci_tcp.ch"
******************************************************************

#include "cobra_service/static___check_args.ch"
#include "cobra_service/static___check_clnt_info.ch"
#include "cobra_service/static___isanswer.ch"
#include "cobra_service/static___isquery.ch"
#include "cobra_service/static___run_auth.ch"
#include "cobra_service/static___run_codbaddobj.ch"
#include "cobra_service/static___run_codbdelobj.ch"
******************************************************************
*#include "cobra_service/static___run_codbgetnobj.ch"
static function __run_codbGetNObj(qArgs,oAnswer)
	local i,tmp
	if ! __check_args(qArgs,1,{"A"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_getValue")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	oAnswer:return := {}
	for i=1 to len(qArgs[1])
		tmp := clip("codb_cacheGet",qArgs[1][i])
		if tmp == NIL
			tmp := clip("codb_getValue",qArgs[1][i])
		endif
		if tmp != NIL
			clip("codb_cacheAdd",tmp:id,tmp)
		endif
		aadd(oAnswer:return,tmp)
	next
return
******************************************************************
*#include "cobra_service/static___run_codbgetobj.ch"
static function __run_codbGetObj(qArgs,oAnswer)
	local tmp
	if ! __check_args(qArgs,1,{"C"})
		oAnswer:errno := COBRA_ERR_BADARG
		return
	endif
	if !isFunction("codb_getValue")
		oAnswer:errno := COBRA_ERR_CODB
		oAnswer:error := [CODB library not linked]
		return
	endif
	tmp := clip("codb_cacheGet",qArgs[1])
	if tmp == NIL
		tmp := clip("codb_getValue",qArgs[1])
	endif
	if !empty(tmp)
		clip("codb_cacheAdd",tmp:id,tmp)
	endif
	oAnswer:return := tmp
return
******************************************************************
#include "cobra_service/static___run_codbmetabyname.ch"
#include "cobra_service/static___run_codbselect.ch"
#include "cobra_service/static___run_codbupdobj.ch"
#include "cobra_service/static___run_codbunify.ch"
#include "cobra_service/static___run_getfile.ch"
#include "cobra_service/static___run_getmyfile.ch"
#include "cobra_service/static_cobra_answersend.ch"
#include "cobra_service/static_cobra_answerwait.ch"
#include "cobra_service/static_cobra_close.ch"
#include "cobra_service/static_cobra_connect.ch"
#include "cobra_service/static_cobra_errorblock.ch"
#include "cobra_service/static_cobra_queryasend.ch"
#include "cobra_service/static_cobra_querydelete.ch"
#include "cobra_service/static_cobra_querynew.ch"
#include "cobra_service/static_cobra_queryrun.ch"
#include "cobra_service/static_cobra_querysend.ch"
#include "cobra_service/static_cobra_receive.ch"
#include "cobra_service/static_cobra_receiveline.ch"
#include "cobra_service/static_cobra_send.ch"
#include "cobra_service/static_cobra_sendline.ch"
#include "cobra_service/static_cobra_setrootpath.ch"
******************************************************************

#include "cobra_service/cobraservicenew.ch"
******************************************************************
