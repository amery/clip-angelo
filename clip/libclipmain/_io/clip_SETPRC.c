int
clip_SETPRC(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->argc > 0)
      ClipMachineMemory->prow = _clip_parni(ClipMachineMemory, 1);
   if (ClipMachineMemory->argc > 1)
      ClipMachineMemory->pcol = _clip_parni(ClipMachineMemory, 2);
   return 0;
}
