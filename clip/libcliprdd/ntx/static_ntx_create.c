static int
ntx_create(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri,
	   RDD_ORDER ** rop, const char *tag, const char *expr, ClipVar * block, int unique, int first, unsigned int header, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   NTX_HEADER hdr;

   NTX_PAGE page;

   ClipVar vv, *vp;

   int lastrec;

   int s, e, i, er;

   BTREE *bt;

   void *buf;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;
   *rop = ro;

   ro->header = header;
   ro->name = (char *) calloc(1, strlen(tag ? tag : ri->name) + 1);
   strcpy(ro->name, tag ? tag : ri->name);

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      ro->simpexpr = 1;

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
   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
      return er;
   vp = _clip_vptr(&vv);

   switch (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType)
   {
   case CHARACTER_type_of_ClipVarType:
      ro->keysize = vp->ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf;
      ro->dec = 0;
      ro->type = 'C';
      break;
   case NUMERIC_type_of_ClipVarType:
      ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vp->ClipType_t_of_ClipVar.len_of_ClipType : 10);
      ro->dec = vp->ClipType_t_of_ClipVar.dec_of_ClipType;
      ro->type = 'N';
      break;
   case LOGICAL_type_of_ClipVarType:
      ro->keysize = 1;
      ro->dec = 0;
      ro->type = 'L';
      break;
   case DATE_type_of_ClipVarType:
      ro->keysize = 8;
      ro->dec = 0;
      ro->type = 'D';
      break;
   default:
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
   }
   _clip_destroy(ClipMachineMemory, &vv);

   ro->unique = unique;
   ro->descend = rd->os.lDescend;
   ro->custom = rd->os.lCustom;

   ro->key = (char *) malloc(ro->keysize + 1);
   ro->fullpage = ((((1024 - 2) / (ro->keysize + 2 * 4 + 2)) - 1) / 2) * 2;
   if (ro->fullpage < 4)
      return rdd_err(ClipMachineMemory, EG_CREATE, 0, __FILE__, __LINE__, __PROC__, er_longkey);
   ro->halfpage = ro->fullpage / 2;
   if (rd->os.bForCondition.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ro->cforexpr = malloc(strlen(rd->os.cForCondition) + 1);
      strcpy(ro->cforexpr, rd->os.cForCondition);
      _clip_clone(ClipMachineMemory, &ro->bforexpr, &rd->os.bForCondition);
   }
   ro->curpage = calloc(1, sizeof(NTX_PAGE));
   ro->curoffs = 0;

   ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
   ri->orders[ri->norders] = ro;
   ri->norders++;

   memset(&hdr, 0, sizeof(NTX_HEADER));
   _rdd_put_ushort(hdr.sig, 6);
   _rdd_put_ushort(hdr.ver, 1);
   _rdd_put_uint(hdr.fuu, 0);
   _rdd_put_ushort(hdr.itemsize, ro->keysize + 8);
   _rdd_put_ushort(hdr.keysize, ro->keysize);
   _rdd_put_ushort(hdr.dec, ro->dec);
   _rdd_put_ushort(hdr.fullpage, ro->fullpage);
   _rdd_put_ushort(hdr.halfpage, ro->halfpage);
   strncpy(hdr.key, ro->expr, sizeof(hdr.key));
   loc_write(rd->loc, (unsigned char *) hdr.key, strlen(hdr.key));
   if (rd->os.cForCondition)
      strcpy(hdr.forexpr, rd->os.cForCondition);
   hdr.unique = ro->unique;
   hdr.descend = ro->descend;
   hdr.custom = ro->custom;
   if (tag)
      strcpy(hdr.tag, tag);

   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
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
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
      return er;

   if ((er = _ntx_init_page(ClipMachineMemory, ro, &page, 0, __PROC__)))
      return er;
   if ((er = rdd_write(ClipMachineMemory, &ri->file, _rdd_uint(hdr.root), sizeof(NTX_PAGE), (char *) &page, __PROC__)))
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
   if (ro->descend)
      bt = NULL;
   else
      bt = bt_create(ro->unique, lastrec, ro->keysize + sizeof(unsigned int) + 1, _ntx_comp);
   buf = malloc(ro->keysize + sizeof(unsigned int) + 1);
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
	    if(vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType!=LOGICAL_t)
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
	    if ((er = ntx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &vv);
	    bt_add(bt, ro, buf);
	 }
	 else
	 {
	    if ((er = ntx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
	       return er;
	 }
	 rd->keysincluded++;
      }
      rd->keysincluded = 0;
   }
   else
   {
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
	    if ((er = ntx_formatkey(ClipMachineMemory, ro, vp, buf + sizeof(unsigned int), __PROC__)))
	       return er;
	    _clip_destroy(ClipMachineMemory, &vv);
	    bt_add(bt, ro, buf);
	 }
	 else
	 {
	    if ((er = ntx_addkey(ClipMachineMemory, rd, ro, NULL, __PROC__)))
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
      if ((er = _ntx_savebtree(ClipMachineMemory, rd, ro, bt, __PROC__)))
      {
	 bt_destroy(bt);
	 return er;
      }
      bt_destroy(bt);
   }
   free(buf);
   rd->tagindexing = NULL;
   return 0;
}
