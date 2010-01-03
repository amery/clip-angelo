static int
bt_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   ClipFrame *fp;

   int       i;

   for (i = 0, fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
    {
       if (!fp->line_of_ClipFrame)
	  continue;
       fprintf(_clip_dbg_out, "%d: file:%s", i, nullstr(fp->filename_of_ClipFrame));
       fprintf(_clip_dbg_out, " line:%d", fp->line_of_ClipFrame);
       fprintf(_clip_dbg_out, " func:%s\n", nullstr(fp->procname_of_ClipFrame));
       i++;
    }

   return 0;
}
