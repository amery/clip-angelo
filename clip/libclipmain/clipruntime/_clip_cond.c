CLIP_DLLEXPORT int
_clip_cond(ClipMachine * ClipMachineMemory, int *ifp)
{
   int r = _clip_tcond(ClipMachineMemory, ifp);

   if (r)
      return r;

   ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame--;
   _clip_destroy(ClipMachineMemory, ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
   return 0;
}
