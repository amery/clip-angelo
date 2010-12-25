int
clip_LOWER(ClipMachine * ClipMachineMemory)
{
   int l;

   char *s;

   char *r;

   s = _clip_parcl(ClipMachineMemory, 1, &l);
   if (!s)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
   }

   r = _clip_memdup(s, l);
   translate((unsigned char *) r, l, 0);

   _clip_retcn_m(ClipMachineMemory, r, l);

   return 0;
}
