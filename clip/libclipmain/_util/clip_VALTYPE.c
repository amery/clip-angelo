int
clip_VALTYPE(ClipMachine * ClipMachineMemory)
{

   _clip_retc(ClipMachineMemory, _clip_type_name(_clip_par(ClipMachineMemory, 1)));
   return 0;
}
