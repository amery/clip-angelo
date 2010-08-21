int
clip_COLORSELECT(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->colorSelect = _clip_parni(ClipMachineMemory, 1);
   return 0;
}
