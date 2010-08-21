int
rdd_event(ClipMachine * ClipMachineMemory, int nEvent, int nArea, int nFieldPos, ClipVar * xTrigVal, int *re, const char *__PROC__)
{
   ClipFunction *fp;

   ClipBlock *bp;

   ClipVar tres;

   ClipVar args[4];

   DBWorkArea *wa;

   int er;

   if (re)
      *re = 1;
   if (nArea < 0)
      return 0;

   wa = ClipMachineMemory->areas->items_of_ClipVect[nArea];
   if (!wa->trig_active)
      return 0;

   if (_clip_get_function(ClipMachineMemory, wa->trighash, &fp, &bp))
   {
      memset(args, 0, sizeof(args));
      args[0].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      args[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      args[0].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      args[0].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nEvent;
      args[1].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      args[1].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      args[1].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      args[1].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nArea + 1;
      args[2].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      args[2].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
      args[2].ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      args[2].ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = nFieldPos;
      if (xTrigVal)
      {
	 args[3].ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = xTrigVal->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;
	 args[3].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MREF_ClipFlags;
	 args[3].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = xTrigVal;
      }
      if ((er = _clip_clip(ClipMachineMemory, wa->trigger, 4, args, &tres)))
	 return er;
      if (re)
	 *re = tres.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      _clip_destroy(ClipMachineMemory, &tres);
   }
   else
   {
      wa->trig_active = 0;
   }
   return 0;
}
