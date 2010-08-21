int
rdd_calc(ClipMachine * ClipMachineMemory, int area, ClipVar * block, ClipVar * var, int noerror)
{
   int oldarea = 0;

   int er;

   if (area != -1)
   {
      oldarea = ClipMachineMemory->curArea;
      ClipMachineMemory->curArea = area;
   }
   if (noerror)
   {
      ClipMachineMemory->noerrblock++;
      if ((er = _clip_eval(ClipMachineMemory, block, 0, 0, var)))
      {
	 var->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 var->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
      }
      ClipMachineMemory->noerrblock--;
   }
   else
   {
      if ((er = _clip_eval(ClipMachineMemory, block, 0, 0, var)))
	 return er;
   }
   if (area != -1)
      ClipMachineMemory->curArea = oldarea;
   return 0;
}
