int
clip_DISKREADYW(ClipMachine * ClipMachineMemory)
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
   {
      _clip_retl(ClipMachineMemory, 0);
   }
   else
   {
#ifdef OS_MINGW
      _clip_retl(ClipMachineMemory, 0);
#else
      uid_t uid = geteuid();

      gid_t gid = getegid();

      if ((uid == st.st_uid && (st.st_mode & S_IWUSR)) || (gid == st.st_gid && (st.st_mode & S_IWGRP)) || (st.st_mode & S_IWOTH))
      {
	 _clip_retl(ClipMachineMemory, 1);
      }
      else
      {
	 _clip_retl(ClipMachineMemory, 0);
      }
#endif
   }
   return 0;
}
