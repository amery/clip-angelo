/*
   Copyright (C) 2001  ITK
   Authors  : Uri Hnykin <uri@itk.ru>, Paul Lasarev <paul@itk.ru>, and other
   License : (GPL) http://www.itk.ru/clipper/license.html

	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

*/
#include <ci_clipcfg.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#ifdef OS_MINGW
#include <ltdl.h>
#else
#include <dlfcn.h>
#endif
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>

#include <ci_clip.h>
#include <ci_clipvm.h>
#include <ci_hash.h>
#include <../libclipscreen/ci_screen.h>
#include <ci_error.ch>
#include <ci_hashcode.h>
#include <ci_rational.h>
#include <../libclipscreen/ci_charset.h>
#include <ci_coll.h>
#include <license.h>

#ifdef USE_TASKS
#include <../libcliptask/ci_task.h>
#endif

#define NEW(type) ((type*)calloc(sizeof(type),1))
#define NEWVECT(type,len) ((type*)calloc(sizeof(type),(len)))
#define VAR(type,var,ini) type *var=(type*)(ini)
#define NEWVAR(type,var) type *var=(type*)calloc(sizeof(type),1)
#define c_DELETE(type,var)	{destroy_##type(var);free(var);}

#define EXPAND_MACRO

#define MIN_NUMERIC_DIFF 0.00000000000001

extern CLIP_DLLEXPORT void _clip_close_all(ClipMachine * ClipMachineMemory);

extern int clip_RDDCLOSEALL(ClipMachine * ClipMachineMemory);

#ifdef FORCEALIGN

static long
GETLONG(void *p)
{
   long r;

   memcpy(&r, p, sizeof(r));
   return r;
}

static void
SETLONG(void *ptr, long l)
{
   memcpy(ptr, &l, sizeof(l));
}

#else

#define GETLONG(p) (*(long*)(p))
#define SETLONG(ptr,l) (*(long*)(ptr)=(l))

#endif

static ClipInitStruct *init_struct = 0;

char *_clip_hostcs = "CP437";

CLIP_DLLEXPORT void
_clip_init_struct(ClipInitStruct * sp)
{
   if (!init_struct)
      init_struct = sp;
}

static const char __file__[] = __FILE__;

int search_map(ClipVarEl * items, int count, long key, int *index);

static ClipVarEl *get_map_el(ClipMapVar * a, long no);

static void remove_private_vect(ClipMachine * ClipMachineMemory, int num, void *vect);

static int descr_str(ClipMachine * ClipMachineMemory, ClipVar * vp, char **str);

static void delete_ClipVarFrame(ClipMachine * ClipMachineMemory, ClipVarFrame * localvars);

char *CLIPROOT = CLIP_ROOT;

char *CLIP_LOCALE_ROOT = CLIP_ROOT;

static VarEntry *add_private(ClipMachine * ClipMachineMemory, long hash);

static VarEntry *fetch_var(ClipMachine * ClipMachineMemory, long hash);

static void dup_ref(ClipVar * dest, ClipVar * src);

static ClipVar *clip_fetch(ClipMachine * ClipMachineMemory, int dim, int push, int store, ClipVar ** mapp, long *hashp);

int _clip_profiler = 0;

static void fini_profiler(ClipMachine * ClipMachineMemory);

/* [ ClipBuf */
#include <clipruntime/destroy_ClipBuf.c>
#include <clipruntime/setLen_ClipBuf.c>
#include <clipruntime/_clip_type.c>
#include <clipruntime/_clip_type_name.c>
#include <clipruntime/_clip_vptr.c>
/* ] ClipBuf */
/* [ ClipVect */
#include <clipruntime/destroy_ClipVect.c>
#include <clipruntime/add_ClipVect.c>
#include <clipruntime/clear_ClipVect.c>
#include <clipruntime/remove_ClipVect.c>
#include <clipruntime/insert_ClipVect.c>
/* ] ClipVect */
/* [ ClipMachine */

static int machineCount = 0;

#include <clipruntime/typedef_struct_FuncEntry.h>

static ClipMachine *cur_mp = 0;

static ClipMachine *first_mp = 0;

