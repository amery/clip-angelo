int
clip_SETCOLOR(ClipMachine * ClipMachineMemory)
{
   char buf[64], *sp;

   int get_num = _clip_parl(ClipMachineMemory, 2);

   get_color(ClipMachineMemory, buf, sizeof(buf), get_num);
   _clip_retc(ClipMachineMemory, buf);
   if (ClipMachineMemory->argc > 0 && (sp = _clip_parc(ClipMachineMemory, 1)))
   {
      if (*sp)
	 set_color(ClipMachineMemory, sp);
      else
	 set_color(ClipMachineMemory, DEFAULT_COLOR);
   }
   return 0;
}
