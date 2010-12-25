int
clip_HS_ADD(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "HS_ADD";

   int h = _clip_parni(ClipMachineMemory, 1);

   HIPER *hs;

   unsigned int rno;

   int er;

   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   CHECKARG3(2, CHARACTER_type_of_ClipVarType, CCODE_type_of_ClipVarType, PCODE_type_of_ClipVarType);

   hs = _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_HIPER);
   if (!hs)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badhiper);
      goto err;
   }

   if (_clip_parinfo(ClipMachineMemory, 2) == CHARACTER_type_of_ClipVarType)
   {
      int len;

      const char *str = _clip_parcl(ClipMachineMemory, 2, &len);

      if ((er = hs_add(ClipMachineMemory, hs, str, len, &rno, __PROC__)))
	 goto err;
   }
   else
   {
      ClipVar *block = _clip_par(ClipMachineMemory, 2);

      ClipVar v, *vp;

      if ((er = rdd_calc(ClipMachineMemory, -1, block, &v, 0)))
	 goto err;
      vp = _clip_vptr(&v);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != CHARACTER_type_of_ClipVarType)
      {
	 er = rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
	 goto err;
      }
      if ((er =
	   hs_add(ClipMachineMemory, hs, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, &rno, __PROC__)))
	 goto err;
      _clip_destroy(ClipMachineMemory, &v);
   }
   if (rno % 32 == 1)
   {
      int ints = (rno + 31) >> 5;

      hs->map = realloc(hs->map, ints << 2);
      hs->map[ints - 1] = 0;
      if (hs->rmap)
      {
	 hs->rmap = realloc(hs->rmap, ints << 2);
	 hs->rmap[ints - 1] = 0;
      }
   }
   _clip_retni(ClipMachineMemory, rno);
   return 0;
 err:
   return er;
}
