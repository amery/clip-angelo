static int
sql_searchtree(ClipMachine * ClipMachineMemory, SQLROWSET * rowset, SQLORDER * order)
{
   ClipVar   var;

   void     *k = calloc(1, order->len);

   int       found;

   *(int *) k = rowset->recno;
   if (_clip_eval(ClipMachineMemory, order->block, 1, order->rmap, &var))
      return 1;
   switch (var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
		case CHARACTER_type_of_ClipVarType:
       if (var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf > order->len)
	{
	   free(k);
	   return 0;
	}
       memcpy((char *) k + sizeof(int), var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	      var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
       break;
		case NUMERIC_type_of_ClipVarType:
       *(double *) ((char *) k + sizeof(int)) = var.ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
       break;
		case DATE_type_of_ClipVarType:
       *(long *) ((char *) k + sizeof(int)) = var.ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
       break;
		case DATETIME_type_of_ClipVarType:
       *(long *) ((char *) k + sizeof(int)) = var.ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar;
       *(long *) ((char *) k + sizeof(int) + sizeof(long)) = var.ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
       break;
    default:
       break;
    }
   _clip_destroy(ClipMachineMemory, &var);
   found = bt_seek(order->bt, order, k);
   free(k);
   if (!found)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_INTERNAL, er_internal);
       return 1;
    }
   return 0;
}
