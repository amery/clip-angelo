int
clip_FERROR(ClipMachine * ClipMachineMemory)
{
   int *err;

   err = _clip_fetch_item(ClipMachineMemory, HASH_ferror);
   _clip_retni(ClipMachineMemory, *err);
   return 0;
}
