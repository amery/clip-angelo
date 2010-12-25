int
clip_DISPBEGIN(ClipMachine * ClipMachineMemory)
{
#ifdef USE_TASKS
   if (!ClipMachineMemory->update)
      clip_TASKSTOP(ClipMachineMemory);
#endif
   ClipMachineMemory->update++;
   return 0;
}
