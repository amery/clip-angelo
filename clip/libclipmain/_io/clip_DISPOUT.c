int
clip_DISPOUT(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 2)), DEV_SCR, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}
