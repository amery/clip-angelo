/*
 * ���������� ��� ������� �������� � �������, �������������
 * ��� Clipper-������ � ����������� �� errno � unix,
 * ��� ����������� �����������
 */

void
_check_error(ClipMachine * ClipMachineMemory, const char *fname, int isdir)
{
#define NO_DISK_ERR		0
#define ER_FILE_NOT_FOUND	-2
#define ER_PATH_NOT_FOUND	-3
#define ER_ACCESS_DENIED	-5

   char *path = NULL, *tmp;

   switch (errno)
   {
   case ENOENT:
      path = _get_path(fname);
      if (isdir)
	 tmp = (char *) fname;
      else
	 tmp = path;
      if (tmp == NULL || access(tmp, F_OK))
	 _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      else
	 _clip_retni(ClipMachineMemory, ER_FILE_NOT_FOUND);
      if (path != NULL)
	 free(path);
      return;
   case ENOTDIR:
      _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      return;
   default:
      _clip_retni(ClipMachineMemory, ER_ACCESS_DENIED);
   }
   return;
}
