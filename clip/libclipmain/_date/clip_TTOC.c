int
clip_TTOC(ClipMachine * ClipMachineMemory)
{
   ClipVar *r = RETPTR(ClipMachineMemory);

   char *date_format = _clip_parc(ClipMachineMemory, 2);

   long time = 0;

   long julian;

   if (_clip_parinfo(ClipMachineMemory, 1) == DATE_type_of_ClipVarType)
      julian = _clip_pardj(ClipMachineMemory, 1);
   else if (_clip_parinfo(ClipMachineMemory, 1) == DATETIME_type_of_ClipVarType)
      julian = _clip_pardtj(ClipMachineMemory, 1, &time);
   else
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "TTOC");

   if (date_format == NULL || *date_format == 0)
      date_format = ClipMachineMemory->date_format;

   memset(r, 0, sizeof(ClipVar));
   r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
      _clip_ttoc(ClipMachineMemory, julian, time, &r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, date_format, ClipMachineMemory->hours, ClipMachineMemory->seconds);
   return 0;
}
