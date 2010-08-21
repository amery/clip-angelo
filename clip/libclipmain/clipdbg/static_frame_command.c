static int
frame_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   if (argc < 1)
   {
      ClipFrame *fp;

    prframe:
      fp = get_frame(ClipMachineMemory);
      fprintf(_clip_dbg_out, "%d: file:%s", cur_frame, nullstr(fp->filename_of_ClipFrame));
      fprintf(_clip_dbg_out, " line:%d", fp->line_of_ClipFrame);
      fprintf(_clip_dbg_out, " func:%s\n", nullstr(fp->procname_of_ClipFrame));
      return 0;
   }
   else
   {
      int fno = atoi(argv[0]);

      ClipFrame *fp;

      int i;

      for (i = 0, fp = ClipMachineMemory->fp; fp; fp = fp->ClipFrame_up_of_ClipFrame)
      {
	 if (!fp->line_of_ClipFrame)
	    continue;
	 if (i == fno)
	 {
	    cur_frame = fno;
	    goto prframe;
	 }
	 i++;
      }
   }

   return 0;
}