#include <clipruntime/_clip_init_tty.c>
#include <clipruntime/_clip_restore_tty.c>

static int errorlevel = 0;	/* result of errorlevel() */

static HashTable *all_publics = 0;

static HashTable *all_hashnames = 0;

static HashTable *all_functions = 0;

static HashTable *all_aliases = 0;

static HashTable *all_fields = 0;

static HashTable *all_keys = 0;

static HashTable *all_store = 0;

/*
static ClipVect *all_areas = 0;
static ClipVect *all_areaStack = 0;
*/

static Container *all_container = 0;

static DBDriver *all_dbdrivers = 0;

struct _RDD_DATA_VTBL_ **all_data_drivers;

struct _RDD_INDEX_VTBL_ **all_idx_drivers;

struct _RDD_MEMO_VTBL_ **all_memo_drivers;

static int all_ndbdrivers = 0;

static int all_ndata_drivers = 0;

static int all_nidx_drivers = 0;

static int all_nmemo_drivers = 0;

static char all_def_data_driver[6];

static char all_def_idx_driver[6];

static char all_def_memo_driver[6];

static char all_def_db_driver[9];

static SQLDriver *all_sqldrivers = 0;

static int all_nsqldrivers = 0;

static HashTable *all_tasklocks = 0;

static HashTable *all_fileopens = 0;

/*static ClipWindow *all_windows = 0;*/

static int *all_kbdbuf;

static int *all_kbdptr;

int _clip_sig_flag = 0;

void _clip_signal_real(int sig);

#include <clipruntime/new_ClipMachine.c>

char *_clip_progname = 0;

int _clip_argc = 0;

char **_clip_argv = 0;

char **_clip_envp = 0;

int log_level = 1;

FILE *logg = 0;

static int exited = 0;

int _clip_raw_argc = 0;

char **_clip_raw_argv = 0;

#include <clipruntime/_clip_exit.c>

#define M_OFFS(base,nl,ns) ((base)+8+nl*sizeof(long)+ns*sizeof(short))

#include <clipruntime/static_get_env.c>
#include <clipruntime/_clip_getenv.c>
#include <clipruntime/static_put_env.c>
#include <clipruntime/_clip_putenv.c>
#include <clipruntime/_clip_put_env.c>
#include <clipruntime/static_set_sysvars.c>
#include <clipruntime/static_process_environ.c>

int _clip_debuglevel = 0;

#include <clipruntime/static_debug_enable.c>
#include <clipruntime/static_license_on_start.c>
#include <clipruntime/static_profile_enable.c>

static int need_stop = 0;

#include <clipruntime/static_breakpoint_on_start.c>
#include <clipruntime/struct_OptionAction.h>

static struct OptionAction hidden_options[] = {
   {"--debug", debug_enable},
   {"--profile", profile_enable},
   {"--stop", breakpoint_on_start},
   {"--license", license_on_start},
};

#define HIDDEN_NUM (sizeof(hidden_options)/sizeof(struct OptionAction))

#include <clipruntime/static_process_args.c>
#include <clipruntime/_clip_init.c>
#include <clipruntime/_clip_register_file.c>
#include <clipruntime/_clip_log_init.c>
#include <clipruntime/_clip_loglevel.c>
#include <clipruntime/_clip_out_log.c>
#include <clipruntime/_clip_flush_log.c>
#include <clipruntime/_clip_logg.c>
#include <clipruntime/_clip_signal.c>
#include <clipruntime/_clip_signal_real.c>
#include <clipruntime/cur_ClipMachine.c>
#include <clipruntime/static_new_VarEntry.c>
#include <clipruntime/static_delete_VarEntry.c>
#include <clipruntime/static_delete_VarTable.c>
#include <clipruntime/static_clear_VarTable.c>
#include <clipruntime/static_delete_spaces.c>
#include <clipruntime/static_delete_Functions.c>
#include <clipruntime/delete_ClipMachine.c>
#include <clipruntime/_clear_trap.c>
#include <clipruntime/_clip_log_call.c>
#include <clipruntime/_clip_errorblock.c>
#include <clipruntime/_clip_errorblock_res.c>
#include <clipruntime/static_do_call.c>
#include <clipruntime/static_init_profiler.c>
#include <clipruntime/static_find_profile_bucket_name.c>
#include <clipruntime/static_find_profile_bucket.c>
#ifndef timer_add
#define	timer_add(a, b, result)							  \
  do {										  \
	(result)->tv_sec = (a)->tv_sec + (b)->tv_sec;				  \
	(result)->tv_usec = (a)->tv_usec + (b)->tv_usec;				  \
	if ((result)->tv_usec >= 1000000)						  \
	  {										  \
	++(result)->tv_sec;							  \
	(result)->tv_usec -= 1000000;						  \
	  }										  \
  } while (0)
