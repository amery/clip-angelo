int
clip_VALPOS(ClipMachine * ClipMachineMemory)
{
   int l;

   int ret = 0;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   int num = _clip_parni(ClipMachineMemory, 2);

   if (str == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "VALPOS");
   }
   if (_clip_parinfo(ClipMachineMemory, 0) < 2)
      num = l;
   if (num > 0 && num <= l)
      ret = str[num - 1];
   if (ret >= '0' && ret <= '9')
      ret = ret - '0';
   else
      ret = 0;
   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
