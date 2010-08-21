/* boldattr([lEnable]) -> prevstate */
int
clip_BOLDATTR(ClipMachine * ClipMachineMemory)
{
   int prevState = -1;

   if (_clip_parinfo(ClipMachineMemory, 1) == LOGICAL_type_of_ClipVarType)
      prevState = _clip_parl(ClipMachineMemory, 1);

   _clip_fullscreen(ClipMachineMemory);
   setAttr_Screen(ClipMachineMemory->screen, 0, &prevState);

   _clip_retl(ClipMachineMemory, prevState);
   return 0;
}