#endif

#ifndef timer_sub
#define timer_sub(a, b, result) \
   do {                         \
	   (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;     \
	   (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;  \
	   if ((result)->tv_usec < 0) {                      \
		  --(result)->tv_sec;                        \
		  (result)->tv_usec += 1000000;              \
	   }                                                 \
   } while (0)
#endif

#include <clipruntime/static_start_profiler.c>
#include <clipruntime/_clip_start_profiler.c>
#include <clipruntime/_clip_start_profiler_name.c>
#include <clipruntime/static_stop_profiler.c>
#include <clipruntime/_clip_stop_profiler.c>
#include <clipruntime/_clip_stop_profiler_name.c>
#include <clipruntime/static_cmp_profile_out.c>
#include <clipruntime/static_fini_profiler.c>
#include <clipruntime/static__clip_refclone.c>
#include <clipruntime/_clip_eval.c>
#include <clipruntime/clip_CLIP.c>
#include <clipruntime/clip_CLIPA.c>
#include <clipruntime/_clip_clip.c>
#include <clipruntime/_clip_push_func.c>
#include <clipruntime/_clip_func.c>
#include <clipruntime/_clip_func_name.c>
#include <clipruntime/static_init_machine.c>
#include <clipruntime/_clip_proc.c>
#include <clipruntime/_clip_register.c>
#include <clipruntime/_clip_register_hash.c>
#include <clipruntime/_clip_register_block.c>
#include <clipruntime/_clip_get_function.c>
#include <clipruntime/_clip_call_errblock.c>
#include <clipruntime/_clip_func_hash.c>
#include <clipruntime/_clip_proc_hash.c>
#include <clipruntime/_clip_return.c>
#include <clipruntime/_clip_pop.c>
#include <clipruntime/static_delete_ClipVarFrame.c>
#include <clipruntime/_clip_destroy_locals.c>

#define MAX_LOOP_DEEP 1024

#include <clipruntime/static_calc_loopcount.c>
#include <clipruntime/_clip_add_tempvar.c>
#include <clipruntime/_clip_resume.c>
#include <clipruntime/_clip_vresume.c>
#include <clipruntime/_clip_delete.c>
#include <clipruntime/_clip_destroy.c>
#include <clipruntime/_clip_ref_destroy.c>
#include <clipruntime/type_weight.c>
#include <clipruntime/_clip_cmp.c>
#include <clipruntime/_clip_strstr.c>
#include <clipruntime/static_do_assign.c>
#include <clipruntime/_clip_op.c>
#include <clipruntime/_clip_not.c>
#include <clipruntime/_clip_minus.c>
#include <clipruntime/_clip_opassign.c>
#include <clipruntime/_clip_opiassign.c>
#include <clipruntime/_clip_assign.c>
#include <clipruntime/_clip_refassign.c>
#include <clipruntime/_clip_iassign.c>
#include <clipruntime/static_find_lval.c>
#include <clipruntime/_clip_macroassign.c>
#include <clipruntime/_clip_pop_fieldhash.c>
#include <clipruntime/_clip_cond.c>
#include <clipruntime/_clip_tcond.c>
#include <clipruntime/_clip_memdup.c>
#include <clipruntime/_clip_mclone.c>
#include <clipruntime/_clip_clone.c>
#include <clipruntime/_clip_dup.c>
#include <clipruntime/_clip_push.c>
#include <clipruntime/_clip_push_nil.c>
#include <clipruntime/_clip_push_true.c>
#include <clipruntime/_clip_push_false.c>
#include <clipruntime/_clip_push_static.c>
#include <clipruntime/_clip_push_hash.c>
#include <clipruntime/_clip_ref.c>
#include <clipruntime/_clip_hash.c>
#include <clipruntime/static_calc_casehash.c>
#include <clipruntime/_clip_casehash.c>
#include <clipruntime/_clip_pop_hash.c>
#include <clipruntime/_clip_pop_shash.c>
/* ] ClipMachine */
/* [ param */
#include <clipruntime/_clip_param.c>
#include <clipruntime/_clip_ref_param.c>
/* ] param */
/* [ memvars */
#include <clipruntime/_clip_memvar_public.c>
#include <clipruntime/_clip_add_private.c>
#include <clipruntime/_clip_memvar_private.c>
#include <clipruntime/_clip_is_public.c>
#include <clipruntime/_clip_is_private.c>
#include <clipruntime/_clip_is_memvar.c>
#include <clipruntime/_clip_space.c>
#include <clipruntime/_clip_memvar_space.c>
#include <clipruntime/_clip_memvar_param.c>
#include <clipruntime/static_remove_private_vect.c>
#include <clipruntime/_clip_remove_privates.c>
#include <clipruntime/_clip_vremove_privates.c>
#include <clipruntime/static_add_private.c>
#include <clipruntime/static_fetch_var.c>
#include <clipruntime/static_fetch_obj.c>
#include <clipruntime/_clip_memvar.c>
#include <clipruntime/_clip_fmemvar.c>
#include <clipruntime/_clip_memvarf.c>
#include <clipruntime/_clip_ref_fmemvar.c>
#include <clipruntime/_clip_ref_memvar.c>
#include <clipruntime/_clip_ref_memvar_noadd.c>
#include <clipruntime/_clip_public.c>
#include <clipruntime/_clip_ref_public.c>
#include <clipruntime/_clip_ref_public_noadd.c>
/* ] memvars */
/* [ traps */
#include <clipruntime/_clip_trap.c>
#include <clipruntime/_clip_trap_pop.c>
#include <clipruntime/_clip_trap_str.c>
#include <clipruntime/_clip_trap_operation.c>
#include <clipruntime/_clip_trap_printf.c>
#include <clipruntime/_clip_trap_printv.c>
#include <clipruntime/_clip_trap_var.c>
#include <clipruntime/_clip_trap_invargv.c>
/* ] traps */
/* [ arrays */
#include <clipruntime/_clip_sarray.c>
#include <clipruntime/static_new_array.c>
#include <clipruntime/_clip_dimarray.c>
#include <clipruntime/_clip_newarray.c>
#include <clipruntime/_clip_vnewarray.c>
#include <clipruntime/_clip_array.c>
#include <clipruntime/_clip_adel.c>
#include <clipruntime/_clip_asize.c>
#include <clipruntime/_clip_ains.c>
#include <clipruntime/_clip_aadd.c>
#include <clipruntime/_clip_aref.c>
#include <clipruntime/_clip_aget.c>
#include <clipruntime/_clip_aset.c>
#include <clipruntime/_clip_unref_arr.c>
#include <clipruntime/static_print_dim.c>
#include <clipruntime/search_map.c>
#include <clipruntime/static_get_map_el.c>
#include <clipruntime/_clip_map.c>
#include <clipruntime/_clip_mputn.c>
#include <clipruntime/_clip_mputl.c>
#include <clipruntime/_clip_mputc.c>
#include <clipruntime/_clip_madd.c>
#include <clipruntime/_clip_mdel.c>
#include <clipruntime/_clip_mind.c>
#include <clipruntime/static_test_maponerror.c>
#include <clipruntime/static_fetch_arr.c>
#include <clipruntime/static_clip_count.c>
#include <clipruntime/static_clip_fetch.c>
#include <clipruntime/_clip_ifetch.c>
#include <clipruntime/_clip_fetch.c>
#include <clipruntime/_clip_fetchref.c>
#include <clipruntime/static_test_modify.c>
#include <clipruntime/_clip_istore.c>
#include <clipruntime/_clip_store.c>
#include <clipruntime/_clip_opistore.c>
#include <clipruntime/_clip_opstore.c>
/* ] arrays */
/* [ var */
#include <clipruntime/_clip_log.c>
#include <clipruntime/_clip_long.c>
#include <clipruntime/_clip_double.c>
#include <clipruntime/static_descr_str.c>
#include <clipruntime/_clip_strFromVar.c>
#include <clipruntime/_clip_eval_macro.c>
#include <clipruntime/_clip_expand_var.c>
#include <clipruntime/static_find_macro.c>
#include <clipruntime/static_octdig.c>
#include <clipruntime/static_hexdig.c>
#include <clipruntime/_clip_expand.c>
#include <clipruntime/_clip_macro.c>
#include <clipruntime/_clip_var_str.c>
#include <clipruntime/_clip_var_num.c>
#include <clipruntime/_clip_var_log.c>
/* ] var */
/* [ pcount */
#include <clipruntime/_clip_pcount.c>
#include <clipruntime/_clip_pshift.c>
#include <clipruntime/_clip_parn.c>
/* ] pcount */
/* [ Object calls */

static int obj_call(struct ClipMachine *ClipMachineMemory, int argc, long hash, int call);

#define CALL_CALL 0
#define CALL_GET 1
#define CALL_SET 2

#include <clipruntime/_clip_call.c>
#include <clipruntime/static_obj_call.c>
#include <clipruntime/_clip_get.c>
#include <clipruntime/_clip_set.c>
/* ] Object calls */
/* [ OutBuf */
#include <clipruntime/init_Buf.c>
#include <clipruntime/destroy_Buf.c>
#include <clipruntime/static_grow_buf.c>
#include <clipruntime/putByte_Buf.c>
#include <clipruntime/putBuf_Buf.c>
#include <clipruntime/putShort_Buf.c>
#include <clipruntime/putLong_Buf.c>
#include <clipruntime/putInt32_Buf.c>
#include <clipruntime/putDouble_Buf.c>
#include <clipruntime/putStr_Buf.c>
/* ] OutBuf */
/* [ utils */
#include <clipruntime/_clip_cdate.c>
#include <clipruntime/_clip_jdate.c>
#include <clipruntime/_clip_parinfo.c>
#include <clipruntime/_clip_parni.c>
#include <clipruntime/_clip_parnl.c>
#include <clipruntime/_clip_par.c>
#include <clipruntime/_clip_spar.c>
#include <clipruntime/_clip_par_isref.c>
#include <clipruntime/_clip_par_assign.c>
#include <clipruntime/_clip_par_assign_str.c>
#include <clipruntime/_clip_par_assign_str_m.c>
#include <clipruntime/_clip_parnd.c>
#include <clipruntime/_clip_parp.c>
#include <clipruntime/_clip_parl.c>
#include <clipruntime/_clip_parcl.c>
#include <clipruntime/_clip_parc.c>
#include <clipruntime/_clip_retndp.c>
#include <clipruntime/_clip_retni.c>
#include <clipruntime/_clip_retnl.c>
#include <clipruntime/_clip_retnd.c>
#include <clipruntime/_clip_retnr.c>
#include <clipruntime/_clip_retl.c>
#include <clipruntime/_clip_retcn.c>
#include <clipruntime/_clip_retcn_m.c>
#include <clipruntime/_clip_retc.c>
#include <clipruntime/_clip_storclen.c>
#include <clipruntime/_clip_storc.c>
#include <clipruntime/_clip_stornd.c>
#include <clipruntime/_clip_storni.c>
#include <clipruntime/_clip_stornl.c>
#include <clipruntime/_clip_storl.c>
#include <clipruntime/static_incr_var.c>
#include <clipruntime/_clip_incr.c>
#include <clipruntime/_clip_unptr.c>
#include <clipruntime/_clip_find_obj.c>
#include <clipruntime/static_do_main.c>
#include <clipruntime/_clip_main.c>
#include <clipruntime/_clip_main_func.c>
#include <clipruntime/_clip_main_code.c>
#include <clipruntime/_clip_typename.c>
/* ] utils */
#include <clipruntime/static_cmp_hash_bucket.c>
#include <clipruntime/_clip_hash_name.c>
#include <clipruntime/_clip_pardj.c>
#include <clipruntime/_clip_pardtj.c>
#include <clipruntime/_clip_pardc.c>
#include <clipruntime/_clip_retdj.c>
#include <clipruntime/_clip_retdtj.c>
#include <clipruntime/_clip_retdc.c>
#include <clipruntime/_clip_stordj.c>
#include <clipruntime/_clip_stordc.c>

#define DD_BUF_LEN 	7

#include <clipruntime/_clip_str_to_date.c>
#include <clipruntime/_clip_datetostr.c>
#include <clipruntime/_clip_date_to_str.c>
#include <clipruntime/_clip_glob_match.c>
#include <clipruntime/typedef_struct_item_bucket.h>
#include <clipruntime/_clip_fetch_item.c>
#include <clipruntime/_clip_store_item.c>
#include <clipruntime/_clip_store_item_destroy.c>
#include <clipruntime/_clip_free_item.c>
#include <clipruntime/_clip_remove_item.c>
/*
 * CONTAINER
 */
#include <clipruntime/_clip_compare_c_item.c>
#include <clipruntime/_clip_compare_c_item_type.c>
#include <clipruntime/_clip_fetch_c_item.c>
#include <clipruntime/_clip_fetch_c_item_type.c>
#include <clipruntime/_clip_destroy_c_item.c>
#include <clipruntime/_clip_store_c_item.c>
/*
 * CONTAINER end
*/
#include <clipruntime/static__clip_normalize_path.c>
#include <clipruntime/_clip_translate_path.c>
#include <clipruntime/_clip_mptr.c>
#include <clipruntime/_clip_new_error.c>
#include <clipruntime/_clip_errname.c>
#include <clipruntime/_clip_generr.c>
#include <clipruntime/_clip_trap_err.c>
#include <clipruntime/_clip_mget.c>
#include <clipruntime/_clip_mtype.c>
#include <clipruntime/_clip_mgetn.c>
#include <clipruntime/_clip_mgetl.c>
#include <clipruntime/_clip_mgetc.c>
#include <clipruntime/_clip_upstr.c>
#include <clipruntime/_clip_toupper.c>
#include <clipruntime/_clip_tolower.c>
#include <clipruntime/_clip_calc_hash.c>
#include <clipruntime/_clip_calc_hash2.c>
#include <clipruntime/_clip_math_operation.c>
#include <clipruntime/_clip_forstep.c>
#include <clipruntime/_clip_map_first.c>
#include <clipruntime/_clip_map_next.c>
#include <clipruntime/_clip_swap.c>
#include <clipruntime/_clip_push_locale.c>
#include <clipruntime/static_local_ref.c>
#include <clipruntime/_clip_push_local.c>
#include <clipruntime/_clip_ref_local.c>
#include <clipruntime/_clip_pop_ref.c>
#include <clipruntime/_clip_unref.c>
#include <clipruntime/_clip_refmacro.c>
#include <clipruntime/static_dup_ref.c>
#include <clipruntime/_clip_get_fielddef.c>
#include <clipruntime/static_choose_quot.c>
#include <clipruntime/_clip_catstr.c>
#include <clipruntime/_clip_quot.c>
#include <clipruntime/_clip_strcmp.c>
#include <clipruntime/_clip_strncmp.c>
#include <clipruntime/_clip_strnncmp.c>
#include <clipruntime/_clip_strcasecmp.c>
#include <clipruntime/_clip_strncasecmp.c>
#include <clipruntime/_clip_strnncasecmp.c>
#include <clipruntime/_clip_unix_path.c>
#include <clipruntime/_clip_absolute_path.c>
#include <clipruntime/_clip_path.c>
#include <clipruntime/typedef_struct_TranslateTable.h>
#include <clipruntime/static_cmp_TranslateTable.c>
#include <clipruntime/static_delete_TranslateTable.c>

static Coll *tr_charsets = 0;

#include <clipruntime/static_pg2pgch.c>
#include <clipruntime/_clip_translate_charset.c>
#include <clipruntime/clip_ALTD.c>
#include <clipruntime/_clip_check_stack.c>
#include <clipruntime/_clip_host_charset.c>
#include <clipruntime/clip_ERRORLEVEL.c>
#include <clipruntime/clip___QUIT.c>
