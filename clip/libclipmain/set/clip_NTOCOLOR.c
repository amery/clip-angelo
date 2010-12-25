int
clip_NTOCOLOR(ClipMachine * ClipMachineMemory)
{
   int attr = _clip_parni(ClipMachineMemory, 1);

   int num_format = _clip_parl(ClipMachineMemory, 2);

   char *buf;

   if (attr < 0 || attr > 0xff)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   buf = malloc(32);
   memset(buf, 0, 32);
   _clip_attr2str(attr, buf, 31, !num_format);
   _clip_retc(ClipMachineMemory, buf);
   free(buf);
   return 0;
}
