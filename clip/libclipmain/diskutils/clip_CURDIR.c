int
clip_CURDIR(ClipMachine * ClipMachineMemory)
{
   char drv[3], *tmp;

   char *disk = _clip_fetch_item(ClipMachineMemory, CLIP_CUR_DRIVE);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
   {
      tmp = _clip_parc(ClipMachineMemory, 1);
      drv[0] = toupper(*tmp);
   }
   else
      drv[0] = *disk;
   drv[1] = ':';
   drv[2] = 0;
   _clip_retc(ClipMachineMemory, _clip_curdir(ClipMachineMemory, drv));
   return 0;
}
