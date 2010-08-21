static int
step_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   _clip_debugnext = 0;
   next_line = ClipMachineMemory->fp->line_of_ClipFrame ? ClipMachineMemory->fp->line_of_ClipFrame : -1;
   next_file = 0;
   next_proc = 0;
   in_breakpoint = 0;
   return -1;
}
