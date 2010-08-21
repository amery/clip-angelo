int
clip_I2BIN(ClipMachine * ClipMachineMemory)
{
   char buf[3];

   char *e;

   short i = (short) _clip_parni(ClipMachineMemory, 1);

   e = (char *) (&i);
   buf[0] = e[0];
   buf[1] = e[1];
   buf[2] = 0;
   _clip_retcn(ClipMachineMemory, buf, 2);
   return 0;
}
