int
clip_ALLTRIM(ClipMachine * ClipMachineMemory)
{
   int vl = 0, rl = 0;

   char *rp, *e;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return 0;
      //return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "ALLTRIM");
   }
   rp = (char *) malloc(vl + 1);
   for (e = vp; e < vp + vl && (*e == ' ' || *e == '\t'); e++);
   rl = vl - (e - vp);
   memcpy(rp, e, rl);
   for (e = rp + rl - 1; e >= rp && (*e == ' ' || *e == '\t'); e--, rl--);
   rp = (char *) realloc(rp, rl + 1);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}
