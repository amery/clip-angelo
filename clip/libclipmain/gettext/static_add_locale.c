static void
add_locale(char *module, char *filename)
{
   Locale *lp;

   int fd, j, ind;

   int use_mmap = 0;

   size_t size;

   struct stat st;

   struct mo_file_header *data = (struct mo_file_header *) -1;

   nls_uint32 revision;

   char path[256];

   char loc[32];

   char *locale = "";

   char *charset = 0;

   lp = (Locale *) calloc(1, sizeof(Locale));
   lp->module = strdup(module);

   if (search_Coll(&locales, lp, &ind))
   {
		delete_Locale((Locale *) locales.items_of_Coll[ind]);
		locales.items_of_Coll[ind] = lp;
   }
   else if (!insert_Coll(&locales, lp))
   {
      delete_Locale(lp);
      return;
   }
   path[0] = 0;
   if (filename)
   {
      snprintf(path, sizeof(path), "%s", filename);
   }
   else
   {
		for (j = 0; j < locale_names.count_of_Coll; j++)
      {
	 char *e;

	 DIR *dir;

	 if (charset)
	 {
	    free(charset);
	    charset = 0;
	 }
	 	 locale = locale_names.items_of_Coll[j];
	 snprintf(loc, sizeof(loc), "%s", locale);
	 snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, loc, module);

	 if (!m_access(path, R_OK))
	 {
	    e = strrchr(loc, '.');
	    if (e)
	       charset = strdup(e + 1);
	    break;
	 }

	 e = strrchr(loc, '.');
	 if (e)
	    *e = 0;
	 snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, loc, module);

	 if (!m_access(path, R_OK))
	 {
	    if (e)
	       charset = strdup(e + 1);
	    break;
	 }

	 e = strrchr(loc, '_');
	 if (e)
	    *e = 0;

	 snprintf(path, sizeof(path), "%s/locale.mo", CLIP_LOCALE_ROOT);
	 dir = opendir(path);
	 if (dir)
	 {
	    struct dirent *dp;

	    int br = 0;

	    while ((dp = readdir(dir)))
	    {
	       if (strncmp(dp->d_name, loc, strlen(loc)))
		  continue;

	       snprintf(path, sizeof(path), "%s/locale.mo/%s/%s.mo", CLIP_LOCALE_ROOT, dp->d_name, module);

	       if (!m_access(path, R_OK))
	       {
		  br = 1;
		  e = strrchr(dp->d_name, '.');
		  if (e)
		     charset = strdup(e + 1);
		  break;
	       }
	    }
	    closedir(dir);
	    if (br)
	       break;
	    else
	       path[0] = 0;
	 }
      }
   }

   lp->name = strdup(locale);
   if (*path == 0)
   {
      _clip_logg(2, "locale settings not found for module '%s'", module);
      return;
   }
   if (m_access(path, R_OK))
   {
      _clip_logg(2, "no access to locale file '%s': %s", path, strerror(errno));
      return;
   }

   fd = open(path, O_RDONLY | O_BINARY);
   if (fd < 0)
   {
      _clip_logg(1, "cannot open locale file '%s': %s", path, strerror(errno));
      return;
   }
   else
      _clip_logg(3, "open locale file '%s', charset '%s'", path, charset);

   if (fstat(fd, &st) != 0 || ((size = st.st_size) < sizeof(struct mo_file_header)))
   {
      close(fd);
      _clip_logg(1, "cannot stat locale file '%s': %s", path, strerror(errno));
      return;
   }

#ifndef NO_MMAP
   data = (struct mo_file_header *) mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
   if (data != (struct mo_file_header *) -1)
   {
      /* mmap() call was successful.  */
#ifndef OS_CYGWIN
/* this f...ed win32 require(undocumented, of couse!) that fd must be keeped OPEN while use mmaped memory...*/
      close(fd);
      fd = -1;
#endif
      use_mmap = 1;
   }
