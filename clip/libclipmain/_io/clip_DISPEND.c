int
clip_DISPEND(ClipMachine * ClipMachineMemory)
{
   ClipMachineMemory->update--;
   if (_clip_debuglevel)
      sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
   if (ClipMachineMemory->update <= 0)
   {
      ClipMachineMemory->update = 0;
      if (ClipMachineMemory->fullscreen)
	 sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
#ifdef USE_TASKS
      clip_TASKSTART(ClipMachineMemory);
#endif
   }
   return 0;
}
