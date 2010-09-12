static int
set_locale(void)
{
   char path[256], *s, *e;

   char *filename = file_name;

   if (out)
   {
      fclose(out);
      out = 0;
   }

   snprintf(path, sizeof(path), "%s/locale.pot/%s", CLIP_LOCALE_ROOT, CLIP_MODULE);

   if (make_dir(path))
   {
      file_name = 0;
      return -1;
   }

   snprintf(path, sizeof(path), "%s", filename);
   s = strrchr(path, '/');
   e = strrchr(path, '.');
   if (!s)
      s = path;
   if (e && e > path)
      *e = 0;

   free(fname);
   fname = strdup(s);

   snprintf(path, sizeof(path), "%s/locale.pot/%s/%s.pot", CLIP_LOCALE_ROOT, CLIP_MODULE, fname);
   mname = strdup(CLIP_MODULE);
   for (s = mname; *s; s++)
      *s = toupper(*s);

   out = fopen(path, "w");
   if (!out)
   {
      yywarning("cannot create file '%s': %s", path, strerror(errno));
      file_name = 0;
      return -1;
   }
   v_printf(1, "\nopen locale file '%s'\n", path);

   return 0;
}
