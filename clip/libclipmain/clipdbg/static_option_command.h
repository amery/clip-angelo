static int
option_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   char     *opt, *arg;


   if (argc < 1)
      return 0;

   opt = argv[0];
   arg = (argc > 1) ? argv[1] : 0;

   if (!strcasecmp(opt, "codeblock"))
    {
       if (arg)
	  bp_in_codeblock = atoi(arg);
       else
	  fprintf(_clip_dbg_out, "codeblock %d", bp_in_codeblock);
    }
   fprintf(_clip_dbg_out, "\n");

   return 0;
}
