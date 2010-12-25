int
clip_B2BIN(ClipMachine * ClipMachineMemory)
{
   char buf[2];

   char i = (char) _clip_parni(ClipMachineMemory, 1);

   buf[0] = i;
   buf[2] = 0;
   _clip_retcn(ClipMachineMemory, buf, 1);
   return 0;
}
