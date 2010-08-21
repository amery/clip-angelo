static int
interrupt_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   fprintf(_clip_dbg_out, "interrupt at ");
   fprintf(_clip_dbg_out, "file:%s", nullstr(ClipMachineMemory->fp->filename_of_ClipFrame));
   fprintf(_clip_dbg_out, " line:%d", ClipMachineMemory->fp->line_of_ClipFrame);
   fprintf(_clip_dbg_out, " func:%s\n", nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
   in_breakpoint = 1;
   return 0;
}
