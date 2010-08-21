#ifdef _WIN32
int
clip_GETCONSOLECP(ClipMachine * ClipMachineMemory)
{
   char *s = _clip_win_cp(1);

   _clip_retcn_m(ClipMachineMemory, s, strlen(s));
   return 0;
}
#else
int
clip_GETCONSOLECP(ClipMachine * ClipMachineMemory)
{
   _clip_retc(ClipMachineMemory, _clip_getenv("CLIP_CLIENTCS"));
   return 0;
}
#endif
