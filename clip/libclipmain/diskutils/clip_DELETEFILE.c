/*
 * ������� ����
 */
int
clip_DELETEFILE(ClipMachine * ClipMachineMemory)
{
/*
   0      NO_DISK_ERR         No error occurs
   -2      ER_FILE_NOT_FOUND   File not found
   -3      ER_PATH_NOT_FOUND   Path not found
   -5      ER_ACCESS_DENIED    Access denied (e.g., file is read-only)
 */
   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
   {
      _clip_retni(ClipMachineMemory, ER_PATH_NOT_FOUND);
      return 1;
   }
   if (unlink(uname))
      _check_error(ClipMachineMemory, uname, 0);	/* fail */
   else
      _clip_retni(ClipMachineMemory, NO_DISK_ERR);	/* success */
   free(uname);
   return 0;
}
