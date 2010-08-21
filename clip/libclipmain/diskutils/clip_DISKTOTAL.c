int
clip_DISKTOTAL(ClipMachine * ClipMachineMemory)
{
#ifdef OS_MINGW
   DWORD dt;
#else
   struct statfs st;
#endif
   char *dname = _clip_parc(ClipMachineMemory, 1);

   char *uname;

   if (dname == NULL || *dname == 0)
      dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   uname = _get_disk_path(ClipMachineMemory, dname);

   if (uname == NULL)
      uname = dname;

   if (uname == NULL)
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }

#ifdef OS_MINGW
   GetDiskFreeSpace(uname, NULL, NULL, NULL, &dt);
   _clip_retnd(ClipMachineMemory, (double) dt);
#else
   if (statfs(uname, &st))
   {
      _clip_retnd(ClipMachineMemory, 0);
      return 0;
   }
   _clip_retnd(ClipMachineMemory, ((double) st.f_blocks) * st.f_bsize);
#endif

   return 0;
}
