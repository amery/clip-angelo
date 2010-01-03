int
clip_SQLLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar  *r = RETPTR(ClipMachineMemory);
   long      dims[1] = { 0 };
   int       i;

   _clip_array(ClipMachineMemory, r, 1, dims);
   for (i = 0; i < *ClipMachineMemory->nsqldrivers; i++)
    {
       ClipVar   a;

       ClipVar   e;

       memset(&a, 0, sizeof(ClipVar));
       memset(&e, 0, sizeof(ClipVar));
       _clip_array(ClipMachineMemory, &a, 1, dims);
		 e.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf =
	strlen((*ClipMachineMemory->sqldrivers)[i].id_of_SQLDriver);
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (*ClipMachineMemory->sqldrivers)[i].id_of_SQLDriver;
       _clip_aadd(ClipMachineMemory, &a, &e);
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf =
	strlen((*ClipMachineMemory->sqldrivers)[i].name_of_SQLDriver);
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	(*ClipMachineMemory->sqldrivers)[i].name_of_SQLDriver;
       _clip_aadd(ClipMachineMemory, &a, &e);
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf =
	strlen((*ClipMachineMemory->sqldrivers)[i].desc_of_SQLDriver);
       e.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf =
	(*ClipMachineMemory->sqldrivers)[i].desc_of_SQLDriver;
       _clip_aadd(ClipMachineMemory, &a, &e);
       _clip_aadd(ClipMachineMemory, r, &a);
    }
   return 0;
}
