CLIP_DLLEXPORT int
_clip_opiassign(struct ClipMachine *ClipMachineMemory, void *Lval, int op)
{
   int ret;

   ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

   ClipVar *lval = (ClipVar *) Lval;

   if ((ret = do_assign(ClipMachineMemory, lval, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1, op)))
      return ret;
   _clip_destroy(ClipMachineMemory, rval);
   if ((ret = _clip_mclone(ClipMachineMemory, rval, lval)))
      return ret;
   return 0;
}
