int
clip_HASHNAME(ClipMachine * ClipMachineMemory)
{
   long hash = _clip_parnl(ClipMachineMemory, 1);

   char buf[256];

   _clip_hash_name(ClipMachineMemory, hash, buf, sizeof(buf));
   _clip_retc(ClipMachineMemory, buf);

   return 0;
}
