int
rdd_calcfilter(ClipMachine * ClipMachineMemory, RDD_DATA * rd, int *fok, const char *__PROC__)
{
   int i, er;

   *fok = 1;
   if (!rd->filter)
      return 0;
   for (i = 0; i < rd->filter->nfps; i++)
   {
      ClipVar v, *vp;

      if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->filter->fps[i].bfilter, &v, rd->x)))
	 return er;
      vp = _clip_vptr(&v);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
      {
	 _clip_destroy(ClipMachineMemory, &v);
	 return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badfilterexpr);
      }
      switch (rd->filter->fps[i].op)
      {
      case 0:
	 *fok = vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	 break;
      case 1:
	 *fok = *fok || vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	 break;
      case 2:
	 *fok = *fok && vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
	 break;
      case 3:
	 *fok = (*fok && !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar) || (!(*fok) && vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar);
	 break;
      case 4:
	 *fok = !vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      }
      _clip_destroy(ClipMachineMemory, &v);
   }
   return 0;
}
