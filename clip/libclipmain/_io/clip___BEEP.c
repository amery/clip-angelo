int
clip___BEEP(ClipMachine * ClipMachineMemory)
{
   if (ClipMachineMemory->fullscreen)
      ClipMachineMemory->screen->beeps++;
   else
      fputc('\007', (FILE *) ClipMachineMemory->out);

   sync_mp(ClipMachineMemory);
   return 0;
}
