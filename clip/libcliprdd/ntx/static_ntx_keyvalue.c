static int
ntx_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   NTX_HEADER hdr;

   NTX_PAGE page;

   NTX_BUCKET *buck;

   int r, er;

   ClipVar vv, *vp;

   if (rd->eof)
      return 0;

   _ntx_header(ro, &hdr);
   ro->stack[0].page = _rdd_uint(hdr.root);
   ro->level = 0;

   if ((er = _ntx_calc_key(ClipMachineMemory, rd, ro, __PROC__)))
      return er;

   if ((er = _ntx_search_tree(ClipMachineMemory, rd, ro, ro->key, ro->keysize, rd->recno, &r, 1, __PROC__)))
      return er;
   if (r)
      return 0;

   _ntx_page(ro, ro->stack[ro->level].page, &page);

   buck = _ntx_buck(&page, ro->stack[ro->level].pos);
   if (rd->recno != _rdd_uint(buck->recno))
      return 0;

   if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
      return er;
   vp = _clip_vptr(&vv);
   _clip_clone(ClipMachineMemory, v, vp);
   _clip_destroy(ClipMachineMemory, &vv);
   return 0;
}
