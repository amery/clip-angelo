int
clip_QQOUT(ClipMachine * ClipMachineMemory)
{
   int i;

   for (i = 1; i <= ClipMachineMemory->argc; ++i)
   {
      ClipVar *vp = _clip_par(ClipMachineMemory, i);

      if (i > 1)
	 out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_DEVS);
      if (!vp)
	 continue;
      print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_DEVS, 0);
   }
   sync_mp(ClipMachineMemory);

   return 0;
}
