int
clip_SETCLEARB(ClipMachine * ClipMachineMemory)
{
   int ch = _clip_parni(ClipMachineMemory, 1);

   char *s = _clip_parc(ClipMachineMemory, 1);

   int *set = _clip_fetch_item(ClipMachineMemory, HASH_setclearb);

   if (s != NULL)
      ch = (*s);
   ch = (ch % 0xff);
   *((char *) set) = (char) ch;
   _clip_retc(ClipMachineMemory, "");

   return 0;
}
