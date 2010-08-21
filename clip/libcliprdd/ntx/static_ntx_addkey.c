static int
ntx_addkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_BUCKET *bp;

   NTX_PAGE page;

   int r, er;

   if (ro->bforexpr.ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != UNDEF_type_of_ClipVarType)
   {
      ClipVar vv, *vp;

      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->bforexpr, &vv, 0)))
	 return er;
      vp = _clip_vptr(&vv);
      if (vp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != LOGICAL_type_of_ClipVarType)
	 return rdd_err(ClipMachineMemory, EG_DATATYPE, 0, __FILE__, __LINE__, __PROC__, er_badforexpr);
      if (!vp->ClipLogVar_l_of_ClipVar.value_of_ClipLogVar)
      {
	 _clip_destroy(ClipMachineMemory, &vv);
	 return 0;
      }
      _clip_destroy(ClipMachineMemory, &vv);
   }

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if (v)
   {
      if ((er = ntx_formatkey(ClipMachineMemory, ro, v, ro->key, __PROC__)))
	 return er;
   }
   else
   {
      if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
	 return er;
   }

   if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;

   _ntx_page(ro, ro->stack[ro->level].page, &page);
   bp = _ntx_buck(&page, _rdd_ushort(page.nkeys));

   ro->valid_stack = 0;

   if (!r && ro->unique)
      return 0;
   else if ((er = _ntx_add_node(ClipMachineMemory, rd, ro, ro->key, rd->recno, _rdd_uint(bp->left), __PROC__)))
      return er;
   rd->eof = 0;
   ro->curoffs = 0;
   return 0;
}
