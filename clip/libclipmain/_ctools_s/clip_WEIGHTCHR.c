int
clip_WEIGHTCHR(ClipMachine * ClipMachineMemory)
{
   int i;

   unsigned char ch = _clip_parni(ClipMachineMemory, 1);

   for (i = 0; i < 256 && _clip_cmptbl[i] != ch; i++)
      ;
   _clip_retcn(ClipMachineMemory, (char *) &i, 1);
   return 0;
}
