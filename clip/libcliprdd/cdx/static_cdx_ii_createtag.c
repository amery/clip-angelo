static int
cdx_ii_createtag(ClipMachine * ClipMachineMemory, RDD_INDEX * ri, const char *tag, const char *expr, RDD_ORDER ** rop, const char *__PROC__)
{
   CDX_HEADER chdr;

   RDD_ORDER *tagbag = malloc(sizeof(RDD_ORDER));

   char tagname[11];

   char tn[11];

   unsigned int newheader, dummy;

   ClipVar v, vv, *vp;

   ClipVar block;

   int found, er;

   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   CDX_HEADER hdr;

   unsigned int newroot;

   ro->level = 0;
   memset(&v, 0, sizeof(ClipVar));
   memset(&vv, 0, sizeof(ClipVar));
   memset(&block, 0, sizeof(ClipVar));

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(CDX_HEADER), &chdr, __PROC__)))
      goto err;
   if ((er = _cdx_init_order(ClipMachineMemory, -1, ri, 0, tagbag, __PROC__)))
      goto err;

   tagbag->unique = 1;
   strncpy(tagname, tag, 10);
   tagname[10] = 0;
   _clip_upstr(tagname, strlen(tagname));
   v.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = CHARACTER_type_of_ClipVarType;
   v.ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.buf_of_ClipBuf = tagname;
   v.ClipStrVar_s_of_ClipVar.ClipBuf_str_of_ClipStrVar.len_of_ClipBuf = strlen(tagname);
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newheader, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &dummy, __PROC__)))
      goto err;

   strcpy(tn, tagname);
   memset(tn + strlen(tn), 0x20, 10 - strlen(tn));
   tn[10] = 0;
   tagbag->stack[0].page = _rdd_uint(chdr.root);
   if ((er = _cdx_search_tree(ClipMachineMemory, tagbag, tn, 10, 0, &found, NULL, __PROC__)))
      goto err;
   if (found)
   {
      if ((er = _cdx_delkey(ClipMachineMemory, tagbag, __PROC__)))
	 goto err;
   }
   if ((er = __cdx_addkey(ClipMachineMemory, -1, tagbag, &v, ri->loc, newheader, __PROC__)))
      goto err;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;

   ro->header = newheader;
   ro->name = (char *) malloc(strlen(tag) + 1);
   strcpy(ro->name, tag);
   ro->orderno = -1;

   ro->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(ro->expr, expr);
   {
      char e[1024];

      sprintf(e, "{||%s}", ro->expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, e, strlen(e), &block)))
	 return er;
   }
   if ((er = rdd_calc(ClipMachineMemory, -1, &block, &vv, 0)))
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
      ro->keysize = (vp->ClipType_t_of_ClipVar.len_of_ClipType ? vp->ClipType_t_of_ClipVar.len_of_ClipType : 10);
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
      return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_baddata);
   }
   ro->key = malloc(ro->bufsize);
   ro->iikey = malloc(ro->bufsize);

   ri->orders = (RDD_ORDER **) realloc(ri->orders, (ri->norders + 1) * sizeof(RDD_ORDER *));
   ri->orders[ri->norders] = ro;
   ri->norders++;

   memset(&hdr, 0, sizeof(CDX_HEADER));
   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      goto err;
   if ((er = _cdx_getfreepage(ClipMachineMemory, ri, &newroot, __PROC__)))
      goto err;
   _rdd_put_uint(hdr.root, newroot);
   _rdd_put_uint(hdr.keysize, ro->bufsize);
   hdr.options = 0x62;
   hdr.sign = 1;
   _rdd_put_ushort(hdr.sort, ro->descend);
   _rdd_put_ushort(hdr.explen, strlen(ro->expr) + 1);
   _rdd_put_ushort(hdr.totlen, _rdd_ushort(hdr.explen));
   strcpy((char *) hdr.keyforexpr, ro->expr);
   _rdd_put_ushort(hdr.forlen, 1);

   if ((er = rdd_write(ClipMachineMemory, &ri->file, ro->header, sizeof(CDX_HEADER), (char *) &hdr, __PROC__)))
      goto err;

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
      goto err;

   if ((er = _cdx_init_rootleaf(ClipMachineMemory, ro, newroot, __PROC__)))
      goto err;

   *rop = ro;

   _clip_destroy(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &block);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return 0;
 err:
   _clip_destroy(ClipMachineMemory, &vv);
   _clip_destroy(ClipMachineMemory, &block);
   destroy_rdd_order(ClipMachineMemory, tagbag);
   return er;
}
