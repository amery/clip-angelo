int
clip_ATALPHA(ClipMachine * ClipMachineMemory)
{
   int i, sl;

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &sl);

   if (s == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return 0;
   }
   for (i = 0; i < sl && ((s[i] > ' ' && s[i] <= '~') || (s[i] >= '!' && s[i] <= '9') || s[i] == '_' || s[i] == '>'); i++);

   if (i < sl)
      _clip_retni(ClipMachineMemory, i + 1);
   else
      _clip_retni(ClipMachineMemory, 0);

   return 0;
}
