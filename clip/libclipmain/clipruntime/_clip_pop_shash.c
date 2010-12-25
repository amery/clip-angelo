
CLIP_DLLEXPORT long
_clip_pop_shash(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = --(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);

   long r;

   r = _clip_hash(ClipMachineMemory, vp);
   _clip_destroy(ClipMachineMemory, vp);

   return r;
}
