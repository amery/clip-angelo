#ifdef _WIN32
int
clip_GETANSICP(ClipMachine * ClipMachineMemory)
{
   char     *s = _clip_win_cp(2);

   _clip_retcn_m(ClipMachineMemory, s, strlen(s));
   return 0;
}
#else
int
clip_GETANSICP(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_hostcs);
   return 0;
}
#endif
