int
clip_DT_DATE(ClipMachine * ClipMachineMemory)
{
   int len;

   ClipDateTime *dt, *dt2;

   char *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_DATE");

   dt = (ClipDateTime *) tmp;
   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));
   _clip_dt_normalize(dt2);
   _clip_retdc(ClipMachineMemory, dt2->year, dt2->month, dt2->day);
   free(dt2);
   return 0;
}
