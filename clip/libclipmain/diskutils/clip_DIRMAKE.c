
/*
 * ������� �������, � ���� ����� ���� ����� �����
 */
int
clip_DIRMAKE(ClipMachine * ClipMachineMemory)
{
/*
   0      NO_DISK_ERR         No error occurred
   -2      ER_FILE_NOT_FOUND   File not found
   -3      ER_PATH_NOT_FOUND   Path not found
   -5      ER_ACCESS_DENIED    Access denied (e.g., in network)
 */
   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
   {
      _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      return 1;
   }

   /*
    * ������� ������� �� ����� ������� ��� ����, ��� ���������
    * � ����������� �� ��������� umask ��� ����� �����
    */
#ifdef OS_MINGW
   if (mkdir(uname))
#else
   if (mkdir(uname, ClipMachineMemory->dirCreateMode))
#endif
      _check_error(ClipMachineMemory, uname, 1);	/* fail */
   else
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */

   free(uname);
   return 0;
}
