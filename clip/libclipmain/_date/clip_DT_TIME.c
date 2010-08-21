int
clip_DT_TIME(ClipMachine * ClipMachineMemory)
{
   int len;

   ClipDateTime *dt, *dt2;

   char buf[32];

   char *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_TIME");

   dt = (ClipDateTime *) tmp;
   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));
   _clip_dt_normalize(dt2);
   snprintf(buf, 31, "%02d:%02d:%02d:%02d", dt2->hour, dt2->min, dt2->sec, dt2->msec);
   free(dt2);
   _clip_retc(ClipMachineMemory, buf);
   return 0;
}
