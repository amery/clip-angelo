int
clip_ISLOWER(ClipMachine * ClipMachineMemory)
{
   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (!s)
      _clip_retl(ClipMachineMemory, 0);
   else
   {
      int ch = *s;

      _clip_retl(ClipMachineMemory, (_clip_isalpha_tbl[ch] && ch == _clip_lowtbl[ch]));
		/*_clip_retl(ClipMachineMemory, (ch == _clip_lowtbl[ch]));*/
   }

   return 0;
}
