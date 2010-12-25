int
clip_WEIGHTASC(ClipMachine * ClipMachineMemory)
{
   char *s = _clip_parc(ClipMachineMemory, 1);

   if (s == NULL)
      _clip_retni(ClipMachineMemory, 0);
   else
   {
      int ch = *(unsigned char *) s;

      _clip_retni(ClipMachineMemory, _clip_cmptbl[ch]);
   }
   return 0;
}
