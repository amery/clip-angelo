int
clip_CTOBIT(ClipMachine * ClipMachineMemory)
{
   int LSel, LMask, i, j;

   char *CSel = _clip_parcl(ClipMachineMemory, 1, &LSel);

   char *CMask = _clip_parcl(ClipMachineMemory, 2, &LMask);

   unsigned int Res = 0;

   if (_clip_parinfo(ClipMachineMemory, 0) != 2 || CSel == NULL || CMask == NULL)
   {
      _clip_retni(ClipMachineMemory, 0);
      return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "CTOBIT");
   }

   if (LSel > 16)
      LSel = 16;
   if (LMask > 16)
      LMask = 16;
   for (i = 0; i < LSel; i++)
   {
      for (j = 0; j < LMask && CSel[i] != CMask[j]; j++);
      if (j != LMask)
	 Res |= 1 << (LMask - j - 1);
   }
   _clip_retni(ClipMachineMemory, Res);
   return 0;
}
