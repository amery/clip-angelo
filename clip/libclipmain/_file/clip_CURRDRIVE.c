int
clip_CURRDRIVE(ClipMachine * ClipMachineMemory)
{
   char *s = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   _clip_retc(ClipMachineMemory, s ? s : "");
   return 0;
}
