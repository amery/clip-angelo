int
clip_RIGHT(ClipMachine * ClipMachineMemory)
{
   int vl = 0;

   int i, j;

   char *rp;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   int nl = _clip_parni(ClipMachineMemory, 2);

   _clip_retc(ClipMachineMemory, "");
   if (vp == NULL)
      return 0;
   if (nl < 0)
      nl = 0;
   if (nl > vl)
      nl = vl;
   rp = malloc(nl + 1);
   for (i = vl - nl, j = 0; i < vl; i++, j++)
      rp[j] = vp[i];
   rp[j] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, nl);
   return 0;
}
