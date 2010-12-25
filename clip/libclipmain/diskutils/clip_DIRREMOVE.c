/*
 * �������� ��������, �������� �������� �� ������� �������,
 * �.�. ��� �� ��������� �������� ������� ������
 */
int
clip_DIRREMOVE(ClipMachine * ClipMachineMemory)
{
/*
   0       Directory has been successfully removed.
   -3      Access path not found or directory not empty.
   -5      Access denied (e.g., in a network)
   -16     Current directory (cannot be removed)
 */
   struct stat st, stcur;

   char *uname = _get_unix_name(ClipMachineMemory, _clip_parc(ClipMachineMemory, 1));

   if (uname == NULL)
   {
      _clip_retni(ClipMachineMemory, -3);
      return 1;
   }

   if (stat(uname, &st))
   {
      _clip_retni(ClipMachineMemory, errno == ENOENT ? -3 : -5);
      goto ret;
   }

   if (stat(".", &stcur))
   {
      _clip_retni(ClipMachineMemory, -5);
      goto ret;
   }

   /*
    * �������� �� ������� �������
    */
   if (st.st_ino == stcur.st_ino)
   {
      _clip_retni(ClipMachineMemory, -16);
      goto ret;
   }

   if (!rmdir(uname))		/* success */
      _clip_retni(ClipMachineMemory, 0);
   else
      _clip_retni(ClipMachineMemory, -3);

 ret:
   free(uname);
   return 0;
}
