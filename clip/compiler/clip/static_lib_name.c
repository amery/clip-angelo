static void
lib_name(char *buf, int buflen, char *root, char *prefix, char *e, int elen)
{
   if ((!root || !root[0]) && elen > 3 && !memcmp(e, "lib", 3))
   {
      int l;

      char *s = strrchr(e + 3, '.');

      if (s)
	 l = s - (e + 3);
      else
	 l = elen - 3;
      snprintf(buf, buflen, "-l%.*s", l, e + 3);
   }
   else if (!root || !root[0] || e[0] == '/' || (e[0] == '.' && e[1] == '/') || (e[0] == '.' && e[1] == '.' && e[2] == '/'))
      snprintf(buf, buflen, "%.*s", elen, e);
   else
      snprintf(buf, buflen, "%s/%s/%.*s", root, prefix, elen, e);
}
