int
clip_L2BIN(ClipMachine * ClipMachineMemory)
{
   char buf[5];

   char *e;

   long i = (long) _clip_parnl(ClipMachineMemory, 1);

   e = (char *) (&i);
   buf[0] = e[0];
   buf[1] = e[1];
   buf[2] = e[2];
   buf[3] = e[3];
   buf[4] = 0;
   _clip_retcn(ClipMachineMemory, buf, 4);
   return 0;
}