#endif
   /* If the data is not yet available (i.e. mmap'ed) we try to load
      it manually.  */

   if (data == (struct mo_file_header *) -1)
   {
      size_t to_read;

      char *read_ptr;

      data = (struct mo_file_header *) malloc(size);

      to_read = size;
      read_ptr = (char *) data;
      do
      {
	 long int nb = (long int) read(fd, read_ptr, to_read);

	 if (nb == -1)
	 {
	    close(fd);
	    _clip_logg(1, "cannot read locale file '%s': %s", path, strerror(errno));
	    return;
	 }

	 read_ptr += nb;
	 to_read -= nb;
      }
      while (to_read > 0);

      close(fd);
      fd = -1;
   }

   /* Using the magic number we can test whether it really is a message
      catalog file.  */
	if (data->magic_of_mo_file_header != _MAGIC && data->magic_of_mo_file_header != _MAGIC_SWAPPED)
   {
      /* The magic number is wrong: not a message catalog file.  */
#ifndef NO_MMAP
      if (use_mmap)
	 munmap((caddr_t) data, size);
      else
#endif
	 free(data);
      if (fd != -1)
	 close(fd);
      _clip_logg(1, "wrong magic number of locale file '%s'", path);
      return;
   }

   lp->use_mmap = use_mmap;
   lp->mmap_size = size;
	lp->must_swap = data->magic_of_mo_file_header != _MAGIC;
   lp->data = (char *) data;
   lp->fd = fd;
   lp->charset = charset;

   /* Fill in the information about the available tables.  */
	revision = W(lp->must_swap, data->revision_of_mo_file_header);
   switch (revision)
   {
   case 0:
		lp->nstrings = W(lp->must_swap, data->nstrings_of_mo_file_header);
		lp->orig_tab = (struct string_desc *) ((char *) data + W(lp->must_swap, data->orig_tab_offset_of_mo_file_header));
		lp->trans_tab = (struct string_desc *) ((char *) data + W(lp->must_swap, data->trans_tab_offset_of_mo_file_header));
		lp->hash_size = W(lp->must_swap, data->hash_tab_size_of_mo_file_header);
		lp->hash_tab = (nls_uint32 *) ((char *) data + W(lp->must_swap, data->hash_tab_offset_of_mo_file_header));
      break;
   default:
      _clip_logg(1, "illegal revision of locale file '%s': %d", path, revision);
#ifndef NO_MMAP
      if (use_mmap)
	 munmap((caddr_t) data, size);
      else
#endif
	 free(data);
      if (fd != -1)
	 close(fd);
      free(lp);
      return;
   }

   lp->ok = 1;
   lp->nullentry = find_msg(lp, "", 0);

   if (lp->nullentry)
   {
      char *plural;

      char *nplurals;

      plural = strstr(lp->nullentry, "plural=");
      nplurals = strstr(lp->nullentry, "nplurals=");
      if (plural && nplurals)
      {
	 char *endp;

	 unsigned long int n;

	 int l;

	 /* First get the number.  */
	 nplurals += 9;
	 while (*nplurals != '\0' && isspace((unsigned char) *nplurals))
	    ++nplurals;
	 if (!(*nplurals >= '0' && *nplurals <= '9'))
	    goto no_plural;
	 for (endp = nplurals, n = 0; *endp >= '0' && *endp <= '9'; endp++)
	    n = n * 10 + (*endp - '0');
	 if (nplurals == endp)
	    goto no_plural;
	 lp->nplural = n;

	 plural += 7;
	 l = strcspn(plural, ";\n\r");
	 lp->pd = plural_new(plural, l);
      }
    no_plural:
      charset = strstr(lp->nullentry, "charset=");
      if (charset)
      {
	 int l;

	 charset += 8;
	 l = strcspn(charset, ";\n\r \t");
	 if (lp->charset)
	    free(lp->charset);
	 lp->charset = (char *) malloc(l + 1);
	 memcpy(lp->charset, charset, l);
	 lp->charset[l] = 0;
      }
   }

   if (lp->charset)
   {
      char *s;

      for (s = lp->charset; *s; s++)
	 *s = tolower(*s);
   }
}
