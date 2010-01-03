static int
kill_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int       code = 0;


   if (argc > 0)
      code = atoi(argv[0]);

   fprintf(_clip_dbg_out, "dbg kill: %s: exit now", _clip_progname);
   fprintf(_clip_dbg_out, "\n.\n");
   fflush(_clip_dbg_out);

   exit(code);
   return 0;
}
