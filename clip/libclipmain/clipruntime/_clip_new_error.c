CLIP_DLLEXPORT ClipVar *
_clip_new_error(ClipMachine * ClipMachineMemory,
		const char *description, int severity, int genCode, int osCode, int canDefault, int canRetry, int canSubstitute, int tries, const char *subSystem, int subCode, const char *operation)
{
   ClipVar *rp = NEW(ClipVar);

   ClipVar var;

   char *s;

   long dim;

   int i;

   _clip_map(ClipMachineMemory, rp);

   memset(&var, 0, sizeof(var));

   /* 'cargo' */

   dim = ClipMachineMemory->argc;
   _clip_array(ClipMachineMemory, &var, 1, &dim);
   for (i = 0; i < ClipMachineMemory->argc; i++)
   {
      dim = i;
      _clip_aset(ClipMachineMemory, &var, ARGPTR(ClipMachineMemory, i + 1), 1, &dim);
   }
   _clip_madd(ClipMachineMemory, rp, HASH_args, &var);
   _clip_destroy(ClipMachineMemory, &var);

   _clip_var_str(description, strlen(description), &var);
   _clip_madd(ClipMachineMemory, rp, HASH_description, &var);
   _clip_destroy(ClipMachineMemory, &var);

   _clip_var_num(severity, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_severity, &var);
   _clip_destroy(ClipMachineMemory, &var);

   _clip_var_num(genCode, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_genCode, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'osCode' */
   _clip_var_num(osCode, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_osCode, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'canDefault' */
   _clip_var_log(canDefault, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_canDefault, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'canRetry' */
   _clip_var_log(canRetry, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_canRetry, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'canSubstitute' */
   _clip_var_log(canSubstitute, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_canSubstitute, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'tries' */
   _clip_var_num(tries, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_tries, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'filename' */
   if (ClipMachineMemory->fp)
      s = (char *) ClipMachineMemory->fp->filename_of_ClipFrame;
   else
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMachineMemory, rp, HASH_filename, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'operation' */
   s = (char *) operation;
   if (!s)
      s = (char *) ClipMachineMemory->fp->procname_of_ClipFrame;
   if (!s)
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMachineMemory, rp, HASH_operation, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'subSystem' */
   s = (char *) subSystem;
   if (!s)
      s = "";
   _clip_var_str(s, strlen(s), &var);
   _clip_madd(ClipMachineMemory, rp, HASH_subSystem, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'subCode' */
   _clip_var_num(subCode, &var);
   _clip_madd(ClipMachineMemory, rp, HASH_subCode, &var);
   _clip_destroy(ClipMachineMemory, &var);

   /* 'syserr' */
   _clip_var_str(ClipMachineMemory->syserr, strlen(ClipMachineMemory->syserr), &var);
   ClipMachineMemory->syserr[0] = 0;
   _clip_madd(ClipMachineMemory, rp, HASH_syserr, &var);
   _clip_destroy(ClipMachineMemory, &var);

   return rp;
}
