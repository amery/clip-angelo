int
clip_SETFIELD(ClipMachine * ClipMachineMemory)
{
   int l;

   char *name = _clip_parcl(ClipMachineMemory, 1, &l);

   ClipVar *vp = _clip_par(ClipMachineMemory, 2);

   if (!vp || !name)
      return 0;

   return _clip_nameassign(ClipMachineMemory, name, l, vp, 1);
}
