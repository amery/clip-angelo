int
clip_HS_REPLACE(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_REPLACE";

   int h = _clip_parni(ClipMachineMemory, 1);

   ClipVar *val = _clip_spar(ClipMachineMemory, 2);

   unsigned int rno = _clip_parni(ClipMachineMemory, 3);

   int dirty = _clip_parni(ClipMachineMemory, 4);

   HIPER *hs;

   ClipVar v, *vp;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG3(2, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType, CHARACTER_type_of_ClipVarType);
   CHECKARG1(3, NUMERIC_type_of_ClipVarType);
   CHECKOPT1(4, LOGICAL_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

   if (!dirty)
   {
      if ((er = hs_remove(ClipMachineMemory, hs, rno, __PROC__)))
	 goto err;
   }
   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
   {
      if ((er =
	   _hs_add(ClipMachineMemory, hs, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rno, __PROC__)))
	 goto err;
   }
   else
   {
      if ((er = rdd_calc(ClipMachineMemory, -1, val, &v, 0)))
	 goto err;
      vp = _clip_vptr(&v);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	 goto err;
      }
      if ((er =
	   _hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, rno, __PROC__)))
	 goto err;
      _clip_destroy(ClipMachineMemory, &v);
   }

   _clip_retni(ClipMachineMemory, 1);
   return 0;
 err:
   return er;
}
