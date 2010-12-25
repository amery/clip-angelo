CLIP_DLLEXPORT int
_clip_proc_hash(struct ClipMachine *ClipMachineMemory, long hash, int argc, int rest, ClipVarFrame * uplocals)
{
   ClipFunction *fp;

   ClipBlock *bp;

   int r = 0;

   ClipFrame *inMacro = ClipMachineMemory->inMacro;

   ClipMachineMemory->inMacro = 0;
   if (!_clip_get_function(ClipMachineMemory, hash, &fp, &bp))
   {
      char buf[64];

      _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
      r = _clip_trap_err(ClipMachineMemory, EG_NOFUNC, 0, 0, 0, 0, buf);
      return _clip_call_errblock(ClipMachineMemory, r);
   }

   if (fp)
   {
      char *oldbuf = 0, *buf = 0;

      if (log_level >= 4)
      {
	 if (ClipMachineMemory->fp->procname_of_ClipFrame)
	 {
	    buf = (char *) calloc(1, 64);
	    _clip_hash_name(ClipMachineMemory, hash, buf, 64);
	    oldbuf = (char *) ClipMachineMemory->fp->procname_of_ClipFrame;
	    ClipMachineMemory->fp->procname_of_ClipFrame = buf;
	 }
      }

      r = _clip_proc(ClipMachineMemory, fp, argc, rest, uplocals);

      if (log_level >= 4)
      {
	 if (oldbuf)
	 {
	    ClipMachineMemory->fp->procname_of_ClipFrame = oldbuf;
	    free(buf);
	 }

      }
   }
   else
      r = _clip_code_proc(ClipMachineMemory, bp, argc, rest, uplocals);

   ClipMachineMemory->inMacro = inMacro;
   return r;
}
