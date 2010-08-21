CLIP_DLLEXPORT int
_clip_path(ClipMachine * ClipMachineMemory, const char *fn, char *p, int len, int create)
{
   char path[PATH_MAX];

   const char *s, *s1 = NULL, *s2 = NULL;

   if (strchr(fn, '|'))
   {
      strncpy(p, fn, len);
      p[len - 1] = 0;
      return 0;
   }

   if ((memcmp(fn, "\\\\", 2) == 0) || (memcmp(fn, "//", 2) == 0))
   {
      strncpy(p, fn, len);
   }
   else if ((s = strrchr(fn, '/')) || (s1 = strrchr(fn, '\\')) || (s2 = strrchr(fn, ':')))
   {
      char path[PATH_MAX];

      int f = 0;

      if (!s1)
	 s1 = strrchr(fn, '\\');
      if (!s2)
	 s2 = strrchr(fn, ':');
      s = (s > s1) ? s : s1;
      if (s2 > s)
	 f = 1;
      s = (s > s2) ? s : s2;
      memcpy(path, fn, s - fn + f);
      path[s - fn + f] = 0;
      if (_clip_absolute_path(ClipMachineMemory, path, p, len))
	 return 1;
      strncat(p, s + 1, len - strlen(p) - 1);
      _clip_unix_path(p, ClipMachineMemory->flags & TRANSLATE_FLAG);
   }
   else
   {
      if (_clip_absolute_path(ClipMachineMemory, ClipMachineMemory->defaul, p, len))
	 return 1;
      strncat(p, fn, len - strlen(p) - 1);
      _clip_unix_path(p, ClipMachineMemory->flags & TRANSLATE_FLAG);
      if (!create && access(p, F_OK))
      {
	 char *c, *c1 = path;

	 strncpy(path, ClipMachineMemory->path, sizeof(path) - 1);
	 while (c1 != (char *) 1 && access(p, F_OK))
	 {
	    c = strchr(c1, ';');
	    if (c)
	    {
	       *c = 0;
	    }
	    else
	    {
	       c = strchr(c1, ',');
	       if (c)
		  *c = 0;
	    }
	    if (_clip_absolute_path(ClipMachineMemory, c1, p, len))
	       return 1;
	    strncat(p, fn, len - strlen(p) - 1);
	    _clip_unix_path(p, ClipMachineMemory->flags & TRANSLATE_FLAG);
	    c1 = c + 1;
	 }
      }
   }
   return 0;
}
