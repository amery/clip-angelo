int
clip_DISKFREE(ClipMachine * ClipMachineMemory)
{
   char *dname = _clip_parc(ClipMachineMemory, 1);

   char *uname;

#ifdef OS_MINGW
   DWORD fs;

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   uname = _get_disk_name(ClipMachineMemory, dname);
   GetDiskFreeSpace(uname, NULL, NULL, &fs, NULL);
   _clip_retnd(ClipMachineMemory, ((double) fs) * fs);
#else
   struct statfs st;

   char drv[3];

   char *tmp, buf[PATH_MAX];

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (*dname == '/')
      uname = dname;
   else
   {
      drv[0] = *dname;
      drv[1] = ':';
      drv[2] = 0;
      tmp = _clip_curdir(ClipMachineMemory, drv);

      buf[0] = drv[0];
      buf[1] = ':';
      buf[2] = '/';
      strncpy(buf + 3, tmp, PATH_MAX - 4);
      uname = _get_unix_name(ClipMachineMemory, buf);
   }

   if (uname == NULL)
      uname = dname;

   if (uname == NULL)
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }

   if (statfs(uname, &st))
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }
   _clip_retnd(ClipMachineMemory, ((double) st.f_bavail) * st.f_bsize);
#endif

   return 0;
}
