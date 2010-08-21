int
clip___ACCEPTSTR(ClipMachine * ClipMachineMemory)
{
   int c;

   if ((c = ClipMachineMemory->history.count_of_ClipVect))
      _clip_retc(ClipMachineMemory, ClipMachineMemory->history.items_of_ClipVect[c - 1]);
   else
      _clip_retc(ClipMachineMemory, "");
   return 0;
}
