static void
read_file(char *path, Coll * coll)
{
   FILE *f;

   char buf[128];

   f = fopen(path, "r");
   if (!f)
      return;

   while (fgets(buf, sizeof(buf), f))
   {
      int l;

      l = strlen(buf);
      while (l > 0 && (buf[l - 1] == '\n' || buf[l - 1] == '\r'))
	 l--;
      buf[l] = 0;
      if (!l || buf[0] == '#')
	 continue;

      add_name(coll, buf);
   }

   fclose(f);
}
