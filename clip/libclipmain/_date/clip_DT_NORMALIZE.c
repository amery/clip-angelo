int
clip_DT_NORMALIZE(ClipMachine * ClipMachineMemory)
{
   int len;

   ClipDateTime *dt;

   ClipDateTime *dt2;

   char *tmp = _clip_parcl(ClipMachineMemory, 1, &len);

   if (tmp == NULL || len != sizeof(ClipDateTime) - 1 || *tmp != _C_ITEM_TYPE_DATETIME)
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "DT_NORMALIZE");

   dt = (ClipDateTime *) tmp;

   _clip_dt_normalize(dt);

   dt2 = malloc(sizeof(ClipDateTime));
   memcpy(dt2, dt, sizeof(ClipDateTime));

   _clip_retcn_m(ClipMachineMemory, (char *) dt2, sizeof(ClipDateTime) - 1);
   return 0;
}
