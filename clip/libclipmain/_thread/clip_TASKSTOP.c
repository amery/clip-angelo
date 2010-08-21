int
clip_TASKSTOP(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   Task_STOP();
#endif
   return 0;
}
