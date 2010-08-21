int
clip_FAHRENHEIT(ClipMachine * ClipMachineMemory)
{
   _clip_retnd(ClipMachineMemory, _clip_parnd(ClipMachineMemory, 1) * 9 / 5 + 32);
   return 0;
}
