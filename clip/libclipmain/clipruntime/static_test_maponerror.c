static int
test_maponerror(ClipMachine * ClipMachineMemory, ClipVar * map, char *member)
{
   int ind, ret = 1;

   /* try run `onerror` member */
   if (search_map(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar, map->ClipMapVar_m_of_ClipVar.count_of_ClipMapVar, HASH_onerror, &ind))
   {
      ClipVar *ep;

      map->ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      ep = _clip_vptr(&(map->ClipMapVar_m_of_ClipVar.ClipVarEl_items_of_ClipMapVar[ind].ClipVar_var_of_ClipVarEl));

      if (ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == PCODE_type_of_ClipVarType || ep->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CCODE_type_of_ClipVarType)
      {
	 int i;

	 ClipVar stack[3];

	 ClipVar *rval = ClipMachineMemory->fp->ClipVar_sp_of_ClipFrame - 1;

	 memset(stack, 0, sizeof(stack));

	 stack[0].ClipType_t_of_ClipVar = map->ClipType_t_of_ClipVar;
	 stack[0].ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
	 stack[0].ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = map;
	 map->ClipType_t_of_ClipVar.count_of_ClipType++;

	 _clip_var_str(member, strlen(member), stack + 1);
	 _clip_clone(ClipMachineMemory, stack + 2, rval);

	 _clip_eval(ClipMachineMemory, _clip_vptr(ep), 3, stack, rval);

	 for (i = 0; i < 3; i++)
	    _clip_destroy(ClipMachineMemory, stack + i);
	 ret = 0;
      }
      map->ClipType_t_of_ClipVar.memo_of_ClipType = 1;
   }
   return ret;
}
