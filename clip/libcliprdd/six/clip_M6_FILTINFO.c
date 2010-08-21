int
clip_M6_FILTINFO(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "M6_FILTINFO";

   int h = _clip_parni(ClipMachineMemory, 1);

   RDD_FILTER *fp;

   long vect[1];

   ClipVar *rp = RETPTR(ClipMachineMemory);

   ClipVar var;

   unsigned int tmp, cnt;

   int er;

   ClipMachineMemory->m6_error = 0;
   CHECKARG1(1, NUMERIC_type_of_ClipVarType);
   fp = (RDD_FILTER *) _clip_fetch_c_item(ClipMachineMemory, h, _C_ITEM_TYPE_RYO);
   if (!fp)
   {
      er = rdd_err(ClipMachineMemory, EG_ARG, 0, __FILE__, __LINE__, __PROC__, er_badfilter);
      goto err;
   }

   vect[0] = 7;
   _clip_array(ClipMachineMemory, rp, 1, vect);

/*  INFO_EXPR (1) */
   vect[0] = INFO_EXPR - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->sfilter;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->sfilter);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_NONEXPR (2) */
   vect[0] = INFO_NONEXPR - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   if (fp->optimize == 2)
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = "";
   else
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->sfilter;
   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_OPTLVL (3) */
   vect[0] = INFO_OPTLVL - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->optimize;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_COUNT (4) */
   vect[0] = INFO_COUNT - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   if ((er = rm_evalpartial(ClipMachineMemory, fp, NULL, &tmp, &cnt, __PROC__)))
      goto err;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = cnt;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_SIZE (5) */
   vect[0] = INFO_SIZE - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->size;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_OWNER (6) */
   vect[0] = INFO_OWNER - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = (fp->active ? 1 : 2);
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

/*  INFO_POS (7) */
   vect[0] = INFO_POS - 1;
   memset(&var, 0, sizeof(ClipVar));
   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = -1;
   _clip_aset(ClipMachineMemory, rp, &var, 1, vect);

   return 0;
 err:
   return er;
}
