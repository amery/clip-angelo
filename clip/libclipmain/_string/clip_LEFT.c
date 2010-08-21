int
clip_LEFT(ClipMachine * ClipMachineMemory)
{
   int vl = 0;

   int i;

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
   if (nl > vl)
      nl = vl;
   for (i = 0; i < nl; i++)
      rp[i] = vp[i];
   rp[i] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, nl);
   return 0;
}
