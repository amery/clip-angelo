static int
until_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   if (argc < 1)
      until_line = ClipMachineMemory->fp->line_of_ClipFrame + 1;
   else
   {
      char *e = argv[0];

      char *s = strchr(e, ':');

      if (s)
      {
	 until_line = atoi(s + 1);
	 until_file = _clip_memdup(e, s - e);
      }
      else
	 until_line = atoi(e);
   }
   in_breakpoint = 0;
   return -1;
}
