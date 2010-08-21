/* dispoutat(nRow,nCol,var[,cColor]) */
int
clip_DISPOUTAT(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 3);

   clip_DISPPOS(ClipMachineMemory);

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   print_var(ClipMachineMemory, vp, get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 4)), DEV_SCR, 0);
   sync_mp(ClipMachineMemory);

   return 0;
}
