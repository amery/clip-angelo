static int
cdx_keyvalue(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, ClipVar * v, const char *__PROC__)
{
   CDX_HEADER hdr;

   ClipVar vv, *vp;

   int found, er;

   void *key = malloc(ro->bufsize);

   if (rd->eof)
      return 0;
   if (ro->custom)
   {
      _clip_var_str(ro->key, ro->keysize, &vv);
      vp = _clip_vptr(&vv);
   }
   else
   {
      ro->level = 0;
      _CDX_HEADER(ro, &hdr);
      ro->stack[0].page = _rdd_uint(hdr.root);

      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	 goto err;
      vp = _clip_vptr(&vv);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	 goto err;
      _clip_destroy(ClipMachineMemory, &vv);

      if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	 goto err;
      if (!found)
	 return 0;
      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &vv, 0)))
	 goto err;
      vp = _clip_vptr(&vv);
   }
   _clip_clone(ClipMachineMemory, v, vp);
   _clip_destroy(ClipMachineMemory, &vv);
   free(key);
   return 0;
 err:
   free(key);
   return er;
}
