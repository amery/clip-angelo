int
rdd_create(ClipMachine * ClipMachineMemory, const char *driver, const char *memo_driver, const char *name, ClipVar * stru, const char *__PROC__)
{
   int count = stru->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;

   RDD_STRUCT *rddstru = NULL;

   int i, j, er = EG_UNSUPPORTED;

   RDD_DATA_VTBL *vtbl;

   char *path = NULL;

   if (count < 1)
   {
      er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badstructure);
      goto err;
   }
   rddstru = calloc(count, sizeof(RDD_STRUCT));
   for (i = 0; i < count; i++)
   {
      ClipVar *vp;

      ClipVar *name;

      ClipVar *type;

      ClipVar *len;

      ClipVar *dec;

      ClipVar *binary;

      ClipVar *nullable;

      vp = _clip_vptr(stru->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);
      if ((vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType) || (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < 4))
      {
	 er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	 goto err;
      }

      name = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 0);
      if (name->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	 goto err;
      }
      strncpy(rddstru[i].name, name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, sizeof(rddstru[i].name));
      rddstru[i].name[sizeof(rddstru[i].name) - 1] = 0;
      for (j = 0; j < name->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf; j++)
	 rddstru[i].name[j] = toupper(rddstru[i].name[j]);

      type = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 1);
      if (type->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	 goto err;
      }
      rddstru[i].type = toupper(type->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0]);

      if (rddstru[i].type == 'D')
      {
	 rddstru[i].len = 8;
	 rddstru[i].dec = 0;
      }
      else if (rddstru[i].type == 'L')
      {
	 rddstru[i].len = 1;
	 rddstru[i].dec = 0;
      }
      else if (rddstru[i].type == 'M')
      {
	 rddstru[i].len = 10;
	 rddstru[i].dec = 0;
      }
      else
      {
	 len = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 2);
	 if (len->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	 {
	    er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	    goto err;
	 }
	 rddstru[i].len = (int) (len->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar + 0.5);

	 dec = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 3);
	 if (dec->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != NUMERIC_type_of_ClipVarType)
	 {
	    er = rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_badfield);
	    goto err;
	 }
	 rddstru[i].dec = (int) (dec->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar + 0.5);
      }
      if (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar >= 5)
      {
	 binary = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 4);
	 if (binary->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	    rddstru[i].binary = binary->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      }
      if (vp->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar >= 6)
      {
	 nullable = _clip_vptr(vp->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + 5);
	 if (nullable->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	    rddstru[i].nullable = nullable->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar;
      }
   }
   if (!(vtbl = rdd_datadriver(ClipMachineMemory, driver, __PROC__)))
      goto err;

   if ((er = _rdd_parsepath(ClipMachineMemory, name, vtbl->suff, &path, NULL, EG_CREATE, __PROC__)))
      goto err;

   if ((er = vtbl->create(ClipMachineMemory, vtbl, path, rddstru, count, __PROC__)))
      goto err;
   free(path);
   free(rddstru);
   return 0;
 err:
   if (path)
      free(path);
   if (rddstru)
      free(rddstru);
   return er;
}
