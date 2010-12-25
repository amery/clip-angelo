CLIP_DLLEXPORT int
_clip_translate_path(ClipMachine * ClipMachineMemory, const char *str, char *buf, int buflen)
{
#ifdef _WIN32
   if ((memcmp(str, "\\\\", 2) == 0) || (memcmp(str, "//", 2) == 0))
   {
      /* network resource */
      strncpy(buf, str, buflen);
      return 0;
   }

   if (str != NULL && (*str == '/' || *str == '\\') && memcmp(str, "/cygdrive/", 10))
   {
      /* add default drive */
      char *s = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

      if (ClipMachineMemory->rootpath == NULL)
	 snprintf(buf, buflen - 1, "%s%s", s, str);
      else
      {
	 char *_str = NULL;

	 _str = _clip_normalize_path((char *) str);
	 snprintf(buf, buflen - 1, "%s%s%s", s, ClipMachineMemory->rootpath, _str);
	 free(_str);
      }
      return 0;
   }
   if (str != NULL && strlen(str) >= 2 && str[1] == ':' && str[2] != '/' && str[2] != '\\')
   {
      /* drive with default path */
      char *def_path;

      def_path = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[toupper(*str) - 65]);
      snprintf(buf, buflen - 1, "%c:%s%s\\%s", *str, ClipMachineMemory->rootpath, def_path, str + 2);
      return 0;
   }
   if (ClipMachineMemory->rootpath == NULL)
      snprintf(buf, buflen - 1, "%s", str);
   else
   {
      char *_str = NULL;

      char *tmp = strdup(str);

      _str = _clip_normalize_path(tmp);
      snprintf(buf, buflen - 1, "%s%s", ClipMachineMemory->rootpath, _str);
      free(_str);
      free(tmp);
   }
   return 0;
#else
   char *s, *e;

   char *root = 0;

   int bl, ch;

   buf[0] = 0;
   if (strchr(str, '|'))
   {
      strncpy(buf, str, buflen);
      buf[buflen - 1] = 0;
      return 0;
   }

   if ((s = strchr(str, ':')) && s == str + 1)
   {
      char dn[3];

      long dh;

      dn[0] = toupper(*str);
      dn[1] = ':';
      dn[2] = 0;
      dh = _clip_hashstr(dn);
      if ((root = _clip_fetch_item(ClipMachineMemory, dh)) == NULL)
      {
	 return -1;
      }

      str = s + 1;
      while ((ch = *str) && (ch == '\\' || ch == '/'))
	 ++str;
   }

   bl = 0;
   if (root)
   {
      snprintf(buf, buflen - 1, "%s", root);
      bl = strlen(buf);
      if (bl > 0 && (buf[bl - 1] == '/' || buf[bl - 1] == '\\'))
	 --bl;
      buf[bl++] = '/';
   }
   /*
      else if (str[0] == '.' && str[1] == '.')
      {
      strncpy(buf, str, buflen);
      return 1;
      }
    */
   else if (*str != '/' /*&& *str != '\\' */ )
   {
      if (ClipMachineMemory->rootpath == NULL)
      {
	 getcwd(buf, buflen);
	 bl = strlen(buf);
	 buf[bl++] = '/';
      }
   }
   if (ClipMachineMemory->rootpath != NULL)
   {
      char *_str = NULL;

      char *tmp = strdup(buf);

      snprintf(buf, buflen, "%s%s", ClipMachineMemory->rootpath, tmp);
      free(tmp);
      bl = strlen(buf);

      tmp = strdup(str);
      _str = (char *) _clip_normalize_path(tmp);
      free(tmp);
      snprintf(buf + bl, buflen - bl, "%s", _str);
      free(_str);
   }
   else
   {
      snprintf(buf + bl, buflen - bl, "%s", str);
   }
   bl = strlen(buf);
   for (e = buf + bl, s = buf; s < e; ++s)
   {
      if (*s == '\\')
	 *s = '/';
   }
   s = strrchr(buf, '/');
   if (s && s != buf)
   {
      char ocwd[PATH_MAX];

      char fname[PATH_MAX];

      char ch;

      getcwd(ocwd, sizeof(ocwd));

      strncpy(fname, s + 1, sizeof(fname));

      ch = *s;
      *s = 0;
      if (chdir(buf) == 0)
      {
	 getcwd(buf, buflen);
	 strcat(buf, "/");
	 strcat(buf, fname);
	 chdir(ocwd);
      }
      else
	 *s = ch;
   }
   bl = strlen(buf);
   if (ClipMachineMemory->flags & TRANSLATE_FLAG)
   {
#if 1
      int i, j;

      for (i = 0, j = 0; i < bl; i++)
      {
	 if (buf[i] == ' ')
	    continue;
	 buf[j] = tolower((unsigned char) (buf[i]));
	 j++;
      }
      buf[j] = 0;
#else
      char *tmp;

      for (e = buf + bl, s = buf; s < e; ++s)
	 *s = tolower(*(unsigned char *) s);
      for (tmp = buf + strlen(buf) - 1; *tmp == ' ' && tmp >= buf; tmp--);
      tmp++;
      *tmp = 0;
#endif
   }
   return 0;
#endif
}
