int
clip_COUNTLEFT(ClipMachine * ClipMachineMemory)
{
   int l1, l2, ret = 0;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l1);

   unsigned char *s = (unsigned char *) _clip_parcl(ClipMachineMemory, 2, &l2);

   int ch = _clip_parni(ClipMachineMemory, 2);

   if (str == NULL || l1 == 0)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "COUNTLEFT");
   }
   if (s != NULL)
      ch = *s;
   if (ch == 0)
      ch = ' ';

   for (ret = 0; ret < l1 && str[ret] == ch; ret++);

   _clip_retni(ClipMachineMemory, ret);
   return 0;
}
