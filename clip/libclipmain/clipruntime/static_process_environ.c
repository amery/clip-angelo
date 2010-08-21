static void
process_environ(char *root, char *name)
{
   char buf[256];

   FILE *file;

   snprintf(buf, sizeof(buf), "%s/%s", root, name);
   file = fopen(buf, "rt");
   if (!file)
      goto ret;
   while (fgets(buf, sizeof(buf), file))
   {
      char *s, *e, *g;

      int l;

      int overwrite = 0;

      s = buf;

    again:
      l = strspn(s, " \t");
      s += l;
      l = strlen(s);
      if (l < 2)
	 continue;
      s[l - 1] = 0;
      if (s[0] == '#')
	 continue;
      if (s[0] == '+')
      {
	 s++;
	 overwrite = 1;
      }
      else if (s[0] == '?')
      {
	 char *g1;

	 s++;
	 e = strchr(s, '=');
	 if (e)
	    *e++ = 0;
	 else
	    continue;
	 g = strchr(e + 1, '?');
	 if (g)
	    *g++ = 0;
	 else
	    continue;
	 g1 = get_env(_clip_envp, s);
	 if (!g1)
	    continue;
	 if (strcmp(e, g1))
	    continue;
	 /* condition matched */
	 s = g;
	 goto again;
      }

      if (!overwrite)
      {
	 e = strchr(s, '=');
	 if (e)
	    *e = 0;
	 g = get_env(_clip_envp, s);
	 /* don't overwrite!! */
	 if (g)
	    continue;
	 if (e)
	    *e = '=';
      }
      put_env(&_clip_envp, s);
   }

   fclose(file);
 ret:
   set_sysvars();
}
