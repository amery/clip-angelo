static ClipFrame *
get_frame(ClipMachine * ClipMachineMemory)
{
   ClipFrame *fp;

   int i;

   for (i = 0, fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
   {
      if (!fp->line_of_ClipFrame)
	 continue;
      if (i == cur_frame)
	 return fp;
      i++;
   }

   if (!fp)
      fp = ClipMachineMemory->fp;
   return fp;
}
