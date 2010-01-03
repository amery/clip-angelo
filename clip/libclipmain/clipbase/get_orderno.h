int
get_orderno(DBWorkArea * wa, ClipVar * order, ClipVar * index)
{
   int       ord = -1;

   int       indno = 0;

   if ((!order || (order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType))
       && (!index || (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)))
      return wa->rd->curord;
   if (index
       && (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType
	   || index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType))
    {
       if (index->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	{
	   indno = index->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	}
       else
	{
	   char     *i = strdup(index->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

	   char     *e = i + strlen(i);

	   int       l;

	   while (*(--e) == ' ');
	   *(e + 1) = 0;
	   l = e - i + 1;
	   for (indno = 0; indno < wa->rd->idxs_opened; indno++)
	    {
	       if (strcasecmp(wa->rd->indices[indno]->name, i) == 0)
		  break;
	    }
	}
       if (indno < wa->rd->idxs_opened && indno >= 0)
	{
	   RDD_INDEX *ri = wa->rd->indices[indno];

	   if (order && order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	    {
	       if (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 1
		   && order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar <= ri->norders)
		  ord = ri->orders[(int) order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1]->orderno;
	    }
	   else if (order && order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	    {
	       int       ordno;

	       char     *o = strdup(order->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

	       char     *e = o + strlen(o);

	       int       l;

	       while (*(--e) == ' ');
	       *(e + 1) = 0;
	       l = e - o + 1;
	       for (ordno = 0; ordno < ri->norders; ordno++)
		{
		   if (strcasecmp(ri->orders[ordno]->name, o) == 0)
		    {
		       ord = ri->orders[ordno]->orderno;
		       break;
		    }
		}
	    }
	   else
	    {
	       ord = ri->orders[0]->orderno;
	    }
	}
    }
   else if (order)
    {
       if (order->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	{
	   if (order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar >= 1
	       && order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar <= wa->rd->ords_opened)
	      ord = order->ClipNumVar_n_of_ClipVar.double_of_ClipNumVar - 1;
	}
       else
	{
	   int       ordno;

	   char     *o = strdup(order->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf);

	   char     *e = o + strlen(o);

	   int       l;

	   while (*(--e) == ' ');
	   *(e + 1) = 0;
	   l = e - o + 1;
	   for (ordno = 0; ordno < wa->rd->ords_opened; ordno++)
	    {
	       if (strcasecmp(wa->rd->orders[ordno]->name, o) == 0)
		{
		   ord = ordno;
		   break;
		}
	    }
	}
    }
   return ord;
}
