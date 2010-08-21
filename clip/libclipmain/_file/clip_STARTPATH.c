int
clip_STARTPATH(ClipMachine * ClipMachineMemory)
{
   char buf[PATH_MAX];

   memset(buf, 0, PATH_MAX);

   if (_clip_argv[0][0] == '/')
   {
      memcpy(buf, _clip_argv[0], strlen((char *) _clip_argv[0]));
   }
   else
   {
      getcwd(buf, sizeof(buf));
      memcpy(buf + strlen(buf) + 1, _clip_argv[0], strlen((char *) _clip_argv[0]) + 1);
      buf[strlen(buf)] = '/';
   }
#ifdef OS_CYGWIN
   {
      char ch[MAXPATHLEN];

      cygwin_posix_to_win32_path_list(buf, ch);
      _clip_retc(ClipMachineMemory, ch);
   }
#else
   _clip_retc(ClipMachineMemory, buf);
#endif
   return 0;
}
