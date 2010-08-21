int
clip_SETCANCEL(ClipMachine * ClipMachineMemory)
{
   int l = -1;

   char *str = NULL;

   _clip_retl(ClipMachineMemory, !ClipMachineMemory->notcancel);

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMachineMemory, 1);
   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 2) == LOGICAL_type_of_ClipVarType)
      l = _clip_parl(ClipMachineMemory, 2);
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
      str = _clip_parc(ClipMachineMemory, 2);

   if (str != NULL)
      l = !strcasecmp(str, "on");

   if (l >= 0)
   {
      if (ClipMachineMemory->fullscreen)
	 setCtrlBreak_Screen(ClipMachineMemory->screen, l);
      ClipMachineMemory->notcancel = !l;
   }

   return 0;
}
