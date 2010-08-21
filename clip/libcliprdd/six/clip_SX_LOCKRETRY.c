int
clip_SX_LOCKRETRY(ClipMachine * ClipMachineMemory)
{
/* dummy */
   ClipMachineMemory->m6_error = 0;
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}
