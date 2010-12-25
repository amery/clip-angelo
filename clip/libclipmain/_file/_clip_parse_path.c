int
_clip_parse_path(char *fullpath, char *path, char *name)
{
   int len, i, j;

   len = strlen(fullpath);
   for (i = len; i >= 0 && fullpath[i] != '/' && fullpath[i] != '\\' && fullpath[i] != ':'; i--);
   for (j = i + 1; j <= len; j++)
      name[j - i - 1] = fullpath[j];
   if (i == 0)
   {
      path[0] = fullpath[0];
      path[1] = 0;
   }
   else if (i > 0)
   {
#ifdef _WIN32
      if ((i > 1 && fullpath[i - 1] == ':' && fullpath[i] == '\\') || fullpath[i] == ':')
	 i++;
#endif
      path[i] = 0;
      for (i--; i >= 0; i--)
	 path[i] = fullpath[i];
      i++;
   }
   else
      path[0] = 0;
   return i;
}
