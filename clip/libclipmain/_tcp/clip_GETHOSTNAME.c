/*****************************/
int
clip_GETHOSTNAME(ClipMachine * ClipMachineMemory)
{
   /* return host name of process */
   char buf[256];

   if (gethostname(buf, 255) != 0)
      _clip_retc(ClipMachineMemory, "");
   else
      _clip_retc(ClipMachineMemory, buf);
   return 0;
}
