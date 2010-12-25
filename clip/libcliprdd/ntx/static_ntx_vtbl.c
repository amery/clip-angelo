static RDD_INDEX_VTBL *
ntx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "NTX");
   strcpy(vtbl->suff, ".ntx");
   strcpy(vtbl->sing_suff, ".ntx");
   strcpy(vtbl->desc, "Generic NTX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 0;		/* one order per file */

   vtbl->open = ntx_open;
   vtbl->close = ntx_close;
   vtbl->create = ntx_create;
   vtbl->zap = ntx_zap;
   vtbl->reindex = ntx_reindex;
   vtbl->gotop = ntx_gotop;
   vtbl->gobottom = ntx_gobottom;
   vtbl->next = ntx_next;
   vtbl->prev = ntx_prev;
   vtbl->seek = ntx_seek;
   vtbl->addkey = ntx_addkey;
   vtbl->delkey = ntx_delkey;
   vtbl->destroy = ntx_destroy;
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
