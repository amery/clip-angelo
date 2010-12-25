int
clip_DEVOUTPICT(ClipMachine * ClipMachineMemory)
{
   ClipVar *vp = _clip_par(ClipMachineMemory, 1);

   char *pict = _clip_parc(ClipMachineMemory, 2);

   int attr;

   _clip_fullscreen(ClipMachineMemory);

   if (!vp)
      return 0;

   attr = get_color(ClipMachineMemory, _clip_parc(ClipMachineMemory, 3));
   if (pict)
   {
      transform(ClipMachineMemory, vp, pict);
      out_dev(ClipMachineMemory, ClipMachineMemory->buf, strlen(ClipMachineMemory->buf), attr, 0);
   }
   else
      print_var(ClipMachineMemory, vp, attr, DEV_SCR, 0);

   sync_mp(ClipMachineMemory);

   return 0;
}
