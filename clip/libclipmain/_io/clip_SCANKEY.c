int
clip_SCANKEY(ClipMachine * ClipMachineMemory)
{
   unsigned long key;

   long ms;

   if (ClipMachineMemory->argc < 1)
   {
      ms = 0;
   }
   else
   {
      double t = _clip_parnd(ClipMachineMemory, 1);

      if (t > 0)
	 ms = t * 1000;
      else if (t == 0)
	 ms = 1000 * 6000;
      else
	 ms = 0;
   }

   dialog_init(ClipMachineMemory);
   sync_mp(ClipMachineMemory);

   key = getRawWait_Key(ClipMachineMemory->screen->base, ms);
   _clip_retnl(ClipMachineMemory, key);

   return 0;
}
