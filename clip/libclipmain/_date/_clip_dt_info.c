int
_clip_dt_info(ClipMachine * ClipMachineMemory, int info)
{
   int len, is_dt = 1;

   ClipDateTime *dt;

   char *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      is_dt = 0;

   if (info == 0)
   {
      _clip_retl(ClipMachineMemory, is_dt);
      return 0;
   }
   if (!is_dt)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_INFO");

   dt = (ClipDateTime *) tmp;
   switch (info)
   {
   case 1:
      _clip_retni(ClipMachineMemory, dt->year);
      break;
   case 2:
      _clip_retni(ClipMachineMemory, dt->month);
      break;
   case 3:
      _clip_retni(ClipMachineMemory, dt->day);
      break;
   case 4:
      _clip_retni(ClipMachineMemory, dt->hour);
      break;
   case 5:
      _clip_retni(ClipMachineMemory, dt->min);
      break;
   case 6:
      _clip_retni(ClipMachineMemory, dt->sec);
      break;
   case 7:
      _clip_retni(ClipMachineMemory, dt->msec);
      break;
   default:
      _clip_retni(ClipMachineMemory, 0);
      break;
   }
   return 0;
}
