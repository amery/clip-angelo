static int
bp_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int       i;


   if (argc < 1)
    {
       fprintf(_clip_dbg_out, "%d breakpoints:\n", breakpoints.count);
       for (i = 0; i < breakpoints.count; i++)
	{
	   ClipBreakPoint *bp = (ClipBreakPoint *) breakpoints.items[i];

	   fprintf(_clip_dbg_out, "%d: file:%s line:%d func:%s\n",
		   i, nullstr(bp->filename_of_ClipBreakPoint), bp->line_of_ClipBreakPoint,
		   nullstr(bp->procname_of_ClipBreakPoint));
	}
    }
   else
    {
       for (i = 0; i < argc; i++)
	{
	   char     *s = argv[i], *e;

	   int       line;

	   char     *filename = 0, *procname = 0;

	   ClipFrame *fp = get_frame(ClipMachineMemory);

	   ClipBreakPoint *bp;

	   if (is_dig(s, strlen(s)))
	    {
	       line = atoi(s);
	       if (fp->filename_of_ClipFrame)
		  filename = strdup(fp->filename_of_ClipFrame);
	       procname = 0;
	    }
	   else if ((e = strchr(s, ':')))
	    {
	       line = atoi(e + 1);
	       if (e > s)
		  filename = _clip_memdup(s, e - s);
	       else if (fp->filename_of_ClipFrame)
		  filename = strdup(fp->filename_of_ClipFrame);
	       procname = 0;
	    }
	   else
	    {
	       line = 0;
	       filename = 0;
	       procname = strdup(s);
	    }

	   cur_bp.filename_of_ClipBreakPoint = filename;
	   cur_bp.procname_of_ClipBreakPoint = procname;
	   cur_bp.line_of_ClipBreakPoint = line;
	   if (search_Coll(&sbreakpoints, &cur_bp, 0))
	    {
	       free(filename);
	       free(procname);
	       fprintf(_clip_dbg_out, "breakpoint already exist\n");
	    }
	   else
	    {
	       bp = (ClipBreakPoint *) calloc(sizeof(ClipBreakPoint), 1);
	       bp->filename_of_ClipBreakPoint = filename;
	       bp->procname_of_ClipBreakPoint = procname;
	       bp->line_of_ClipBreakPoint = line;

	       append_Coll(&breakpoints, bp);
	       insert_Coll(&sbreakpoints, bp);
	    }
	}
    }

   return 0;
}
