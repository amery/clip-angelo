int
clip_RDDLIST(ClipMachine * ClipMachineMemory)
{
   ClipVar  *rp = RETPTR(ClipMachineMemory);

   long      vect[1];

   int       i;


   vect[0] = *ClipMachineMemory->ndbdrivers;
   _clip_array(ClipMachineMemory, rp, 1, vect);

   for (i = 0; i < *ClipMachineMemory->ndbdrivers; i++)
    {
       ClipVar   var;

       var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
       var.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
       var.ClipType_t_of_ClipVar.memo_of_ClipType = 0;
       var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf =
	strlen((*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver);
       var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = (*ClipMachineMemory->dbdrivers)[i].id_of_DBDriver;
       vect[0] = i;
       _clip_aset(ClipMachineMemory, rp, &var, 1, vect);
    }
   return 0;
}
