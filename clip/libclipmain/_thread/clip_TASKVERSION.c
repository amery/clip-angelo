int
clip_TASKVERSION(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   _clip_retni(ClipMachineMemory, Task_version());
#endif
   return 0;
}
