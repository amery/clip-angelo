static int
_cdx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, unsigned int header, const char *__PROC__)
{
   RDD_ORDER *ro = NULL;

   CDX_HEADER hdr;

   ClipVar vv, *vp;

   int lastrec, s, e, i, er;

   unsigned int newroot;

   BTREE1 *bt;

   void *buf;

   for (i = 0; i < ri->norders; i++)
      if (strcasecmp(tag, ri->orders[i]->name) == 0)
      {
	 destroy_rdd_order(ClipMachineMemory, ro);
	 ro = ri->orders[i];
	 memset(ro, 0, sizeof(RDD_ORDER));
	 break;
      }
   if (!ro)
   {
      ro = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));
      ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
      ri->orders[ri->norders] = ro;
      ri->norders++;
   }
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   *rop = ro;

   ro->header = header;
   ro->name = (char *) malloc(strlen(tag ? tag : ri->name) + 1);
   strcpy(ro->name, tag ? tag : ri->name);

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
   {
      ro->simpexpr = 1;
      if (ri->rd->fields[ro->simpfno].binary)
	 ro->binary = 1;
      ro->type = ri->rd->fields[ro->simpfno].type;
      ro->bufsize = ri->rd->fields[ro->simpfno].len - 1;
   }

   if (!block || block->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == UNDEF_type_of_ClipVarType)
   {
      int r = _clip_parni(ClipMachineMemory, 1);

      char expr[1024];

      memset(expr, 0, sizeof(expr));

      rdd_expandmacro(rd->area, r, ro->expr, expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->block)))
	 return er;
   }
   else
   {
      _clip_clone(ClipMachineMemory, &ro->block, block);
   }

   if (ro->type == 'X')
   {
      ro->keysize = ro->bufsize = ro->index->rd->fields[ro->simpfno].len - 1;
   }
   else
   {
      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	 return er;
      vp = _clip_vptr(&vv);

      switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
      {
      case CHARACTER_type_of_ClipVarType:
	 ro->bufsize = ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
	 ro->dec = 0;
	 ro->type = 'C';
	 break;
      case NUMERIC_type_of_ClipVarType:
	 ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vv.ClipType_t_of_ClipVar.len_of_ClipType : 10);
	 ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
	 ro->bufsize = 8;
	 ro->type = 'N';
	 break;
      case LOGICAL_type_of_ClipVarType:
	 ro->keysize = 1;
	 ro->dec = 0;
	 ro->bufsize = 1;
	 ro->type = 'L';
	 break;
      case DATE_type_of_ClipVarType:
	 ro->keysize = 8;
	 ro->dec = 0;
	 ro->bufsize = 8;
	 ro->type = 'D';
	 break;
      case DATETIME_type_of_ClipVarType:
	 ro->keysize = 8;
	 ro->dec = 0;
	 ro->bufsize = 8;
	 ro->type = 'T';
	 break;
      default:
#ifdef D20070119
	 _clip_destroy(ClipMachineMemory, &vv);
#endif
	 return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
      }
#ifdef D20070119
      _clip_destroy(ClipMachineMemory, &vv);
#endif
   }
   ro->key = malloc(ro->bufsize);
#ifndef D20070119
   _clip_destroy(ClipMachineMemory, &vv);
