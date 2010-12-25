static int
cdx_delkey(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_ORDER * ro, const char *__PROC__)
{
   CDX_HEADER hdr;

   int found, er;

   ClipVar vv, *vp;

   void *key = malloc(ro->bufsize);

   char cntcdx[4];

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
      goto cont;

   if ((er = _cdx_delkey(ClipMachineMemory, ro, __PROC__)))
      goto err;
   ro->valid_stack = 0;

   if ((er = rdd_read(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
      goto err;
   ro->cntcdx = _rdd_backuint((unsigned char *) cntcdx) + 1;
   _rdd_put_backuint((unsigned char *) cntcdx, ro->cntcdx);
   if ((er = rdd_write(ClipMachineMemory, &ro->index->file, 8, 4, cntcdx, __PROC__)))
      goto err;
   goto cont;
 err:
   free(key);
   return er;
 cont:
   free(key);
   ro->curoffs = 0;
   return 0;
}
