int
clip_OUTERR(ClipMachine * ClipMachineMemory)
{
   int i;

   for (i = 1; i <= ClipMachineMemory->argc; ++i)
   {
      ClipVar *vp = _clip_par(ClipMachineMemory, i);

      if (i > 1)
	 out_any(ClipMachineMemory, " ", 1, _clip_colorSelect(ClipMachineMemory), DEV_ERR);
      print_var(ClipMachineMemory, vp, _clip_colorSelect(ClipMachineMemory), DEV_ERR, 0);
   }
   return 0;
}
