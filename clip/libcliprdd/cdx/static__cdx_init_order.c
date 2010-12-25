static int
_cdx_init_order(ClipMachine * ClipMachineMemory, int area, RDD_INDEX * ri, unsigned int header, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int er;

   if ((er = rdd_read(ClipMachineMemory, &ri->file, header, sizeof(CDX_HEADER), &hdr, __PROC__)))
      return er;
   memset(ro, 0, sizeof(RDD_ORDER));
   ro->expr = malloc(_rdd_ushort(hdr.explen) + 1);
   memcpy(ro->expr, hdr.keyforexpr, _rdd_ushort(hdr.explen));
   ro->expr[_rdd_ushort(hdr.explen)] = 0;
   if (ri->rd)
   {
      if ((ro->simpfno = _rdd_fieldno(ri->rd, _clip_casehashword(ro->expr, strlen(ro->expr)))) != -1)
      {
	 ro->simpexpr = 1;
	 if (ri->rd->fields[ro->simpfno].binary)
	    ro->binary = 1;
	 ro->type = ri->rd->fields[ro->simpfno].type;
      }
   }
   if (!ro->binary)
      loc_read(ri->loc, (unsigned char *) ro->expr, strlen(ro->expr));
   ro->cforexpr = malloc(_rdd_ushort(hdr.forlen) + 1);
   memcpy(ro->cforexpr, hdr.keyforexpr + _rdd_ushort(hdr.explen), _rdd_ushort(hdr.forlen));
   ro->cforexpr[_rdd_ushort(hdr.forlen)] = 0;
   if (!ro->binary)
      loc_read(ri->loc, (unsigned char *) ro->cforexpr, strlen(ro->cforexpr));
   ro->header = header;

   ro->ic = hdr.ic;
   ro->unique = (hdr.options & 0x0001 ? 1 : 0);
   ro->custom = (hdr.options & 0x0004 ? 1 : 0);
   ro->descend = (_rdd_ushort(hdr.sort) ? 1 : 0);
   ro->keysize = _rdd_ushort(hdr.keysize);
   ro->curpage = calloc(1, sizeof(CDX_PAGE));
   ro->curoffs = 0;
   if (strlen(ro->expr) == 0)
   {				/* bag order */
      ro->type = 'C';
      ro->bufsize = 10;
   }
   else
   {				/* calc expr and determine key type */
      int r = _clip_parni(ClipMachineMemory, 1);

      char expr[1024];

      ClipVar v, *vp;

      memset(expr, 0, sizeof(expr));
      rdd_expandmacro(area, r, ro->expr, expr);

      if ((er = _clip_eval_macro(ClipMachineMemory, expr, strlen(expr), &ro->block)))
	 return er;

      if (ro->type == 'X')
      {
	 ro->bufsize = ri->rd->fields[ro->simpfno].len - 1;
      }
      else
      {
	 if ((er = rdd_calc(ClipMachineMemory, area, &ro->block, &v, 0)))
	    return er;
	 vp = _clip_vptr(&v);
	 if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == CHARACTER_type_of_ClipVarType)
	 {
	    ro->type = 'C';
	    ro->bufsize = ro->keysize;
	 }
	 else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == NUMERIC_type_of_ClipVarType)
	 {
	    ro->type = 'N';
	    ro->bufsize = sizeof(double);
	 }
	 else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATE_type_of_ClipVarType)
	 {
	    ro->type = 'D';
	    ro->bufsize = sizeof(double);
	 }
	 else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == DATETIME_type_of_ClipVarType)
	 {
	    ro->type = 'T';
	    ro->bufsize = 8;
	 }
	 else if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType == LOGICAL_type_of_ClipVarType)
	 {
	    ro->type = 'L';
	    ro->bufsize = 1;
	 }
	 else
	    ro->type = 'U';
	 _clip_destroy(ClipMachineMemory, &v);
      }
   }
   ro->key = malloc(ro->bufsize);
   ro->index = ri;
   ro->vtbl = ri->vtbl;
   return 0;
}
