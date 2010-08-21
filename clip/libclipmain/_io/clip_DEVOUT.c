int
clip_DEVOUT(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2)), DEV_DEV, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}
