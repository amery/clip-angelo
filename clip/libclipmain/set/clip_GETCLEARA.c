int
clip_GETCLEARA(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, *((int *) _clip_fetch_item(ClipMachineMemory, HASH_setcleara)));
   return 0;
}
