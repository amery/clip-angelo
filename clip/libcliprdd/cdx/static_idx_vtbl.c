static RDD_INDEX_VTBL *
idx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "IDX");
   strcpy(vtbl->suff, ".idx");
   strcpy(vtbl->sing_suff, ".idx");
   strcpy(vtbl->desc, "Generic IDX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 0;

   vtbl->open = idx_open;
   vtbl->close = cdx_close;
   vtbl->create = idx_create;
   vtbl->reindex = idx_reindex;
   vtbl->zap = idx_zap;
   vtbl->gotop = cdx_gotop;
   vtbl->gobottom = cdx_gobottom;
   vtbl->next = cdx_next;
   vtbl->prev = cdx_prev;
   vtbl->seek = cdx_seek;
   vtbl->gotokey = cdx_gotokey;
   vtbl->descend = cdx_descend;
   vtbl->buildtree = cdx_buildtree;
   vtbl->wildskip = cdx_wildskip;
   vtbl->addkey = cdx_addkey;
   vtbl->delkey = cdx_delkey;
   vtbl->destroy = cdx_destroy;
   vtbl->formatkey = cdx_formatkey;
   vtbl->keyno = cdx_keyno;
   vtbl->lastkey = cdx_lastkey;
   vtbl->info = cdx_info;
   vtbl->keyvalue = cdx_keyvalue;
   vtbl->setscope = cdx_setscope;
   vtbl->calcfiltlist = cdx_calcfiltlist;
   vtbl->_rlock = cdx__rlock;
   vtbl->_wlock = cdx__wlock;
   vtbl->_ulock = cdx__ulock;
   return vtbl;
}
