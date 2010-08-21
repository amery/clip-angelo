static int
_clip_refclone(ClipMachine * ClipMachineMemory, ClipVar * dest, ClipVar * src)
{
   if (src->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType == F_MREF_ClipFlags)
   {
      _clip_destroy(ClipMachineMemory, dest);
      dup_ref(dest, src);
      return 0;
   }
   else
      return _clip_clone(ClipMachineMemory, dest, src);
}
