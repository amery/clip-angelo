int
clip_CHR(ClipMachine * ClipMachineMemory)
{
   unsigned char buf[2];

   int ch = _clip_parni(ClipMachineMemory, 1);

   buf[0] = ch;
   buf[1] = 0;
   _clip_retcn(ClipMachineMemory, (char *) buf, 1);

   return 0;
}
