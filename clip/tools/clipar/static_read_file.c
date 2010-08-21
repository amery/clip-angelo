static void
read_file(char *name, Coll * cp, Coll * readed)
{
   FILE *file = fopen(name, "rb");

   char *modp;

   int j;

   if (!file)
      error("cannot open file '%s': %s", name, strerror(errno));

   for (j = 0; (modp = read_module(file)); ++j)
   {
      /*char *modbeg = M_OFFS(modp, 2, 0); */
      char *filename = M_OFFS(modp, 8, 6 /*7, 7 */ );

      char *e = strrchr(filename, '.');

      if (!readed)
      {
	 /* all modules */
	 append_Coll(cp, modp);
	 continue;
      }

      /* skip internal .pa headers */
      if (e && !strcmp(e, ".pa"))
	 continue;
      /* skip duplicated modules */
      if (!insert_Coll(readed, filename))
	 continue;
      append_Coll(cp, modp);
   }
   v_printf(1, "read file '%s': %d modules\n", name, j);

   fclose(file);
}
