static int
rm_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int       i;


   for (i = 0; i < argc; i++)
    {
       char     *s = argv[i], *e;

       int       line, ind;

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
       if (!search_Coll(&sbreakpoints, &cur_bp, &ind))
	{
	   fprintf(_clip_dbg_out, "no breakpoint at file:%s line:%d proc:%s\n", nullstr(filename), line, nullstr(procname));
	}
       else
	{
	   bp = (ClipBreakPoint *) sbreakpoints.items[ind];
	   atRemove_Coll(&sbreakpoints, ind);
	   remove_Coll(&breakpoints, bp);
	   free(bp->filename_of_ClipBreakPoint);
	   free(bp->procname_of_ClipBreakPoint);
	   free(bp);
	   fprintf(_clip_dbg_out,
		   "breakpoint removed at file:%s line:%d proc:%s\n", nullstr(filename), line, nullstr(procname));
	}

       free(filename);
       free(procname);
    }

   return 0;
}