#endif

   ro->unique = unique;
   ro->descend = rd->os.lDescend;
   ro->custom = rd->os.lCustom;
   ro->canadd = 1;
   ro->curpage = calloc(1, sizeof(CDX_PAGE));
   ro->curoffs = 0;
   ro->ic = rd->os.lIgnoreCase;
   if (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ro->cforexpr = malloc(strlen(rd->os.cForCondition) + 1);
      strcpy(ro->cforexpr, rd->os.cForCondition);
      _clip_clone(ClipMachineMemory, &ro->bforexpr, &rd->os.bForCondition);
   }

   memset(&hdr, 0, sizeof(CDX_HEADER));
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      return er;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
      return er;
   _rdd_put_uint(hdr.root, newroot);
   _rdd_put_uint(hdr.keysize, ro->bufsize);
   hdr.options = (strcmp(ri->vtbl->id, "CDX") ? 0x20 : 0x62)
      | (unique ? 0x01 : 0x00) | (ro->custom ? 0x04 : 0x00) | (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType ? 0x08 : 0x00);
   hdr.sign = 1;
   hdr.ic = ro->ic;
   _rdd_put_ushort(hdr.sort, ro->descend);
   _rdd_put_ushort(hdr.explen, strlen(ro->expr) + 1);
   _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.explen));
   strcpy((char *) hdr.keyforexpr, ro->expr);
   if (!ro->binary)
      loc_write(ri->loc, hdr.keyforexpr, strlen((const char *) hdr.keyforexpr));
   if (rd->os.cForCondition)
   {
      _rdd_put_ushort(hdr.forlen, strlen(rd->os.cForCondition) + 1);
      _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.totlen) + _rdd_ushort(hdr.forlen));
      strcpy((char *) (hdr.keyforexpr + _rdd_ushort(hdr.explen)), rd->os.cForCondition);
      if (!ro->binary)
	 loc_write(ri->loc, hdr.keyforexpr + _rdd_ushort(hdr.explen), strlen((const char *) (hdr.keyforexpr + _rdd_ushort(hdr.explen))));
   }
   else
   {
      _rdd_put_ushort(hdr.forlen, 1);
   }

   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if (ri->file.md == (caddr_t) - 1)
   {
      struct stat st;

      fstat(ri->file.fd, &st);
      _rdd_put_uint(hdr.root, st.st_size);
   }
   else
   {
      _rdd_put_uint(hdr.root, ri->file.mapsize);
   }
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
      return er;

   if (ro->custom)
      return 0;

   if ((er = rd->vtbl->lastrec(ClipMachineMemory, rd, &lastrec, __PROC__)))
      return er;

   s = rd->os.nStart;
   e = min(lastrec, rd->os.nStart + rd->os.nNext - 1);
   if (rd->os.nStart == 0 || !rd->os.lRest)
      s = 1;
   if (rd->os.nNext == 0)
      e = lastrec;
   if (rd->os.nRecord)
      s = e = min(lastrec, rd->os.nRecord);
   if (rd->os.lAll || s < 1 || e < 1)
   {
      s = 1;
      e = lastrec;
   }
   i = max(1, rd->os.nInterval);

   if (rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
      rd->os.lCurrent = 1;
   if (rd->os.lAll)
   {
      if ((er = rdd_gotop(ClipMachineMemory, rd, __PROC__)))
	 return er;
      rd->os.lCurrent = 1;
   }

   rd->tagindexing = ro->name;
   bt = bt1_create(ro->unique, ro->bufsize + sizeof(unsigned int), _cdx_compare, ClipMachineMemory->index_buffer_limit);
//      bt = NULL;
   buf = malloc(ro->bufsize + sizeof(unsigned int));
   if (!rd->os.lCurrent)
   {
      for (rd->recno = s; rd->recno <= e; rd->recno++)
      {
	 rd->eof = 0;
	 if ((er = rd->vtbl->rawgo(ClipMachineMemory, rd, rd->recno, 0, __PROC__)))
	    return er;
	 /* check WHILE condition */
	 /* no need to check WHILE, because USECURRENT is ON with WHILE
	    clause. See above...
	    if(rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType){
	    if((er = rdd_calc(ClipMachineMemory,rd->area,&rd->os.bWhileCondition,&vv,0)))
	    return er;
	    vp = _clip_vptr(&vv);
	    if(vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType!=LOGICAL_type_of_ClipVarType)
	    return rdd_err(ClipMachineMemory,EG_DATATYPE,0,__FILE__,__LINE__,__PROC__,
	    er_badwhileexpr);
	    if(!vp->ClipLogVar_l_of_ClipVar.val){
	    _clip_destroy(ClipMachineMemory,&vv);
	    break;
	    }
	    _clip_destroy(ClipMachineMemory,&vv);
	    }
	  */
	 /* bEval every nInterval */
	 if (rd->keysincluded % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);

	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       break;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       continue;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 if (bt)
	 {
	    if (ro->simpexpr)
	    {
	       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		  return er;
	       vp = &vv;
	    }
	    else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	    }
	    memcpy(buf, &rd->recno, sizeof(unsigned int));
	    if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &vv);
	    if (bt1_add(bt, ro, buf))
	    {
	       if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	       {
		  bt1_destroy(bt);
		  return er;
	       }
	       bt1_destroy(bt);
	       bt = NULL;
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	 }
	 else
	 {
	    if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       return er;
	 }
	 rd->keysincluded++;
      }
      rd->keysincluded = 0;
   }
   else
   {
/*		if((er = rdd_gotop(ClipMachineMemory,rd,__PROC__))) return er;*/
      while (!rd->eof)
      {
	 rd->eof = 0;
	 /* check WHILE condition */
	 if (rd->os.bWhileCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bWhileCondition, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badwhileexpr);
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       break;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 /* bEval every nInterval */
	 if (rd->keysincluded % i == 0 && rd->os.bEval.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &rd->os.bEval, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badevalexpr);
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       break;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
	 {
	    if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	       return er;
	    vp = _clip_vptr(&vv);
	    if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	       return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
	    if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
	    {
	       _clip_destroy(ClipMachineMemory, &vv);
	       if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
		  return er;
	       continue;
	    }
	    _clip_destroy(ClipMachineMemory, &vv);
	 }
	 if (bt)
	 {
	    if (ro->simpexpr)
	    {
	       if ((er = rd->vtbl->getvalue(ClipMachineMemory, rd, ro->simpfno, &vv, __PROC__)))
		  return er;
	       vp = &vv;
	    }
	    else
	    {
	       if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
		  return er;
	       vp = _clip_vptr(&vv);
	    }
	    memcpy(buf, &rd->recno, sizeof(unsigned int));
	    if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &vv);
	    if (bt1_add(bt, ro, buf))
	    {
	       if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
	       {
		  bt1_destroy(bt);
		  return er;
	       }
	       bt1_destroy(bt);
	       bt = NULL;
	       if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
		  return er;
	    }
	 }
	 else
	 {
	    if ((er = cdx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       return er;
	 }
	 rd->keysincluded++;
	 if ((er = rdd_skip(ClipMachineMemory, rd, 1, __PROC__)))
	    return er;
      }
      rd->keysincluded = 0;
   }
   if (bt)
   {
      if ((er = _cdx_savebtree(ClipMachineMemory, ro, bt, lastrec, __PROC__)))
      {
	 bt1_destroy(bt);
	 return er;
      }
      bt1_destroy(bt);
   }
   free(buf);
   rd->tagindexing = NULL;
   return 0;
}
