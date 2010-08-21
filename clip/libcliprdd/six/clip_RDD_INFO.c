int
clip_RDD_INFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "RDD_INFO";

   int type = _clip_parinfo(ClipMachineMemory, 1);

   int no, i, er;

   char *rdd;

   char err[100];

   ClipVar *ap = RETPTR(ClipMachineMemory);
   long dim[] = { 0 };
   ClipVar v;

   ClipMachineMemory->m6_error = 0;
   memset(&v, 0, sizeof(ClipVar));
   if (type == CHARACTER_type_of_ClipVarType)
   {
      rdd = _clip_parc(ClipMachineMemory, 1);
      for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
	 if (strncasecmp(rdd, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	    break;
      if (no >= *ClipMachineMemory->ndbdrivers)
	 return 0;
   }
   else if (type == NUMERIC_type_of_ClipVarType)
   {
      DBWorkArea *wa;

      no = _clip_parni(ClipMachineMemory, 1) - 1;
      if (no < 0 || no >= *ClipMachineMemory->ndbdrivers)
      {
	 sprintf(err, bad_arg, 1);
	 er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, err);
	 goto err;
      }
      wa = (DBWorkArea *) ClipMachineMemory->areas->items_of_ClipVect[no];
      for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
	 if (strncasecmp(wa->driver, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	    break;
   }
   else
   {
      for (no = 0; no < *ClipMachineMemory->ndbdrivers; no++)
	 if (strncasecmp(ClipMachineMemory->def_db_driver, (*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver, 6) == 0)
	    break;
   }

   _clip_array(ClipMachineMemory, ap, 1, dim);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->dbdrivers)[no].id_of_DBDriver);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
   v.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 1;
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].data_of_DBDriver, (*ClipMachineMemory->data_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->data_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].idx_of_DBDriver, (*ClipMachineMemory->idx_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->sing_suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].memo_of_DBDriver, (*ClipMachineMemory->memo_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->memo_drivers)[i]->suff);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->ndata_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].data_of_DBDriver, (*ClipMachineMemory->data_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->data_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nidx_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].idx_of_DBDriver, (*ClipMachineMemory->idx_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->idx_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   for (i = 0; i < *ClipMachineMemory->nmemo_drivers; i++)
      if (strncasecmp((*ClipMachineMemory->dbdrivers)[no].memo_of_DBDriver, (*ClipMachineMemory->memo_drivers)[i]->id, 3) == 0)
	 break;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = strdup((*ClipMachineMemory->memo_drivers)[i]->desc);
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aadd(ClipMachineMemory, ap, &v);
   _clip_destroy(ClipMachineMemory, &v);

   return 0;
 err:
   return er;
}
