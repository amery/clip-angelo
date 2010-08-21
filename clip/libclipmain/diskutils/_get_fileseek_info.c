/*
 * ���� ���� ��� �����, �� �������������� ���,
 * ����� ������� ������� � ������ FILESEEK
 */
char *
_get_fileseek_info(ClipMachine * ClipMachineMemory, char **fname, struct stat *st)
{
   char *uname = ClipMachineMemory->argc > 0 ? _get_unix_name(ClipMachineMemory,
							      _clip_parc(ClipMachineMemory,
									 1)) : NULL;

   FSdata *fsd = NULL;

   if (uname != NULL)
   {
      if (stat(uname, st))
      {
	 _clip_retni(ClipMachineMemory, 0);
	 return NULL;
      }
      if ((*fname = strrchr(uname, '/')) == NULL)
      {
	 *fname = uname;
      }
      else
      {
	 char *tmp = *fname;

	 *fname = ++tmp;
      }
   }
   else
   {
      if ((fsd = _clip_fetch_item(ClipMachineMemory, HASH_fsdata)) == NULL)
      {
	 _clip_retni(ClipMachineMemory, 0);
	 return NULL;
      }
      *fname = fsd->fname[fsd->pos];
      *st = fsd->st[fsd->pos];
   }
   return uname;
}
