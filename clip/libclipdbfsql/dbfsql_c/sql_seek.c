int
sql_seek(ClipMachine * ClipMachineMemory, SQLROWSET * rowset, ClipVar * val, int soft, int *found)
{
   void *k;

   *found = 0;
   if (!rowset->curord)
      return 0;

   if (!rowset->curord->bt)
      return 0;

   k = calloc(1, rowset->curord->len);
   switch (val->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      if (val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf > rowset->curord->len)
      {
	 free(k);
	 return 0;
      }
      *(int *) k = -val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      memcpy((char *) k + sizeof(int), val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf, val->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf);
      break;
   case NUMERIC_type_of_ClipVarType:
      *(double *) ((char *) k + sizeof(int)) = val->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar;
      break;
   case DATE_type_of_ClipVarType:
      *(long *) ((char *) k + sizeof(int)) = val->ClipDateVar_d_of_ClipVar.julian_of_ClipDateVar;
      break;
   case DATETIME_type_of_ClipVarType:
      *(long *) ((char *) k + sizeof(int)) = val->ClipDateTimeVar_dt_of_ClipVar.julian_of_ClipDateTimeVar;
      *(long *) ((char *) k + sizeof(int) + sizeof(long)) = val->ClipDateTimeVar_dt_of_ClipVar.time_of_ClipDateTimeVar;
      break;
   default:
      break;
   }
   *found = bt_seek(rowset->curord->bt, rowset->curord, k);
   free(k);
   if (*found || soft)
      rowset->recno = *(int *) bt_key(rowset->curord->bt);
   return 0;
}
