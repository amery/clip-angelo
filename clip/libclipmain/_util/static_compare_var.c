static int
compare_var(ClipVar * p1, ClipVar * p2, ClipMachine * ClipMachineMemory, void *par)
{
   int ret = 0;

   if (!par)
   {
      _clip_cmp(ClipMachineMemory, p1, p2, &ret, 1);
   }
   else
   {
      ClipVar res, stack[2];

      memset(&res, 0, sizeof(ClipVar));
      stack[0] = *p1;
      stack[1] = *p2;
      _clip_eval(ClipMachineMemory, (ClipVar *) par, 2, stack, &res);
      switch (res.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case LOGICAL_type_of_ClipVarType:
	 ret = !res.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	 break;
      case NUMERIC_type_of_ClipVarType:
	 ret = _clip_double(&res);
	 break;
      default:
	 ;
      }
      _clip_destroy(ClipMachineMemory, &res);
   }
   return ret;
}
