CLIP_DLLEXPORT long
_clip_pop_hash(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);

   long r;

   r = calc_casehash(ClipMachineMemory, vp, 1);
   _clip_destroy(ClipMachineMemory, vp);

   return r;
}
