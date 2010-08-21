/*
 * ���������� ���� � �������� ������� - ������ ���������� ����� dos
 */
char *
_get_disk_path(ClipMachine * ClipMachineMemory, char *dname)
{
   char drv[3] = { 0, ':', 0 };

   if (dname == NULL || (*(dname + 1) != 0 && *(dname + 1) != ':'))
      return NULL;

   switch (*dname)
   {
   case 'a'...'z':
      drv[0] = toupper(*dname);
      break;
   case 'A'...'Z':
      drv[0] = *dname;
      break;
   }

   if (!drv[0])
      return NULL;
   return _clip_fetch_item(ClipMachineMemory, _clip_hashstr(drv));
}
