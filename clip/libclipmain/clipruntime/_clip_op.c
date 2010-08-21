CLIP_DLLEXPORT int
_clip_op(ClipMachine * ClipMachineMemory, int op)
{
   int ret;

   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *lval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 2;

   if ((ret = do_assign(ClipMachineMemory, lval, rval, op)))
      return ret;
   /*return _clip_call_errblock(ClipMachineMemory, ret); */
   _clip_destroy(ClipMachineMemory, rval);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return 0;
}
