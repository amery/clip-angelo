int
clip_CSETREF(ClipMachine * ClipMachineMemory)
{
   char *status;

   status = _clip_fetch_item(ClipMachineMemory, HASH_csetref);
   _clip_retl(ClipMachineMemory, *status == 't');
   if (_clip_parinfo(ClipMachineMemory, 0) > 0)
   {
      if (_clip_parl(ClipMachineMemory, 1))
	 *status = 't';
      else
	 *status = 'f';
   }
   return 0;
}
