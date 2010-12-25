/*
 * ����� �������� �����, (A, B, C, D, ...),
 * ����� ����� ���� � ������������ �����������
 */
int
clip_DISKCHANGE(ClipMachine * ClipMachineMemory)
{
   struct stat st;

   char *drv = NULL;

   char *dname = _clip_parc(ClipMachineMemory, 1);

   char *uname = _get_disk_path(ClipMachineMemory, dname);

   if (uname == NULL || stat(uname, &st) || !S_ISDIR(st.st_mode))
   {
      _clip_retl(ClipMachineMemory, 0);
      return 0;
   }

   drv = (char *) calloc(3, 1);
   drv[1] = ':';
   switch (*dname)
   {
   case 'a'...'z':
      drv[0] = toupper(*dname);
      break;
   case 'A'...'Z':
      drv[0] = *dname;
      break;
   }
#ifdef _WIN32
   if (SetCurrentDirectory(drv) > 0)
   {
      _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
      _clip_retl(ClipMachineMemory, 1);
   }
   else
      _clip_retl(ClipMachineMemory, 0);
#else
   _clip_store_item(ClipMachineMemory, CLIP_CUR_DRIVE, drv);
   _clip_retl(ClipMachineMemory, 1);
#endif
   return 0;
}
