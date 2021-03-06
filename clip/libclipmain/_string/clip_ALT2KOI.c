int
clip_ALT2KOI(ClipMachine * ClipMachineMemory)
{

   int l;

   unsigned char *ret;

   unsigned char *str = (unsigned char *) _clip_parcl(ClipMachineMemory, 1, &l);

   if (str == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }
   ret = malloc(l + 1);
   memcpy(ret, str, l);
   ret[l] = 0;
   _clip_recodeString((char *) ret, l, CS_ALT, CS_KOI);
   _clip_retcn_m(ClipMachineMemory, (char *) ret, l);
   return 0;
}
