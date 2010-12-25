int
clip_MAX(ClipMachine * ClipMachineMemory)
{
   int t1 = _clip_parinfo(ClipMachineMemory, 1);

   int t2 = _clip_parinfo(ClipMachineMemory, 1);

   if (t1 == NUMERIC_type_of_ClipVarType && t2 == NUMERIC_type_of_ClipVarType)
   {
      double n1, n2;

      int l1, l2, d1, d2;

      n1 = _clip_parnd(ClipMachineMemory, 1);
      _clip_parp(ClipMachineMemory, 1, &l1, &d1);
      n2 = _clip_parnd(ClipMachineMemory, 2);
      _clip_parp(ClipMachineMemory, 2, &l2, &d2);
      if (n1 > n2)
	 _clip_retndp(ClipMachineMemory, n1, l1, d1);
      else
	 _clip_retndp(ClipMachineMemory, n2, l2, d2);
      return 0;
   }
   if (t1 == DATE_type_of_ClipVarType && t2 == DATE_type_of_ClipVarType)
   {
      long ret, n1, n2;

      n1 = _clip_pardj(ClipMachineMemory, 1);
      n2 = _clip_pardj(ClipMachineMemory, 2);
      if (n1 > n2)
	 ret = n1;
      else
	 ret = n2;
      _clip_retdj(ClipMachineMemory, ret);
      return 0;
   }
   _clip_retni(ClipMachineMemory, 0);
   return _clip_trap_err(ClipMachineMemory, EG_ARG, 0, 0, __FILE__, __LINE__, "MAX");
}
