int
clip_HS_VERIFY(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_UNDELETE";

   ClipVar *val;

   int subl, strl;

   const char *str = _clip_parcl(ClipMachineMemory, 2, &subl);

   int lcase = _clip_parl(ClipMachineMemory, 3);

   ClipVar v, *vp;

   char *sub = NULL, *string = NULL, *r;

   int er;

   CHECKARG3(1, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG1(2, CHARACTER_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == CHARACTER_type_of_ClipVarType)
   {
      val = _clip_par(ClipMachineMemory, 1);
      strl = val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      string = malloc(strl + 1);
      memcpy(string, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, strl);
   }
   else
   {
      val = _clip_spar(ClipMachineMemory, 1);
      if ((er = rdd_calc(ClipMachineMemory, -1, val, &v, 0)))
	 goto err;
      vp = _clip_vptr(&v);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	 goto err;
      }
      strl = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      string = malloc(strl + 1);
      memcpy(string, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, strl);
      _clip_destroy(ClipMachineMemory, &v);
   }
   sub = malloc(subl + 1);
   memcpy(sub, str, subl);
   if (lcase)
   {
      _clip_upstr(string, strl);
      _clip_upstr(sub, subl);
   }
   r = (char *) _clip_strstr(string, strl, sub, subl);

   free(sub);
   free(string);

   _clip_retl(ClipMachineMemory, r != NULL);
   return 0;
 err:
   if (sub)
      free(sub);
   if (string)
      free(string);
   return er;
}
