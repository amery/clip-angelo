#ifdef _WIN32
int
clip_GETOEMCP(ClipMachine * ClipMachineMemory)
{
   char     *s = _clip_win_cp(3);

   _clip_retcn_m(ClipMachineMemory, s, strlen(s));
   return 0;
}
#else
int
clip_GETOEMCP(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_hostcs);
   return 0;
}
#endif
