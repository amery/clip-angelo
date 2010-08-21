int
clip_CTOD(ClipMachine * ClipMachineMemory)
{
   char *str = _clip_parc(ClipMachineMemory, 1);

   char *format = _clip_parc(ClipMachineMemory, 2);

   if (str == NULL)
   {
      _clip_retdj(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOD");
   }
   if (format == NULL)
      format = ClipMachineMemory->date_format;
   _clip_retdj(ClipMachineMemory, _clip_str_to_date(str, format, ClipMachineMemory->epoch));
   return 0;
}
