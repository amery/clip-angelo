int
clip_GETSHARE(ClipMachine * ClipMachineMemory)	/* Determines the file open (share) mode */
{
   int *smode = _clip_fetch_item(ClipMachineMemory, CLIP_CA_SHARE_MODE);

   _clip_retni(ClipMachineMemory, smode ? *smode : LOCK_RDONLY);
   return 0;
}
