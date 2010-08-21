int
clip_FIELDBLANK(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "FIELDBLANK";

   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int fno = _clip_parni(ClipMachineMemory, 1) - 1;

   ClipVar *r = RETPTR(ClipMachineMemory);

   int er;

   _clip_retc(ClipMachineMemory, "");
   CHECKWA(wa);
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);

   if (fno < 0 || fno >= wa->rd->nfields)
      return rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_nofield);

   if (strchr("MPG", wa->rd->fields[fno].type))
      return 0;

   if ((!wa->rd->valid || wa->rd->pending_child_parent) && !(wa->rd->file.rlocked || wa->rd->file.wlocked))
   {

      READLOCK;
      if ((er = rdd_takevalue(ClipMachineMemory, wa->rd, fno, r, __PROC__)))
	 goto err_unlock;
      UNLOCK;
   }
   else
   {
      if ((er = rdd_takevalue(ClipMachineMemory, wa->rd, fno, r, __PROC__)))
	 goto err;
   }
   if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
   {
      memset(r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, ' ', r->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
   }
   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
   {
      r->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = 0;
   }
   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
   {
      r->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar = 0;
   }
   else if (r->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
   {
      r->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = 0;
   }
   return 0;
 err_unlock:
   wa->rd->vtbl->_ulock(ClipMachineMemory, wa->rd, __PROC__);
 err:
   return er;
}
