int
clip_DT_CMP(ClipMachine * ClipMachineMemory)
{
   int ret = 0, len1, len2;

   char *_dt1, *_dt2;

   ClipDateTime *dt1, *dt2;

   _dt1 = _clip_parcl(ClipMachineMemory, 1, &len1);
   if (_dt1 == NULL || len1 != sizeof(ClipDateTime) - 1 || *_dt1 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_CMP");
   _dt2 = _clip_parcl(ClipMachineMemory, 2, &len2);
   if (_dt2 == NULL || len2 != sizeof(ClipDateTime) - 1 || *_dt2 != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_CMP");

   dt1 = (ClipDateTime *) _dt1;
   dt2 = (ClipDateTime *) _dt2;
   _clip_dt_normalize(dt1);
   _clip_dt_normalize(dt2);

   if (ret == 0)
      ret = dt1->year - dt2->year;
   if (ret == 0)
      ret = dt1->month - dt2->month;
   if (ret == 0)
      ret = dt1->day - dt2->day;
   if (ret == 0)
      ret = dt1->hour - dt2->hour;
   if (ret == 0)
      ret = dt1->min - dt2->min;
   if (ret == 0)
      ret = dt1->sec - dt2->sec;
   if (ret == 0)
      ret = dt1->msec - dt2->msec;

   _clip_retnl(ClipMachineMemory, ret);
   return 0;
}
