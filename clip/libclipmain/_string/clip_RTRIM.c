int
clip_RTRIM(ClipMachine * ClipMachineMemory)
{
   int vl = 0, rl = 0;

   char *rp, *e;

   char *vp = _clip_parcl(ClipMachineMemory, 1, &vl);

   if (vp == NULL)
   {
      _clip_retc(ClipMachineMemory, "");
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "RTRIM");
   }
   for (e = vp + vl - 1; e >= vp && (*e == ' ' || *e == '\t'); e--);
   rl = e - vp + 1;
   rp = (char *) malloc(rl + 1);
   memcpy(rp, vp, rl);
   rp[rl] = 0;
   _clip_retcn_m(ClipMachineMemory, rp, rl);
   return 0;
}
