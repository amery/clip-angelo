static void
read_names(char *s, Coll * ex, Coll * nm)
{
   int       al;

   char      path[256], *e, *b;

   if (!names_flag)
      return;

   b = strrchr(s, '/');
   if (b)
      b++;
   else
      b = s;
   e = strchr(b, '.');

   if (e && (e > b))
      al = e - s;
   else
      al = strlen(s);

   snprintf(path, sizeof(path), "%s", s);
   snprintf(path + al, sizeof(path) - al, ".ex");
   read_file(path, ex);
   snprintf(path + al, sizeof(path) - al, ".nm");
   read_file(path, nm);
}
