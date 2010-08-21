int
clip_TASKSTART(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   Task_START();
#endif
   return 0;
}
