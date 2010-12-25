/* ����� �������� ���������� ��� FILESEEK */
int
clip_FILESEEK(ClipMachine * ClipMachineMemory)	/* Searches for files by name and attribute */
{
/*
   0       FA_NORMAL
   1       FA_READONLY         READ ONLY (Read-only)
   2       FA_HIDDEN           HIDDEN (Hidden files)
   4       FA_SYSTEM           SYSTEM (System files)
   8       FA_VOLUME           VOLUME (Name of a floppy/hard disk)
   16      FA_DIRECTORY        DIR (Directory)
   32      FA_ARCHIVE          ARCHIVE (Changes since last backup)
 */

   char *dname = _clip_parc(ClipMachineMemory, 1);

   int fattr = _clip_parni(ClipMachineMemory, 2);

   /*int exact = _clip_parl(ClipMachineMemory, 3); */
   char *uname = NULL, *mask = NULL, *fullname = NULL, *filename = NULL;

   int i = 0, valid = 0, ret = 0;

   FSdata *fsd = NULL;

   DIR *dir = NULL;

   struct dirent *entry = NULL;

   if (dname)
      uname = _get_unix_name(ClipMachineMemory, dname);

   if (!uname || !*uname)
   {
      if ((fsd = _clip_fetch_item(ClipMachineMemory, HASH_fsdata)) == NULL)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 ret = _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, inv_arg);
			/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "invalid argument");*/
	 goto end;
      }
   }

   if (fsd == NULL)
   {
      fsd = (FSdata *) calloc(sizeof(FSdata), 1);
      if ((mask = strrchr(uname, '/')) == NULL || *(mask + 1) == 0)
      {
	 mask = "*";
      }
      else
      {
	 *mask++ = 0;
      }

      if ((dir = opendir((*uname) == 0 ? "/" : uname)) != NULL)
      {
	 fullname = (char *) calloc(MAXPATHLEN, 1);
	 strcpy(fullname, uname);
	 strcat(fullname, "/");
	 filename = fullname + strlen(fullname);
      }
      else
      {
	 _clip_retni(ClipMachineMemory, 0);
	 /*
	    ret = _clip_trap_err( ClipMachineMemory, EG_OPEN, 0, 0, __FILE__, __LINE__, "cannot open directory" );
	  */
	 goto end;
      }

      for (i = 0; (entry = readdir(dir)) != NULL;)
      {
	 if (fsd->fname == NULL || fsd->fname[i] == 0)
	 {
	    fsd->fname = (char **) realloc(fsd->fname, (i + 64) * sizeof(char *));
	    fsd->st = (struct stat *) realloc(fsd->st, (i + 64) * sizeof(struct stat));

	    fsd->fname[i] = 0;
	 }
#ifdef _WIN32
	 if (_clip_glob_match(entry->d_name, mask, 1) != strlen(entry->d_name))
#else
	 if (_clip_glob_match(entry->d_name, mask, ClipMachineMemory->flags & TRANSLATE_FLAG) != strlen(entry->d_name))
#endif
	    continue;
	 strcpy(filename, entry->d_name);
	 stat(fullname, &(fsd->st[i]));
	 valid = 0;
	 if (fattr & FA_DIRECTORY)
	 {
	    if (S_ISDIR(fsd->st[i].st_mode))
	    {
	       fsd->fname[i++] = strdup(entry->d_name);
	       fsd->fname[i] = 0;
	       continue;
	    }
	 }
	 else
	 {
	    if (S_ISDIR(fsd->st[i].st_mode))
	       continue;
	 }
	 if (fattr == 0)
	 {
	    valid = 1;
	 }
	 else if (fattr & FA_ARCHIVE && S_ISREG(fsd->st[i].st_mode))
	    valid = 1;
	 if (fattr & FA_READONLY)
	 {
	    if ((fsd->st[i].st_mode & S_IRUSR) && !(fsd->st[i].st_mode & S_IWUSR))
	       valid = 1;
	 }
	 else if (fattr & FA_HIDDEN)
	 {
	    if (entry->d_name[0] == '.')
	       valid = 1;
	 }
	 else if (FA_SYSTEM || FA_VOLUME)
	 {
	    ;
	 }
	 else
	 {
	    valid = 1;
	 }

	 if (valid)
	 {
	    fsd->fname[i++] = strdup(entry->d_name);
	    fsd->fname[i] = 0;
	 }
      }

      closedir(dir);

      if (fsd->fname)
	 fsd->fname[i] = 0;

      _clip_store_item_destroy(ClipMachineMemory, HASH_fsdata, fsd, destroy_FSdata);

   }
   else if (fsd->fname[fsd->pos])
   {
      fsd->pos++;
   }

   _clip_retc(ClipMachineMemory, fsd->fname[fsd->pos]);

 end:
   if (uname)
      free(uname);
   if (fullname)
      free(fullname);

   return ret;
}
