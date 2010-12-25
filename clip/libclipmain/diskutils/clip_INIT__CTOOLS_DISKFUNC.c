/*
 * - ����� ����������� ������ hash-����� ��� ������� ������� ��������
 *   ���� �� ������� ����� (���� - ������ 'x_CUR_DIR', ��� ������ 'x'
 *   ������������� ����� �������� �����), ������������ ������ hash-�����
 *   ����� ��������� ��� <���_�����_�����> - 65
 * - ��������������� ��� �� ������������ ����� "C:" ����� � ��������
 *   ������� unix
 * - �� ����� "C:" ��������������� ������� ������� ������� -- �������� ("\")
 */

int
clip_INIT__CTOOLS_DISKFUNC(ClipMachine * ClipMachineMemory)
{
   long i, j;

   char *s = NULL, hstr[10];

   int *f_attr = NULL;

#ifdef _WIN32
   LPTSTR currpath;

   LPTSTR savepath;

   LPTSTR cDisk;
#else
   long len;
#endif
   /*
    * ���������� ������� _hash_cur_dir ��� �������� ��������
    * ������� � ������� ����� �� ��������� ������ � ���������
    * ��������� ����� ��� ���� ������ � ������
    */
   strcpy(hstr, "__CUR_DIR");
   for (i = 0, j = 'A'; i < 26; i++, j++)
   {
      hstr[0] = (char) j;
      _hash_cur_dir[i] = _clip_hashstr(hstr);
      s = (char *) calloc(1, 2);
      *s = '\\';
      _clip_store_item(ClipMachineMemory, _hash_cur_dir[i], s);
   }

   /*
    * ��������� ������� ������� unix, � ���� �� �������� ������������
    * ������-���� ���������� �����, ������ ���� ���� ������� � �������������
    * ������� ������� dos
    */

#ifdef _WIN32

   savepath = malloc(sizeof(LPTSTR) * (MAXPATHLEN + 1));
   i = GetCurrentDirectory(MAXPATHLEN, savepath);
   cDisk = malloc(sizeof(LPTSTR) * 3);
   cDisk[0] = savepath[0];
   if (cDisk[0] >= 'a' && cDisk[0] <= 'z')
      cDisk[0] = cDisk[0] - 32;
   cDisk[1] = ':';
   cDisk[2] = 0;
   /* set current drive for translation path */
   _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, cDisk);

   cDisk = malloc(sizeof(LPTSTR) * 3);
   cDisk[1] = ':';
   cDisk[2] = 0;
   currpath = malloc(sizeof(LPTSTR) * (MAXPATHLEN + 1));
   for (i = 'C'; i <= 'Z'; i++)
   {
      cDisk[0] = i;
      if (SetCurrentDirectory(cDisk) <= 0)
	 continue;
      j = GetCurrentDirectory(MAXPATHLEN, currpath);
#if 0
      if (j > 3)
      {
	 char *ch = (char *) calloc(MAXPATHLEN, 1);

	 cygwin_win32_to_posix_path_list(currpath, ch);
	 _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
      }
      else if (j == 3)
      {
	 char *ch = (char *) calloc(1, 1);

	 _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
      }
#else
      if (j > 0)
      {
	 char *ch = (char *) calloc(j - 1, 1);

	 memcpy(ch, currpath + 2, j - 1);
	 _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);
      }
#endif
   }
   i = SetCurrentDirectory(savepath);
   free(savepath);
   free(currpath);
   free(cDisk);

#else
   if ((s = (char *) calloc(MAXPATHLEN, 1)))
   {

      /* ������ ������� ���� unix */
      if (getcwd(s, MAXPATHLEN))
      {
	 char *d = NULL, *path = strdup(s);

	 hstr[1] = ':';
	 hstr[2] = 0;
	 len = strlen(s);

	 /* ������� ���� ��������� ������ */
	 for (i = 'A'; i <= 'Z'; i++)
	 {
	    hstr[0] = i;
	    if ((d = _clip_fetch_item(ClipMachineMemory, _clip_hashstr(hstr))) == NULL)
	       continue;
	    if ((j = strlen(d)) > len)
	       continue;
	    if (!memcmp(path, d, j))
	    {
	       char *ch = (char *) calloc(3, 1);

	       *ch = i;		/* ����� ����� */
	       *(ch + 1) = ':';

	       /* ��������� �������� ����� */
	       /*
	          if (_clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE) == NULL)
	        */
	       _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, ch);

	       if (path[j] == '/')
		  j++;
	       _clip_translate_path(ClipMachineMemory, path + j, s, MAXPATHLEN);
	       ch = (char *) calloc(len - j + 2, 1);
	       *ch = '\\';
	       memcpy(ch + 1, path + j, len - j);

	       /* ��������� �������� ���������� ���� */
	       _clip_store_item(ClipMachineMemory, _hash_cur_dir[i - 65], ch);

	       //   break;
	    }
	 }
	 free(path);
      }
      free(s);
   }

   /*
    * ��������� ������� ���������� � ������� ��������� �����, ���� �� ���,
    * �� ������������� ������� C:
    */
#endif
   if ((s = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE)) == NULL)
   {
      s = (char *) calloc(1, 3);
      memcpy(s, "C:", 3);
      _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, s);
   }

   i = *s - 65;			/* ������ � _hash_cur_dir ��� �������� ����� */
   j = _clip_hashstr(s);	/* hash-��� ����� �������� ����� */
   /*
    * ��������� ����������� �� ����� �������� ���������� ���� � unix,
    * ���� �� ���, ������������� � �������� ������� unix
    */
   if ((s = _clip_fetch_item(ClipMachineMemory, j)) == NULL)
   {
      s = (char *) calloc(1, 2);
      *s = '/';
      _clip_store_item(ClipMachineMemory, j, s);	/* set( "?:", "/" ) */
   }

   /*
    * ������� ��� �������� ������ �� ���������
    */
   f_attr = (int *) malloc(sizeof(int));

   *f_attr = FA_ARCHIVE;
   _clip_store_item(ClipMachineMemory, CLIP_CA_FCREATE_ATTR, f_attr);

   /*
    * ����� �������� ������ �� ���������
    */
   f_attr = (int *) calloc(1, sizeof(int));

   *f_attr = LOCK_RDONLY;
   _clip_store_item(ClipMachineMemory, CLIP_CA_SHARE_MODE, f_attr);

   return 0;
}
