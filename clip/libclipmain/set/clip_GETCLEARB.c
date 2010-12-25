int
clip_GETCLEARB(ClipMachine * ClipMachineMemory)
{
   _clip_retni(ClipMachineMemory, *((int *) _clip_fetch_item(ClipMachineMemory, HASH_setclearb)));
   return 0;
}
