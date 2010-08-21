int
clip_DISKREADY(ClipMachine * ClipMachineMemory)
{
   struct stat st;

   char *dname = _clip_parc(ClipMachineMemory, 1);

   char *uname = _get_disk_path(ClipMachineMemory, dname);

   if (dname == NULL || *dname == 0)
   {
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);
      uname = _get_disk_path(ClipMachineMemory, dname);
   }
   if (uname == NULL || stat(uname, &st))
      _clip_retl(ClipMachineMemory, 0);
   else
      _clip_retl(ClipMachineMemory, 1);

   return 0;
}
