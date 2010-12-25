int
load_charset_name(char *bname, cons_CharsetEntry ** cp, int *lenp)
{
   char path[256];

   char *name, *s;

   FILE *file;

   int r;

   name = strdup(bname);
   for (s = name; *s; s++)
      *s = tolower(*s);

   snprintf(path, sizeof(path), "%s/charsets/%s", CLIPROOT, name);
   file = fopen(path, "r");
   if (!file)
   {
      snprintf(path, sizeof(path), "%s/charsets/%s.uni", CLIPROOT, name);
      file = fopen(path, "r");
   }
   if (!file)
   {
      snprintf(path, sizeof(path), "%s/charsets/%s.sfm", CLIPROOT, name);
      file = fopen(path, "r");
   }

   if (!file)
      return -1;

   r = load_charset(file, cp, lenp);
   fclose(file);
   free(name);
   return r;
}
