
/*
 * ��� �������� �������� ��������, �������� 1 �������� -- ��� �����,
 * ���� ��� ���, �� ������� ������� ����
 */
int
clip_DIRNAME(ClipMachine * ClipMachineMemory)
{
   char *buf = (char *) malloc(MAXPATHLEN);

   char *dname = _clip_parc(ClipMachineMemory, 1);

   char *drv = dname ? dname : _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   char *dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[toupper(*drv) - 65]);

   int i = 0, n = 0;

   if (dir == NULL)
   {
      buf[0] = '\\';
      buf[1] = 0;
   }
   else
   {
      for (i = 0, n = 0; dir[i]; i++, n++)
	 buf[n] = (dir[i] == '/' ? '\\' : dir[i]);
      buf[n] = 0;
   }
   _clip_retcn_m(ClipMachineMemory, buf, strlen(buf));
   return 0;
}
