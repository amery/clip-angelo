int
clip_SQLROWID(ClipMachine * ClipMachineMemory)
{
   SQLROWSET *rowset = (SQLROWSET *) _clip_fetch_c_item(ClipMachineMemory, _clip_parni(ClipMachineMemory, 1),
							_C_ITEM_TYPE_SQL);

   if (!rowset)
   {
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOROWSET, er_norowset);
      return 1;
   }

   if (!rowset->ids)
   {
      _clip_retni(ClipMachineMemory, rowset->id + 1);
   }
   else if (rowset->nids == 1)
   {
      _clip_retni(ClipMachineMemory, rowset->ids[0] + 1);
   }
   else
   {
      ClipVar *r = RETPTR(ClipMachineMemory);

      ClipVar var;

      long vect[1];

      int i;

      vect[0] = rowset->nids;
      _clip_array(ClipMachineMemory, r, 1, vect);

      for (i = 0; i < rowset->nids; i++)
      {
	 vect[0] = i;
	 memset(&var, 0, sizeof(ClipVar));

	 var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = NUMERIC_type_of_ClipVarType;
	 var.ClipType_t_of_ClipVar.len_of_ClipType = 10;
	 var.ClipType_t_of_ClipVar.dec_of_ClipType = 2;
	 var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar = rowset->ids[i] + 1;
	 _clip_aset(ClipMachineMemory, r, &var, 1, vect);
      }
   }
   return 0;
}
