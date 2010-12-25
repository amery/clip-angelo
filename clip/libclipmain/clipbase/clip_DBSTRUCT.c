int
clip_DBSTRUCT(ClipMachine * ClipMachineMemory)
{
   const char *__PROC__ = "DBSTRUCT";

   DBWorkArea *wa;

   int nfld, i;

   ClipVar *rp = RETPTR(ClipMachineMemory);

   long vect[2];

   int area = _clip_parni(ClipMachineMemory, 1);

   int er;

   CHECKOPT1(1, NUMERIC_type_of_ClipVarType);

   if (_clip_parinfo(ClipMachineMemory, 1) == UNDEF_type_of_ClipVarType)
      wa = cur_area(ClipMachineMemory);
   else
      wa = get_area(ClipMachineMemory, area, 0, 0);

   if (!wa)
   {
      vect[0] = 0;
      return _clip_array(ClipMachineMemory, rp, 1, vect);
   }

   nfld = wa->rd->nfields;
   vect[0] = nfld;
   vect[1] = 4 + ((wa->rd->sig == 0x03) ? 0 : 2);
   _clip_array(ClipMachineMemory, rp, 2, vect);

   for (i = 0; i < nfld; i++)
   {
      ClipVar var;

      RDD_FIELD *fp = &wa->rd->fields[i];

      vect[0] = i;
      memset(&var, 0, sizeof(ClipVar));

      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->name;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->name);
      vect[1] = 0;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(2);
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = fp->type;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[1] = 0;
      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 1;
      vect[1] = 1;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);
      free(var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

      var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
      var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->len;
      var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
      vect[1] = 2;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->dec;
      vect[1] = 3;
      _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

      if (wa->rd->sig == 0x30)
      {
	 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = LOGICAL_type_of_ClipVarType;
	 var.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = fp->binary;
	 vect[1] = 4;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);

	 var.ClipLogVar_l_of_ClipVar.value_of_ClipLogVar = fp->nullable;
	 vect[1] = 5;
	 _clip_aset(ClipMachineMemory, rp, &var, 2, vect);
      }
   }
   return 0;
 err:
   return er;
}
