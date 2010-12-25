#ifdef _WIN32
int
clip_GETACP(ClipMachine * ClipMachineMemory)
{
   return clip_GETANSICP(ClipMachineMemory);
}
#else
int
clip_GETACP(ClipMachine * ClipMachineMemory)
{
   return clip_GETANSICP(ClipMachineMemory);
}
#endif
