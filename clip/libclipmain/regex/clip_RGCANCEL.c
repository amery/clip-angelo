int
clip_RGCANCEL(ClipMachine * ClipMachineMemory)
{
   int item = _clip_parni(ClipMachineMemory, 1);	/* container pattern */

   _clip_destroy_c_item(ClipMachineMemory, item, _C_ITEM_TYPE_REGEX);
   return 0;
}
