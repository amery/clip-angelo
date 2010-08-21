int
clip_NEXTKEY(ClipMachine * ClipMachineMemory)
{
   int eventmask = _clip_parni(ClipMachineMemory, 1);

   if (_clip_parinfo(ClipMachineMemory, 1) != NUMERIC_type_of_ClipVarType)
      eventmask = ClipMachineMemory->eventmask;

   _clip_fullscreen(ClipMachineMemory);
   if (*ClipMachineMemory->kbdptr != ClipMachineMemory->kbdbuf)
      _clip_retndp(ClipMachineMemory, (*ClipMachineMemory->kbdptr)[-1], 10, 0);
   else
   {
      long key;

      int ckey, l;

      key = getWait_Key(ClipMachineMemory->screen->base, 0);
      if (key)
      {
	 ckey = key2clip(key, eventmask);
	 if ((l = *ClipMachineMemory->kbdptr - ClipMachineMemory->kbdbuf) >= ClipMachineMemory->typeahead)
	 {
	    ClipMachineMemory->typeahead *= 2;
	    ClipMachineMemory->kbdbuf = (int *) realloc(ClipMachineMemory->kbdbuf, sizeof(int) * ClipMachineMemory->typeahead);

	    *ClipMachineMemory->kbdptr = ClipMachineMemory->kbdbuf + l;
	 }
	 *(*ClipMachineMemory->kbdptr)++ = ckey;

	 _clip_retndp(ClipMachineMemory, ckey, 10, 0);
      }
      else
	 _clip_retndp(ClipMachineMemory, 0, 10, 0);
   }
   return 0;
}
