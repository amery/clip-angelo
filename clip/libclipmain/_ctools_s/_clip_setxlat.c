int
_clip_setxlat(ClipMachine * ClipMachineMemory, unsigned char *data)
{
   int no = _clip_parni(ClipMachineMemory, 1) % 256;

   unsigned char *s = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   _clip_retl(ClipMachineMemory, 0);
   if (s != NULL)
      no = *s;
   if (_clip_parinfo(ClipMachineMemory, 0) == 0)
   {
      int i;

      for (i = 0; i < 256; i++)
	 data[i] = i;
      _clip_retl(ClipMachineMemory, 1);
      return 0;
   }
   if (_clip_parinfo(ClipMachineMemory, 0) >= 2)
   {
      int len;

      unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &len);

      if ((no + len) > 256)
	 len = 256 - no;
      memcpy(data + no, str, len);
      _clip_retl(ClipMachineMemory, 1);
      return 0;
   }
   return 0;
}
