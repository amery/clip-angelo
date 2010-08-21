/*
 * ������� ������� ������� �������
 * ���� � ��������� ���� ����� �����, �� �������� � ������� ����
 */
int
clip_DIRCHANGE(ClipMachine * ClipMachineMemory)
{
/*
	 0      NO_DISK_ERR         No error occurred
	-2      ER_FILE_NOT_FOUND   File not found
	-3      ER_PATH_NOT_FOUND   Path not found
	-5      ER_ACCESS_DENIED    Access denied (e.g., in network)
*/
   char tmp[MAXPATHLEN], *uname;

   char *drv = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   long hash_dir = _hash_cur_dir[*drv - 65];

   char *dname = _clip_parc(ClipMachineMemory, 1);

   _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
   if (dname == NULL)
      return 0;

#ifdef _WIN32
   if (SetCurrentDirectory(dname) <= 0)
      return 0;
#endif
   if (strlen(dname) > 2 && *(dname + 1) == ':')
   {
      drv = calloc(1, 3);
      drv[0] = toupper(*dname);
      if (drv[0] < 'A' || drv[0] > 'Z')
	 drv[0] = 'C';
      drv[1] = ':';
      /*
         _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
       */
      hash_dir = _hash_cur_dir[*drv - 65];
      dname += 2;
   }
#ifdef _WIN32
   {
      char ndir[MAXPATHLEN];

      char *ch;

      int len;

      len = GetCurrentDirectory(MAXPATHLEN, ndir);
      if (len > 0)
      {
	 ch = (char *) calloc(len - 1, 1);
	 memcpy(ch, ndir + 2, len - 1);
      }
      else
	 ch = (char *) calloc(1, 1);
      _clip_store_item(ClipMachineMemory, hash_dir, ch);
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
      return 0;
   }
#endif
   if (*dname == '\\' || *dname == '/')
   {
      tmp[0] = drv[0];
      tmp[1] = ':';
      strcpy(tmp + 2, dname);
      uname = _get_unix_name(ClipMachineMemory, tmp);
   }
   else
   {
      if (*dname == '.' && (*(dname + 1) == '.'))
      {
	 int i;

	 tmp[0] = drv[0];
	 tmp[1] = ':';
	 tmp[2] = '\\';
	 strcpy(tmp + 3, _clip_fetch_item(ClipMachineMemory, hash_dir));
	 for (i = 0; tmp[i] != 0; i++);
	 for (; i > 2 && tmp[i] != '\\' && tmp[i] != '/'; i--);
	 strcpy(tmp + i, dname + 2);
	 uname = _get_unix_name(ClipMachineMemory, tmp);
      }
      else
      {

	 tmp[0] = drv[0];
	 tmp[1] = ':';
	 /*snprintf(tmp + 2,MAXPATHLEN,"%s\\%s",_clip_fetch_item(ClipMachineMemory, hash_dir), dname); */
	 strcpy(tmp + 2, _clip_fetch_item(ClipMachineMemory, hash_dir));
	 strcat(tmp, "\\");
	 strcat(tmp, dname);
	 uname = _get_unix_name(ClipMachineMemory, tmp);
	 //printf("\n\ndname=%s,uname=%s\n",dname,uname);
      }
   }

   if (uname == NULL)
   {
      _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      return 1;
   }

   //printf("\nunix_dir=%s\n",uname);
   _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   if (chdir(uname))
   {
      _check_error(ClipMachineMemory, uname, 1);	/* fail */
   }
   else
   {
      /*
       * ����� �������� unix �������, ������ ������������� �������
       * ���� (���� ����) � ������� ��������� ���� �� ���� �����
       */
      char *dir = NULL, *ndir = NULL;

      if (*dname == '\\' || *dname == '/')
      {				/* ���������� ���� dos */
	 int len;

	 drv = strdup(dname);
	 len = strlen(drv);
	 if (len > 3 && (drv[len - 1] == '/' || drv[len - 1] == '\\'))
	    drv[len - 1] = 0;
	 _clip_store_item(ClipMachineMemory, hash_dir, drv);
      }
      else
      {				/* ���� ������������, ������� ����� */
	 int dlen = 0;

	 dir = _clip_fetch_item(ClipMachineMemory, hash_dir);
	 dlen = strlen(dir);
	 ndir = (char *) calloc(dlen + strlen(dname) + 2, 1);
	 memcpy(ndir, dir, dlen);
	 if (*dname == '.' && (*(dname + 1) == '.'))
	 {
	    int i;

	    for (i = 0; ndir[i] != 0; i++);
	    for (; i > 0 && ndir[i] != '\\' && ndir[i] != '/'; i--);

	    if (dname[2] == '\\' || dname[2] == '/')
	       strcpy(ndir + i, dname + 3);
	    else
	       strcpy(ndir + i, dname + 2);
	 }
	 else
	 {
	    if (dlen > 1)
	       ndir[dlen++] = '\\';
	    strcpy(ndir + dlen, dname);
	 }
	 dlen = strlen(ndir);
	 if (dlen > 3 && (ndir[dlen - 1] == '/' || ndir[dlen - 1] == '\\'))
	    ndir[dlen - 1] = 0;
	 _clip_store_item(ClipMachineMemory, hash_dir, ndir);
      }

      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   }

   free(uname);
   return 0;
}
