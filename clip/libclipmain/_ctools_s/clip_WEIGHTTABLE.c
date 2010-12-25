int
clip_WEIGHTTABLE(ClipMachine * ClipMachineMemory)
{
   _clip_retcn(ClipMachineMemory, (char *) _clip_cmptbl, 256);
   return 0;
}
