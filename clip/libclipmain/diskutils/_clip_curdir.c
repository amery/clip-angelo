char *
_clip_curdir(ClipMachine * ClipMachineMemory, char *drv)
{
   int i;

   char *dir;

   dir = _clip_fetch_item(ClipMachineMemory, _hash_cur_dir[drv[0] - 65]);
   if (dir == NULL /* || *dir==0 */ )
      return "";
   for (i = 0; dir[i] != 0; i++)
      if (dir[i] == '\\')
	 dir[i] = '/';
   if (*dir == '/')
      dir++;
   return dir;
}
