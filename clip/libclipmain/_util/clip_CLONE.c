int
clip_CLONE(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   ClipVar *rp;

   if (!vp)
      return 0;

   rp = RETPTR(ClipMachineMemory);
   _clip_dup(ClipMachineMemory, rp, vp);
   return 0;
}
