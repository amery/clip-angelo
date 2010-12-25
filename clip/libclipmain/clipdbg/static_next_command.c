static int
next_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   next_line = ClipMachineMemory->fp->line_of_ClipFrame ? ClipMachineMemory->fp->line_of_ClipFrame : -1;
   next_file = (char *) ClipMachineMemory->fp->filename_of_ClipFrame;
   next_proc = (char *) ClipMachineMemory->fp->procname_of_ClipFrame;
   if (next_file && next_proc)
      _clip_debugnext = 1;
   else
      _clip_debugnext = 0;
   in_breakpoint = 0;
   return -1;
}
