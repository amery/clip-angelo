CLIP_DLLEXPORT int
_clip_opassign(struct ClipMachine *ClipMachineMemory, void *Lval, int op)
{
   int ret;

   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   if ((ret = do_assign(ClipMachineMemory, (ClipVar *) Lval, rval, op)))
      return ret;
   _clip_destroy(ClipMachineMemory, rval);
   --ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame;
   return 0;
}
