int
clip_DOSERROR(ClipMachine * ClipMachineMemory)
{
	/*_clip_retni(ClipMachineMemory, errno);*/
   _clip_retni(ClipMachineMemory, 0);
   return 0;
}
