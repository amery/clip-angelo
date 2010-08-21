int
clip_STRFINDBOL(ClipMachine * ClipMachineMemory)
{
   int i, sl;

   char *s = _clip_parcl(ClipMachineMemory, 1, &sl);

   int begpos = _clip_parni(ClipMachineMemory, 2);

   if (s == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   if (begpos < 1 || begpos > sl)
      begpos = sl + 1;

   begpos--;
   if (s[begpos] == '\n')
      begpos--;
   for (i = begpos; i >= 0; i--)
   {
      if (s[i] == '\n')
      {
	 i++;
	 break;
      }
   }
   _clip_retni(ClipMachineMemory, i + 1);
   return 0;
}
