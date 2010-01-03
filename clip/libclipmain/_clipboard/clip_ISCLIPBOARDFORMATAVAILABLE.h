#ifdef _WIN32
int
clip_ISCLIPBOARDFORMATAVAILABLE(ClipMachine * ClipMachineMemory)
{
   _clip_retl(ClipMachineMemory, IsClipboardFormatAvailable(_clip_parni(ClipMachineMemory, 1)));
   return 0;
}
#else
int
clip_ISCLIPBOARDFORMATAVAILABLE(ClipMachine * ClipMachineMemory)
{
   int       f = _clip_parni(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, f > 0 && f < CF_MAX);
   return 0;
}
#endif
