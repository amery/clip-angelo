static int
do_code(ClipMachine * ClipMachineMemory, struct ClipBlock *cp, int argc, int isProc, int rest, ClipVarFrame * uplocals)
{
   int ret, _argc = ClipMachineMemory->argc;

   ClipFrame *fp = ClipMachineMemory->fp;

   ClipVar *bp = ClipMachineMemory->bp;

   ClipVarFrame *s_up = ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame;

   ClipMachineMemory->argc = argc;
   ClipMachineMemory->bp = fp->ClipVar_sp_of_ClipFrame;
   ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = uplocals;

   if (_clip_profiler)
   {
      _clip_stop_profiler(ClipMachineMemory);
   }

   _clip_logg(6, "PCODE call from: proc '%s' file '%s' line %d", fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);
   for (;;)
   {
      int dnext = 0;

      if (_clip_debuglevel)
      {
	 dnext = _clip_debugnext;
	 _clip_debug(ClipMachineMemory);
      }

#ifdef USE_TASKS
      Task_yield();
#endif
      ret = run_vm(ClipMachineMemory, cp);

      if (_clip_sig_flag)
	 _clip_signal_real(_clip_sig_flag);

      if (_clip_debuglevel)
      {
	 if (!dnext && _clip_debugnext)
	 {
	    _clip_debugnext = 0;
	 }
      }
      _clip_logg(4, "PCODE return=%d to: proc '%s' file '%s' line %d", ret, fp->procname_of_ClipFrame ? fp->procname_of_ClipFrame : "unknown", fp->filename_of_ClipFrame, fp->line_of_ClipFrame);

      if (!ret)
	 break;

      if (!ClipMachineMemory->trapVar)
	 _clip_generr(ClipMachineMemory, ret);

      if (ret < 0)
	 break;

      if (!_clip_errorblock(ClipMachineMemory, ClipMachineMemory->trapVar, ret))
      {
	 ret = -ret;
	 break;
      }

      ClipMachineMemory->argc = _argc;
      ClipMachineMemory->fp = fp;
      ClipMachineMemory->bp = bp;
      ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;
   }

   ClipMachineMemory->argc = _argc;
   ClipMachineMemory->fp = fp;
   ClipMachineMemory->bp = bp;
   ClipMachineMemory->fp->ClipVarFrame_localvars_of_ClipFrame = s_up;

   if (isProc)
      ++argc;
   for (; argc; --argc)
   {
      --(fp->ClipVar_sp_of_ClipFrame);
      _clip_destroy(ClipMachineMemory, fp->ClipVar_sp_of_ClipFrame);
   }

   if (_clip_profiler)
   {
      _clip_start_profiler(ClipMachineMemory);
      ClipMachineMemory->pbucket->callno_of_ProfileBucket--;
   }

   return ret;
}
