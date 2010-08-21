static int
cdx_buildtree(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   ClipVar v, *vp;

   int found, er;

   void *key = NULL;

   if (!rd->shared && !rd->eof)
   {
      key = malloc(ro->bufsize);
      if ((er = rdd_calc(ClipMachineMemory, rd->area, &ro->block, &v, 0)))
	 goto err;
      vp = _clip_vptr(&v);
      if ((er = cdx_formatkey(ClipMachineMemory, ro, vp, key, __PROC__)))
	 goto err;
      _clip_destroy(ClipMachineMemory, &v);

      _CDX_HEADER(ro, &hdr);
      ro->stack[0].page = _rdd_uint(hdr.root);
      ro->level = 0;

      if (!ro->descend)
      {
	 if ((er = _cdx_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	    goto err;
      }
      else
      {
	 if ((er = _cdx_back_search_tree(ClipMachineMemory, ro, key, ro->bufsize, rd->recno, &found, NULL, __PROC__)))
	    goto err;
      }
      free(key);
      _clip_destroy(ClipMachineMemory, &v);
/*		if(!found){
			return rdd_err(ClipMachineMemory,EG_CORRUPTION,0,__FILE__,__LINE__,__PROC__,
				er_nokey);
		}*/
      ro->valid_stack = 1;
   }
   return 0;
 err:
   if (key)
      free(key);
   return er;
}
