static int
_clip_calc_crc(ClipMachine * ClipMachineMemory, int type)
{
   long hash;

   int len;

   char *str = _clip_parcl(ClipMachineMemory, 1, &len);

   if (str == NULL)
   {
      _clip_retnl(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "HASHSTR");
   }

   if (type == 32)
      hash = _clip_hashbytes32(0, str, len);
   else
      hash = _clip_hashbytes16(0, str, len);

   _clip_retnl(ClipMachineMemory, hash);

   return 0;
}
