int
load_unicode_table(char *name, int *tbl)
{
   char buf[PATH_MAX];

   char sbuf[4096];

   FILE *f;

   int len;

   unsigned int ch, uch;

   snprintf(buf, PATH_MAX, "%s/charsets/%s.uni", CLIPROOT, name);

   f = fopen(buf, "rt");

   if (f == NULL)
      return -1;

   while (!feof(f))
   {
      len = freadstr(f, sbuf, 4096);
      if (len == 0)
	 continue;
      /* first spaces */
      for (len = 0; sbuf[len] != 0 && (sbuf[len] == ' ' || sbuf[len] == '\t'); len++);
      /* comments */
      if (sbuf[len] == '#' || sbuf[len] < 32)
	 continue;
      ch = strtol(sbuf + len, NULL, 16);
      for (; sbuf[len] != 0 && sbuf[len] != '+'; len++);
      uch = strtol(sbuf + len, NULL, 16);
      if (ch > 256)
      {
	 printf("char code >255\n");
	 continue;
      }
      tbl[ch] = uch;
   }

   fclose(f);
   return 0;
}
