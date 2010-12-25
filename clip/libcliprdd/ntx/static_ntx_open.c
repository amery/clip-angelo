static int
ntx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *ro = (RDD_ORDER *) malloc(sizeof(RDD_ORDER));

   NTX_HEADER hdr;

   int er;

   memset(ro, 0, sizeof(RDD_ORDER));
   ro->vtbl = ri->vtbl;
   ro->index = ri;
   ro->canadd = 1;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;

   ro->orderno = rd->ords_opened - 1;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, 0, sizeof(NTX_HEADER), (char *) &hdr, __PROC__)))
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
   if (hdr.tag[0])
   {
      ro->name = malloc(strlen(hdr.tag) + 1);
      strcpy(ro->name, hdr.tag);
   }
   else
   {
      ro->name = strdup(ri->name);
   }
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

   ri->orders = (RDD_ORDER **) malloc(sizeof(RDD_ORDER *));
   ri->orders[0] = ro;
   ri->norders = 1;

   return 0;
}
