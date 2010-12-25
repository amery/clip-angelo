int
clip_SX_SETTRIGGER(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "SX_SETTRIGGER";

   int action = _clip_parni(ClipMachineMemory, 1);

   const char *trigger = _clip_parc(ClipMachineMemory, 2);

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(2, CHARACTER_type_of_ClipVarType);

   _clip_retc(ClipMachineMemory, wa->trigger);
   switch (action)
   {
   case TRIGGER_ENABLE:
      if (wa->trighash)
	 wa->trig_active = 1;
      break;
   case TRIGGER_DISABLE:
      wa->trig_active = 0;
      break;
   case TRIGGER_REMOVE:
      if (wa->trigger)
      {
	 free(wa->trigger);
	 wa->trigger = NULL;
      }
      wa->trighash = 0;
      break;
   case TRIGGER_INSTALL:
      if (trigger && trigger[0])
      {
	 if (wa->trigger)
	 {
	    free(wa->trigger);
	    wa->trigger = NULL;
	 }
	 wa->trighash = 0;
	 wa->trigger = strdup(trigger);
	 _clip_upstr(wa->trigger, strlen(wa->trigger));
	 wa->trighash = _clip_hashstr(wa->trigger);
	 wa->trig_active = 1;
      }
      break;
   }
   return 0;
 err:
   return er;
}
