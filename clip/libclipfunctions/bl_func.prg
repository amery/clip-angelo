**********************************************************************
// BL Interpreter
#ifdef FSHIP
#include "ci_inkey.fh"
#else
#include "ci_inkey.ch"
#endif
**********************************************************************
#define comm_char '//'
#ifdef FSHIP
#define x_EOL (chr(10))
#else
	#ifdef __CLIP__
		#define x_EOL (chr(10))
	#else
		#define x_EOL (chr(13)+chr(10))
	#endif
#endif
#define x_comment -2
#define x_eval -1
#define x_if 0
#define x_else 1
#define x_endif 2
#define x_while 3
#define x_endw 4
#define x_for 5
#define x_next 6
#define x_private 7
#define x_public 8
#define x_release 9
#define x_exit 10
#define x_loop 11
#define x_proc 12
#define x_endp 13
#define x_call 14
#define x_write 15
#define x_return 16
#define x_push 17
#define x_pop 18
#define x_param 19
#define x_block 20
#define x_elif 21
#define x_loadlib 22

#ifdef __CLIP__
	#define myalert  alert
#else
	#translate myalert => alert
#endif
#ifdef FSHIP
#include "error.fh"
#else
#include "ci_error.ch"
#endif
#define NTRIM(n) RTRIM(Str(n))
**********************************************************************
memvar aCommStack,blStack
static blprocs:={}
**********************************************************************
#include "bl_func/static_apop.ch"
#include "bl_func/static_apush.ch"
#include "bl_func/static_atop.ch"
#include "bl_func/static_block.ch"
#include "bl_func/static_chkstack.ch"
#include "bl_func/static_clmacro.ch"
#include "bl_func/static_commparse.ch"
#include "bl_func/static_e_eval.ch"
#include "bl_func/static_el_eval.ch"
#include "bl_func/static_elif_eval.ch"
#include "bl_func/static_elif_pc.ch"
#include "bl_func/static_endif_eval.ch"
#include "bl_func/static_endw_eval.ch"
#include "bl_func/static_ErrorMessage.ch"
#include "bl_func/static_exit_eval.ch"
#include "bl_func/static_for_eval.ch"
#include "bl_func/static_if_eval.ch"
#include "bl_func/static_lineparse.ch"
#include "bl_func/static_loadprocs.ch"
#include "bl_func/static_loop_eval.ch"
#include "bl_func/static_mkblk.ch"
#include "bl_func/static_mkblkerr.ch"
#include "bl_func/static_next_eval.ch"
#include "bl_func/static_progerr.ch"
#include "bl_func/static_pub_eval.ch"
#include "bl_func/static_realrun.ch"
#include "bl_func/static_rel_eval.ch"
#include "bl_func/static_scantok.ch"
#include "bl_func/static_while_eval.ch"
**********************************************************************
#include "bl_func/progparse.ch"
#include "bl_func/progrun.ch"
**********************************************************************
