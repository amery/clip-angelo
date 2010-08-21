int
clip_DIRECTORY(ClipMachine * ClipMachineMemory)
{
   char *str = _clip_parc(ClipMachineMemory, 1);

   char *attr = _clip_parc(ClipMachineMemory, 2);

   int i, *err, flag_dir = 0;

   char buf[PATH_MAX];

   char dirn[PATH_MAX + 1];

   char filen[PATH_MAX + 1];

   char fullname[PATH_MAX + 1];

   char buftime[9];

   struct dirent *dirp;

   struct stat statbuf;

   DIR *dirh;

   ClipVar var, *empty;

   ClipVar *rp = RETPTR(ClipMachineMemory);

   struct tm *sysTime;

   long vect[2];

   vect[0] = 0;
   vect[1] = 0;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   if (attr == NULL || strlen(attr) == 0)
      /*     flag_dir = 1 */ ;
   else
   {
      for (i = 0; i < strlen(attr) && !flag_dir; i++)
	 flag_dir = (attr[i] == 'D');
   }

   if (str == NULL || strlen(str) == 0)
   {
      strcpy(filen, "*.*");
      getcwd(dirn, sizeof(dirn));
   }
   else
   {
      _clip_translate_path(ClipMachineMemory, str, buf, sizeof(buf));
      i = _clip_parse_path(buf, dirn, filen);
#ifdef _WIN32
      if (i < 0)
	 GetCurrentDirectory(PATH_MAX, dirn);
#else
      if (i < 0)
	 getcwd(dirn, sizeof(dirn));
#endif
   }
   dirh = opendir(*dirn == 0 ? "/" : dirn);

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);

   if (dirh == NULL)
      *err = errno;
   else
      *err = 0;

   if (dirh == NULL)
   {
      return 0;
   }
   dirp = readdir(dirh);
   i = 0;
   while (dirp != NULL)
   {
#ifdef _WIN32
      if (_clip_glob_match(dirp->d_name, filen, 1) <= 0)
#else
      if (_clip_glob_match(dirp->d_name, filen, 0) <= 0)
#endif
      {
	 dirp = readdir(dirh);
	 continue;
      }
      snprintf(fullname, sizeof(fullname), "%s/%s", dirn, dirp->d_name);
      stat(fullname, &statbuf);
      if (S_ISDIR(statbuf.st_mode) && !flag_dir)
      {
	 dirp = readdir(dirh);
	 continue;
      }
      empty = malloc(sizeof(ClipVar));
      vect[0] = 5;
      _clip_array(ClipMachineMemory, empty, 1, vect);
      vect[0] = i + 1;
      _clip_asize(ClipMachineMemory, rp, 1, vect);
      vect[0] = i;
      _clip_aset(ClipMachineMemory, rp, empty, 1, vect);

      vect[1] = 0;
      memset(&var, 0, sizeof(ClipVar));
      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = dirp->d_name;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(dirp->d_name);
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      memset(&var, 0, sizeof(ClipVar));
      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = statbuf.st_size;
      vect[1] = 1;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      memset(&var, 0, sizeof(ClipVar));

      sysTime = localtime(&statbuf.st_mtime);
      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = DATE_type_of_ClipVarType;
      var.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = _clip_jdate(sysTime->tm_mday, sysTime->tm_mon + 1, sysTime->tm_year + 1900);
      vect[1] = 2;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      memset(&var, 0, sizeof(ClipVar));
      snprintf(buftime, 9, "%02d:%02d:%02d", sysTime->tm_hour, sysTime->tm_min, sysTime->tm_sec);
      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = buftime;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 8;
      vect[1] = 3;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      memset(&var, 0, sizeof(ClipVar));
      buftime[1] = 0;
      if (S_ISDIR(statbuf.st_mode))
	 buftime[0] = 'D';
      else
	 buftime[0] = 'A';
      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = buftime;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 1;
      vect[1] = 4;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);
      _clip_destroy(ClipMachineMemory, empty);
      free(empty);

      dirp = readdir(dirh);
      i++;
   }
   closedir(dirh);

   return 0;
}
