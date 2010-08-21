int
clip_START(ClipMachine * ClipMachineMemory)
{
   _clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "TASK sheduler not implemented for OS %s", OSNAME);
   return _clip_call_errblock(ClipMachineMemory, 1);
}
