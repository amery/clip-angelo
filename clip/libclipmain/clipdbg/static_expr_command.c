static int
expr_command(ClipMachine * ClipMachineMemory, int argc, char **argv)
{
   int i;

   char *s = 0;

   int l = 0;

   ClipVar res;

   int binary = 0;

   for (i = 0; i < argc; i++)
   {
      if (!strcasecmp(argv[i], "-binary"))
	 binary = 1;
      else
	 break;
   }

   for (; i < argc; i++)
   {
      int ln = strlen(argv[i]);

      s = (char *) realloc(s, l + ln + 2);
      s[l] = ' ';
      memcpy(s + l + 1, argv[i], ln);
      s[l + 1 + ln] = 0;
      l += ln + 1;
   }

   memset(&res, 0, sizeof(res));
   debugging = 0;
   ClipMachineMemory->noerrblock++;
   _clip_eval_macro(ClipMachineMemory, s, l, &res);
   ClipMachineMemory->noerrblock--;
   fprintf(_clip_dbg_out, "expr; var; ");
   print_dbg(ClipMachineMemory, &res, binary);
   _clip_destroy(ClipMachineMemory, &res);
   debugging = 1;

   fprintf(_clip_dbg_out, "\n");

   free(s);
   return 0;
}
