/*
   Sets the window shadow colors
 */
int
clip_WSETSHADOW(ClipMachine * ClipMachineMemory)
{
   char *color = _clip_parc(ClipMachineMemory, 1);

   int ncolor = _clip_parni(ClipMachineMemory, 1);

   int r = ClipMachineMemory->wshadow;

   _clip_fullscreen(ClipMachineMemory);

   if (ClipMachineMemory->argc > 0)
   {
      if (color)
	 ClipMachineMemory->wshadow = get_color(ClipMachineMemory, color);
      else
	 ClipMachineMemory->wshadow = ncolor;
   }

   _clip_retni(ClipMachineMemory, r);

   return 0;
}
