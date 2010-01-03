int
clip_AFIELDS(ClipMachine * ClipMachineMemory)
{
   DBWorkArea *wa = cur_area(ClipMachineMemory);

   int       nfld, i;

   long      vect[1];

   ClipVar  *name_ap = _clip_par(ClipMachineMemory, 1);

   ClipVar  *type_ap = _clip_par(ClipMachineMemory, 2);

   ClipVar  *len_ap = _clip_par(ClipMachineMemory, 3);

   ClipVar  *dec_ap = _clip_par(ClipMachineMemory, 4);

   int       r;


   if (!wa)
    {
       _clip_retni(ClipMachineMemory, 0);
       return 0;
    }

   nfld = wa->rd->nfields;

   vect[0] = nfld;
   if (name_ap && name_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      name_ap = NULL;
   if (type_ap && type_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      type_ap = NULL;
   if (len_ap && len_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      len_ap = NULL;
   if (dec_ap && dec_ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      dec_ap = NULL;

   for (i = 0; i < nfld; i++)
    {
       ClipVar   var;

       RDD_FIELD *fp = &wa->rd->fields[i];

       vect[0] = i;
       memset(&var, 0, sizeof(ClipVar));

       if (name_ap && i < name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	{
	   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = fp->name;
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(fp->name);
	   _clip_aset(ClipMachineMemory, name_ap, &var, 1, vect);
	}

       if (type_ap && i < type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	{
	   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = malloc(2);
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[0] = fp->type;
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf[1] = 0;
	   var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = 1;
	   _clip_aset(ClipMachineMemory, type_ap, &var, 1, vect);
	}

       if (len_ap && i < len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	{
	   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
	   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->len;
	   var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   _clip_aset(ClipMachineMemory, len_ap, &var, 1, vect);
	}

       if (dec_ap && i < dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)
	{
	   var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	   var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = fp->dec;
	   var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
	   _clip_aset(ClipMachineMemory, dec_ap, &var, 1, vect);
	}
    }
   r = 0x7fffffff;
   if (name_ap)
      if (name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
	 r = name_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (type_ap)
      if (type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
	 r = type_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (len_ap)
      if (len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
	 r = len_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   if (dec_ap)
      if (dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar < r)
	 r = dec_ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
   _clip_retni(ClipMachineMemory, r);
   return 0;
}
