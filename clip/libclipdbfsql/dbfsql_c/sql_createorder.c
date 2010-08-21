int
sql_createorder(ClipMachine * ClipMachineMemory, ClipVar * rmap, SQLROWSET * rowset, char *tagname, ClipVar * expr, int len)
{
   int t4 = expr->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType;

   char m[1024];

   SQLORDER *order = calloc(1, sizeof(SQLORDER));

   if (!HashTable_insert(rowset->orders, order, _clip_casehashstr(tagname)))
   {
      free(order);
      _clip_trap_err(ClipMachineMemory, 0, 0, 0, subsys, ER_ORDEREXIST, er_orderexist);
      return 1;
   }

   order->len = len + sizeof(int);
   order->block = calloc(1, sizeof(ClipVar));
   switch (t4)
   {
   case CHARACTER_type_of_ClipVarType:
      snprintf(m, sizeof(m), "{|rs| rs:getValue(\"%s\")}", expr->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);
      break;
   case NUMERIC_type_of_ClipVarType:
      snprintf(m, sizeof(m), "{|rs| rs:getValue(%d)}", (int) expr->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar);
      break;
   case CCODE_type_of_ClipVarType:
   case PCODE_type_of_ClipVarType:
      _clip_dup(ClipMachineMemory, order->block, expr);
      break;
   }
   if (t4 == CHARACTER_type_of_ClipVarType || t4 == NUMERIC_type_of_ClipVarType)
   {
      if (_clip_eval_macro(ClipMachineMemory, m, strlen(m), order->block))
      {
	 free(order->block);
	 free(order);
	 return 1;
      }
   }
   order->loc = rowset->conn->loc;
   order->rmap = calloc(1, sizeof(ClipVar));
   memcpy(order->rmap, rmap, sizeof(ClipVar));
   order->name = strdup(tagname);
   return 0;
}
