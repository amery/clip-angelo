static int
help_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   if (argc < 1)
   {
      DbgCommandBucket *bp;

      int i;

      for (i = 0, bp = dbg_buckets; bp->name; bp++, i++)
      {
	 fprintf(_clip_dbg_out, "%s", bp->name);
	 if (!i || i % 9)
	    fputc('\t', _clip_dbg_out);
	 else
	    fputc('\n', _clip_dbg_out);
      }
   }
   else
   {
      DbgCommandBucket *bp;

      bp = find_dbg_command(argv[0]);
      if (bp)
	 fprintf(_clip_dbg_out, "%s", bp->help);
      else
	 fprintf(_clip_dbg_out, "unknown command %s", argv[0]);
   }
   fprintf(_clip_dbg_out, "\n");

   return 0;
}
