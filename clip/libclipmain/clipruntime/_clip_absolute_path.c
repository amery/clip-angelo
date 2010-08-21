int
_clip_absolute_path(ClipMachine * ClipMachineMemory, const char *path, char *p, int len)
{
   char op[PATH_MAX];

   char *e = p;

#ifndef _WIN32
   const char *s = path;

   p[0] = 0;
   if (path[1] == ':')
   {
      char dn[3];

      char *root;

      dn[0] = toupper(*s);
      dn[1] = ':';
      dn[2] = 0;
      root = _clip_fetch_item(ClipMachineMemory, _clip_hashstr(dn));
      if (!root)
	 return 0;
      strncpy(p, root, len - 1);
      e = p + strlen(p);
      if (*(e - 1) != '/')
      {
	 *e = '/';
	 e++;
	 *e = 0;
      }
      s += 2;
      if (path[2] == '\\' || path[2] == '/')
	 s++;
   }
   strncpy(e, s, p + len - e - 2);
#else
   if ((memcmp(path, "\\\\", 2) == 0) || (memcmp(path, "//", 2) == 0))
   {
      /* network resource */
      strncpy(p, path, len);
   }
   else
   {
      if (path != NULL && (*path == '/' || *path == '\\') && memcmp(path, "/cygdrive/", 10))
      {

	 /* add default drive */
	 char *s = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

	 snprintf(p, len - 1, "%s%s", s, path);
      }
      else
	 strncpy(p, path, len);
   }
#endif
   _clip_unix_path(p, ClipMachineMemory->flags & TRANSLATE_FLAG);

   errno = 0;
   getcwd(op, sizeof(op));
   if (errno)
      return 1;
   chdir(p);
/*	if (errno)
		return 1;*/
   errno = 0;
   getcwd(p, len);
   if (errno)
      return 1;
   chdir(op);
   if (errno)
      return 1;

   e = p + strlen(p) - 1;
   if (*e != '/' && e - p < len - 2)
   {
      *(e + 1) = '/';
      *(e + 2) = 0;
   }

   return 0;
}
