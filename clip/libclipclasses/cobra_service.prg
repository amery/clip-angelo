/*
	COBrA is CLIP Object Broker & Application Server.
	cobraService - class for connection and dialog client <-> server
*/
/*   Copyright (C) 2003  ITK
     Author  : Uri (uri@itk.ru)
     Licence : (GPL) http://www.itk.ru/clipper/licence.html
*/

#include <ci_set.ch>
#include <ci_cobra.ch>
#include <ci_tcp.ch>
******************************************************************

#include <cobra_service/static___check_args.prg>
#include <cobra_service/static___check_clnt_info.prg>
#include <cobra_service/static___isanswer.prg>
#include <cobra_service/static___isquery.prg>
#include <cobra_service/static___run_auth.prg>
#include <cobra_service/static___run_codbaddobj.prg>
#include <cobra_service/static___run_codbdelobj.prg>
******************************************************************
*#include <cobra_service/static___run_codbgetnobj.prg>
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
*#include <cobra_service/static___run_codbgetobj.prg>
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
#include <cobra_service/static___run_codbmetabyname.prg>
#include <cobra_service/static___run_codbselect.prg>
#include <cobra_service/static___run_codbupdobj.prg>
#include <cobra_service/static___run_codbunify.prg>
#include <cobra_service/static___run_getfile.prg>
#include <cobra_service/static___run_getmyfile.prg>
#include <cobra_service/static_cobra_answersend.prg>
#include <cobra_service/static_cobra_answerwait.prg>
#include <cobra_service/static_cobra_close.prg>
#include <cobra_service/static_cobra_connect.prg>
#include <cobra_service/static_cobra_errorblock.prg>
#include <cobra_service/static_cobra_queryasend.prg>
#include <cobra_service/static_cobra_querydelete.prg>
#include <cobra_service/static_cobra_querynew.prg>
#include <cobra_service/static_cobra_queryrun.prg>
#include <cobra_service/static_cobra_querysend.prg>
#include <cobra_service/static_cobra_receive.prg>
#include <cobra_service/static_cobra_receiveline.prg>
#include <cobra_service/static_cobra_send.prg>
#include <cobra_service/static_cobra_sendline.prg>
#include <cobra_service/static_cobra_setrootpath.prg>
******************************************************************

#include <cobra_service/cobraservicenew.prg>
******************************************************************
