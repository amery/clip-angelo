void
_clip_exit(void)
{
   /* close all ClipMachines */

   if (exited)
      return;
   exited = 1;

   while (first_mp)
   {
      ClipMachine *ClipMachineMemory = first_mp;

      first_mp = first_mp->next;
      delete_ClipMachine(ClipMachineMemory);
   }
   _clip_logg(2, "exit");
   if (logg != stderr)
      fclose(logg);

#ifdef MEMDEBUG
   malloc_shutdown();
#endif

}
