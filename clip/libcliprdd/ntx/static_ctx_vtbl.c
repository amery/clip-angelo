static RDD_INDEX_VTBL *
ctx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "CTX");
   strcpy(vtbl->suff, ".ctx");
   strcpy(vtbl->sing_suff, ".ntx");
   strcpy(vtbl->desc, "Compound NTX (CTX) index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 1;

   vtbl->open = ctx_open;
   vtbl->close = ctx_close;
   vtbl->create = ctx_create;
   vtbl->zap = ctx_zap;
   vtbl->reindex = ctx_reindex;
   vtbl->gotop = ntx_gotop;
   vtbl->gobottom = ntx_gobottom;
   vtbl->next = ntx_next;
   vtbl->prev = ntx_prev;
   vtbl->seek = ntx_seek;
   vtbl->addkey = ntx_addkey;
   vtbl->delkey = ntx_delkey;
   vtbl->destroy = ctx_destroy;
   vtbl->keyno = ntx_keyno;
   vtbl->lastkey = ntx_lastkey;
   vtbl->gotokey = ntx_gotokey;
   vtbl->descend = ntx_descend;
   vtbl->buildtree = ntx_buildtree;
   vtbl->info = ntx_info;
   vtbl->keyvalue = ntx_keyvalue;
   /*vtbl->setscope        = ntx_setscope; */
   vtbl->formatkey = ntx_formatkey;
   vtbl->_rlock = ntx__rlock;
   vtbl->_wlock = ntx__wlock;
   vtbl->_ulock = ntx__ulock;
   return vtbl;
}
