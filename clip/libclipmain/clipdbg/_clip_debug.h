int
_clip_debug(ClipMachine * ClipMachineMemory)
{
   if (!_clip_debuglevel)
      return 0;

   if (!debugging)
      return 0;

   if (in_sigdebug && debug_flag && _clip_dbg_in)
    {
       char      buf[1024];

       int       l;

      /* read command from _clip_dbg_in */
       if (!dbg_commands)
	  dbg_commands = new_Coll(free, 0);
       fgets(buf, sizeof(buf), _clip_dbg_in);
       l = strlen(buf);

       if (l == 1 && buf[0] == '\n')
	  buf[0] = '?';
       else if (l > 0 && buf[l - 1] == '\n')
	  buf[l - 1] = 0;

       append_Coll(dbg_commands, strdup(buf));

       _clip_logg(4, "dbg read cmd: %s", buf);

       debug_flag = 0;
    }

   if (!dbg_commands)
      return 0;

  /* process commands */
   while (dbg_commands->count)
    {
       char     *cmd = (char *) dbg_commands->items[0];

       int       r;

       r = _clip_process_dbg(ClipMachineMemory, cmd);

       atFree_Coll(dbg_commands, 0);

       if (r > 0)
	  return 0;
       else if (r < 0)
	  return r;
    }

  /* check breakpoints */
   if (!in_sigdebug)
    {
       ClipBreakPoint *bp = _clip_breakpoint, *nbp;

       int       ind;

       if (in_breakpoint)
	{
	   fprintf(_clip_dbg_out, "\ninterrupt reached:\n");
	   fprintf(_clip_dbg_out, "file:%s", nullstr(ClipMachineMemory->fp->filename_of_ClipFrame));
	   fprintf(_clip_dbg_out, " line:%d", ClipMachineMemory->fp->line_of_ClipFrame);
	   fprintf(_clip_dbg_out, " func:%s", nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
	   fprintf(_clip_dbg_out, "\n.\n");
	   fflush(_clip_dbg_out);
	   _clip_in_breakpoint();
	   return 0;
	}

       if (next_line && !_clip_debugnext && ClipMachineMemory->fp->line_of_ClipFrame
	   && ClipMachineMemory->fp->line_of_ClipFrame != next_line)
	{
	   next_line = 0;
	   fprintf(_clip_dbg_out, "step line reached:\n");
	   fprintf(_clip_dbg_out, "file:%s", nullstr(ClipMachineMemory->fp->filename_of_ClipFrame));
	   fprintf(_clip_dbg_out, " line:%d", ClipMachineMemory->fp->line_of_ClipFrame);
	   fprintf(_clip_dbg_out, " func:%s", nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
	   fprintf(_clip_dbg_out, "\n.\n");
	   fflush(_clip_dbg_out);
	   _clip_in_breakpoint();
	   return 0;
	}

       if (next_line && _clip_debugnext && ClipMachineMemory->fp->line_of_ClipFrame
	   && ClipMachineMemory->fp->line_of_ClipFrame != next_line && next_file
	   && next_file == ClipMachineMemory->fp->filename_of_ClipFrame && next_proc
	   && next_proc == ClipMachineMemory->fp->procname_of_ClipFrame)
	{
	   _clip_debugnext = 0;
	   next_line = 0;
	   next_file = 0;
	   next_proc = 0;
	   fprintf(_clip_dbg_out, "next line reached:\n");
	   fprintf(_clip_dbg_out, "file:%s", nullstr(ClipMachineMemory->fp->filename_of_ClipFrame));
	   fprintf(_clip_dbg_out, " line:%d", ClipMachineMemory->fp->line_of_ClipFrame);
	   fprintf(_clip_dbg_out, " func:%s", nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
	   fprintf(_clip_dbg_out, "\n.\n");
	   fflush(_clip_dbg_out);
	   _clip_in_breakpoint();
	   return 0;
	}

       if (until_line && ClipMachineMemory->fp->line_of_ClipFrame == until_line
	   && (!until_file
	       || (ClipMachineMemory->fp->filename_of_ClipFrame
		   && !strcasecmp(until_file, ClipMachineMemory->fp->filename_of_ClipFrame))))
	{
	   until_line = 0;
	   if (until_file)
	    {
	       free(until_file);
	       until_file = 0;
	    }
	   fprintf(_clip_dbg_out, "until line reached:\n");
	   fprintf(_clip_dbg_out, "file:%s", nullstr(ClipMachineMemory->fp->filename_of_ClipFrame));
	   fprintf(_clip_dbg_out, " line:%d", ClipMachineMemory->fp->line_of_ClipFrame);
	   fprintf(_clip_dbg_out, " func:%s", nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
	   fprintf(_clip_dbg_out, "\n.\n");
	   fflush(_clip_dbg_out);
	   _clip_in_breakpoint();
	   return 0;
	}

       if (!bp)
	{
	   bp = &cur_bp;
	   bp->filename_of_ClipBreakPoint = (char *) ClipMachineMemory->fp->filename_of_ClipFrame;
	   bp->procname_of_ClipBreakPoint = (char *) ClipMachineMemory->fp->procname_of_ClipFrame;
	   bp->line_of_ClipBreakPoint = ClipMachineMemory->fp->line_of_ClipFrame;
	}

       if (search_Coll(&sbreakpoints, bp, &ind))
	{
	   nbp = (ClipBreakPoint *) sbreakpoints.items[ind];
	   if (nbp == prev_bp)
	      return 0;
	   if (!bp_in_codeblock && ClipMachineMemory->fp->procname_of_ClipFrame
	       && !memcmp(ClipMachineMemory->fp->procname_of_ClipFrame, "___code_", 8))
	      return 0;
	   fprintf(_clip_dbg_out, "\nbreakpoint: file %s line %d proc %s",
		   nullstr(bp->filename_of_ClipBreakPoint), bp->line_of_ClipBreakPoint,
		   nullstr(bp->procname_of_ClipBreakPoint));
	   fprintf(_clip_dbg_out, "\n.\n");
	   fflush(_clip_dbg_out);
	   _clip_logg(1, "breakpoint reached: file %s line %d proc %s",
		      nullstr(bp->filename_of_ClipBreakPoint), bp->line_of_ClipBreakPoint,
		      nullstr(bp->procname_of_ClipBreakPoint));
	   prev_bp = nbp;
	   _clip_in_breakpoint();
	}
       else
	{
	   prev_bp = 0;

	   if (watchs.count)
	    {
	       int       i;

	       for (i = 0; i < watchs.count; i++)
		{
		   ClipVar   res;

		   char     *expr = (char *) watchs.items[i];

		   int       l;

		   memset(&res, 0, sizeof(res));
		   debugging = 0;
		   ClipMachineMemory->noerrblock++;
		   _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &res);
		   ClipMachineMemory->noerrblock--;
		   l = _clip_log(&res);
		   _clip_destroy(ClipMachineMemory, &res);
		   debugging = 1;
		   if (l)
		    {
		      /*watch_line=ClipMachineMemory->fp->line; */
		       fprintf(_clip_dbg_out,
			       "\nwatchpoint: expr '%s' file %s line %d proc %s",
			       expr, nullstr(ClipMachineMemory->fp->filename_of_ClipFrame),
			       ClipMachineMemory->fp->line_of_ClipFrame, nullstr(ClipMachineMemory->fp->procname_of_ClipFrame));
		       fprintf(_clip_dbg_out, "\n.\n");
		       fflush(_clip_dbg_out);
		       _clip_logg(1,
				  "watchpoint reached: expr '%s' file %s line %d proc %s",
				  expr, nullstr(bp->filename_of_ClipBreakPoint), bp->line_of_ClipBreakPoint,
				  nullstr(bp->procname_of_ClipBreakPoint));
		       _clip_in_breakpoint();
		    }
		}
	    }
	}
    }

   return 0;
}
