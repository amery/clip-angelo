int
clip_DT_ADD(ClipMachine * ClipMachineMemory)
{
   int len1, len2;

   ClipDateTime *dt1, *dt2, *dt;

   char *tmp1 = _clip_parcl(ClipMachineMemory, 1, &len1);

   char *tmp2 = _clip_parcl(ClipMachineMemory, 2, &len2);

   if (tmp1 == NULL || len1 != sizeof(ClipDateTime) - 1 || *tmp1 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_ADD");
   if (tmp2 == NULL || len2 != sizeof(ClipDateTime) - 1 || *tmp2 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_ADD");

   dt1 = (ClipDateTime *) tmp1;
   dt2 = (ClipDateTime *) tmp2;

   dt = malloc(sizeof(ClipDateTime));
   dt->sign = _C_ITEM_TYPE_DATETIME;
   dt->year = dt1->year + dt2->year;
   dt->month = dt1->month + dt2->month;
   dt->day = dt1->day + dt2->day;
   dt->hour = dt1->hour + dt2->hour;
   dt->min = dt1->min + dt2->min;
   dt->sec = dt1->sec + dt2->sec;
   dt->msec = dt1->msec + dt2->msec;

   /*
      _clip_dt_normalize(dt);
    */

   _clip_retcn_m(ClipMachineMemory, (char *) dt, sizeof(ClipDateTime) - 1);
   return 0;
}
