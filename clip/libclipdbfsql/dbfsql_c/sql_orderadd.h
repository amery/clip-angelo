int
sql_orderadd(ClipMachine * ClipMachineMemory, SQLROWSET * rowset, long taghash)
{
   SQLORDER *order = (SQLORDER *) HashTable_fetch(rowset->orders, taghash);

   ClipVar   var;

   void     *k;

   if (!order)
    {
       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_NOORDER, er_noorder);
       return 1;
    }
   if (_clip_eval(ClipMachineMemory, order->block, 1, order->rmap, &var))
      return 1;
   if (!order->bt)
    {
       int       (*compare) (void *op, void *key1, void *key2, int *uniq);

       switch (var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
	{
			 case CHARACTER_type_of_ClipVarType:
				 if (var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType && !order->len)
	    {
	       _clip_destroy(ClipMachineMemory, &var);
	       _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_BADORDERLEN, er_badorderlen);
	       return 1;
	    }
	   compare = _sql_char_compare;
	   break;
			 case NUMERIC_type_of_ClipVarType:
	   order->len = sizeof(double) + sizeof(int);
	   compare = _sql_num_compare;
	   break;
			 case DATE_type_of_ClipVarType:
	   order->len = sizeof(long) + sizeof(int);
	   compare = _sql_date_compare;
	   break;
			 case DATETIME_type_of_ClipVarType:
	   order->len = sizeof(long) * 2 + sizeof(int);
	   compare = _sql_dt_compare;
	   break;
	default:
	   _clip_destroy(ClipMachineMemory, &var);
	   _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_BADORDERTYPE, er_badordertype);
	   return 1;
	}
       order->bt = bt_create(0, 100, order->len, compare);
    }
   k = calloc(1, order->len);
   *(int *) k = rowset->recno;
   switch (var.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
    {
		case CHARACTER_type_of_ClipVarType:
       memcpy((char *) k + sizeof(int), var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf,
	      min(var.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf, order->len));
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
   if (bt_add(order->bt, order, k))
    {
       free(k);
       _clip_destroy(ClipMachineMemory, &var);
       return 1;
    }
   free(k);
   _clip_destroy(ClipMachineMemory, &var);
   return 0;
}
