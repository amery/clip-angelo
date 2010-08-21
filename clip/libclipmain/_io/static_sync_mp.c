static void
sync_mp(ClipMachine * ClipMachineMemory)
{
   if (!ClipMachineMemory->update && !ClipMachineMemory->inkey)
   {
      if (ClipMachineMemory->fullscreen)
	 sync_Screen(ClipMachineMemory->screen, ClipMachineMemory->flags1 & UTF8TERM_FLAG);
      else if (ClipMachineMemory->flags1 & FLUSHOUT_FLAG)
	 fflush((FILE *) ClipMachineMemory->out);
   }
}
