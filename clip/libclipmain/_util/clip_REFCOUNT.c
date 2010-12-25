int
clip_REFCOUNT(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   if (!vp)
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, vp->ClipType_t_of_ClipVar.count_of_ClipType);

   return 0;
}
