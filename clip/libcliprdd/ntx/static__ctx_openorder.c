static int
_ctx_openorder(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, CTX_TAG * tag, RDD_ORDER ** pro, const char *__PROC__)
{
   RDD_ORDER *ro;

   NTX_HEADER hdr;

   int er;

   ro = *pro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));
   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;

   ro->name = (char *) malloc(strlen((const char *) tag->tagname) + 1);
   strcpy(ro->name, (const char *) tag->tagname);
   ro->orderno = rd->ords_opened - 1;
   ro->header = _rdd_uint(tag->header);

   if ((er = rdd_read(ClipMachineMemory, &ri->file, ro->header, sizeof(NTX_HEADER), &hdr, __PROC__)))
      return er;

   ro->expr = (char *) malloc(strlen(hdr.key) + 1);
   strcpy(ro->expr, hdr.key);
   loc_read(rd->loc, (unsigned char *) ro->expr, strlen(ro->expr));
   if ((ro->simpfno = _rdd_fieldno(rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      ro->simpexpr = 1;
   {
      int r = _clip_parni(ClipMachineMemory, 1);

      char expr[1024];

      memset(expr, 0, sizeof(expr));
      rdd_expandmacro(rd->area, r, ro->expr, expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->block)))
	 return er;
   }
   ro->unique = hdr.unique;
   ro->descend = hdr.descend;
   ro->custom = hdr.custom;
   ro->keysize = _rdd_ushort(hdr.keysize);
   ro->dec = _rdd_ushort(hdr.dec);

   ro->key = (char *) malloc(ro->keysize + 1);
   ro->fullpage = _rdd_ushort(hdr.fullpage);
   ro->halfpage = _rdd_ushort(hdr.halfpage);

   ro->curpage = calloc(1, sizeof(NTX_PAGE));
   ro->curoffs = 0;

   if (hdr.forexpr[0])
   {
      int r = _clip_parni(ClipMachineMemory, 1);

      char expr[1024];

      ro->cforexpr = malloc(strlen(hdr.forexpr) + 1);
      strcpy(ro->cforexpr, hdr.forexpr);

      memset(expr, 0, sizeof(expr));
      rdd_expandmacro(rd->area, r, ro->cforexpr, expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->bforexpr)))
	 return er;
   }
   ri->norders++;
   return 0;
}
