int
clip_LTRIM(ClipMachine * ClipMachineMemory)
{
   int vl = 0, rl = 0;

   char *rp, *e, *end;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "LTRIM");
   }
   for (e = vp, end = vp + vl; e < end && (*e == ' ' || *e == '\t'); e++);
   rl = vl - (e - vp);
   rp = (char *) malloc(rl + 1);
   memcpy(rp, e, rl);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}
