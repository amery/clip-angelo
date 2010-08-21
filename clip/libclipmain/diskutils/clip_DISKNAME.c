int
clip_DISKNAME(ClipMachine * ClipMachineMemory)
{
   char drv[2] = { 0, 0 };
   char *dname = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (dname == NULL)
   {
      _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "unknown disk");
		/*_clip_trap_printf(ClipMachineMemory, __FILE__, __LINE__, "unknown disk");*/
      _clip_retc(ClipMachineMemory, "");
      return 1;
   }
   drv[0] = *dname;
   _clip_retc(ClipMachineMemory, drv);
   return 0;
}
