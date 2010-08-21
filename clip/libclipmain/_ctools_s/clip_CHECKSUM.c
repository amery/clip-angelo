int
clip_CHECKSUM(ClipMachine * ClipMachineMemory)
{
   unsigned char *str = (unsigned char *) _clip_parc(ClipMachineMemory, 1);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CHECKSUM");
   }
   _clip_retnl(ClipMachineMemory, _clip_hashstr((char *) str));
   return 0;
}
