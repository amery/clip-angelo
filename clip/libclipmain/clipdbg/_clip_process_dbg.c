int
_clip_process_dbg(ClipMachine * ClipMachineMemory, char *str)
{
   char **argv = 0;

   int argc = 0;

   char *cmd = 0;

   DbgCommandBucket *bp;

   int r = 0;

   _clip_logg(0, "process dbg cmd: '%s'", str);

   split_vector(str, &argv, &argc);

   if (!argc)
      goto empty;

   cmd = argv[0];

   bp = find_dbg_command(cmd);

   _clip_logg(0, "find dbg cmd: '%s', %d argc", cmd, argc);
   /*fprintf(_clip_dbg_out, "\n"); */
   if (bp)
      r = bp->dcp(ClipMachineMemory, argc - 1, argv + 1);
   else
   {
      int i;

      fprintf(_clip_dbg_out, "unknown command (try ?):");
      for (i = 0; i < argc; i++)
	 fprintf(_clip_dbg_out, " %s", argv[i]);
      fprintf(_clip_dbg_out, "\n");
   }

 empty:
   if (r >= 0)
   {
      fprintf(_clip_dbg_out, ".\n");
   }
   fflush(_clip_dbg_out);

/*ret: */
   free(argv);
   _clip_logg(0, "end cmd: '%s'", cmd);
   return r;
}
