int
clip_ASC(ClipMachine * ClipMachineMemory)
{
   unsigned char *str = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   int ch = 0;

   if (str)
      ch = *str;

   _clip_retni(ClipMachineMemory, ch);

   return 0;
}
