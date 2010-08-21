CLIP_DLLEXPORT int
_clip_store(ClipMachine * ClipMachineMemory, int dim, int byref)
{
   ClipVar *map = 0;

   long hash = 0;

   ClipVar *vp = clip_fetch(ClipMachineMemory, dim, 0, 1, &map, &hash);

   if (!vp)
      return 1;

   if (map)
      test_modify(ClipMachineMemory, map, hash);

   if (byref == 2)
   {
      _clip_destroy(ClipMachineMemory, vp);
      CLEAR_CLIPVAR(ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame);
      ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame++;
      CLIP_CHECK_STACK;
   }
   else if (byref == 1)
      _clip_refassign(ClipMachineMemory, vp, 1);
   else
      _clip_assign(ClipMachineMemory, vp);
   return 0;
}
