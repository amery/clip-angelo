int
clip_DT_CREATE(ClipMachine * ClipMachineMemory)
{
   int ww;

   ClipDateTime *dt;

   dt = malloc(sizeof(ClipDateTime));
   memset(dt, 0, sizeof(ClipDateTime));
   dt->sign = _C_ITEM_TYPE_DATETIME;

   if (_clip_parinfo(ClipMachineMemory, 1) == DATE_type_of_ClipVarType)
      _clip_pardc(ClipMachineMemory, 1, &(dt->year), &(dt->month), &(dt->day), &ww);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
   {
      char *tmp = _clip_parc(ClipMachineMemory, 1);

      dt->year = atol(tmp);
      for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
      if (*tmp != 0)
	 dt->month = atol(tmp + 1);
      for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
      if (*tmp != 0)
	 dt->day = atol(tmp + 1);
   }
   dt->hour = _clip_parni(ClipMachineMemory, 2);
   dt->min = _clip_parni(ClipMachineMemory, 3);
   dt->sec = _clip_parni(ClipMachineMemory, 4);
   dt->msec = _clip_parni(ClipMachineMemory, 5);

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
   {
      char *tmp = _clip_parc(ClipMachineMemory, 2);

      dt->hour = atol(tmp);
      for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
      if (*tmp != 0)
	 dt->min = atol(tmp + 1);
      for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
      if (*tmp != 0)
	 dt->sec = atol(tmp + 1);
      for (tmp++; *tmp != 0 && *tmp >= '0' && *tmp <= '9'; tmp++);
      if (*tmp != 0)
	 dt->msec = atol(tmp + 1);
   }

   if (_clip_parinfo(ClipMachineMemory, 1) == NUMERIC_type_of_ClipVarType)
   {
      dt->year = _clip_parni(ClipMachineMemory, 1);
      dt->month = _clip_parni(ClipMachineMemory, 2);
      dt->day = _clip_parni(ClipMachineMemory, 3);
      dt->hour = _clip_parni(ClipMachineMemory, 4);
      dt->min = _clip_parni(ClipMachineMemory, 5);
      dt->sec = _clip_parni(ClipMachineMemory, 6);
      dt->msec = _clip_parni(ClipMachineMemory, 7);
   }
   /*
      _clip_dt_normalize(dt);
    */
   _clip_retcn_m(ClipMachineMemory, (char *) dt, sizeof(ClipDateTime) - 1);
   return 0;
}
