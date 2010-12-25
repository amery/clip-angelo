static RDD_INDEX_VTBL *
cdx_vtbl()
{
   RDD_INDEX_VTBL *vtbl = (RDD_INDEX_VTBL *) malloc(sizeof(RDD_INDEX_VTBL));

   memset(vtbl, 0, sizeof(RDD_INDEX_VTBL));
   strcpy(vtbl->id, "CDX");
   strcpy(vtbl->suff, ".cdx");
   strcpy(vtbl->sing_suff, ".idx");
   strcpy(vtbl->desc, "Generic CDX index files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");
   vtbl->ismulti = 1;

   vtbl->open = cdx_open;
   vtbl->close = cdx_close;
   vtbl->create = cdx_create;
   vtbl->reindex = cdx_reindex;
   vtbl->zap = cdx_zap;
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
   vtbl->ii_create = cdx_ii_create;
   vtbl->ii_createtag = cdx_ii_createtag;
   vtbl->ii_open = cdx_ii_open;
   vtbl->ii_opentag = cdx_ii_opentag;
   vtbl->ii_addkey = cdx_ii_addkey;
   vtbl->ii_delkey = cdx_ii_delkey;
   vtbl->ii_gotop = cdx_ii_gotop;
   vtbl->ii_gobottom = cdx_ii_gobottom;
   vtbl->ii_id = cdx_ii_id;
   vtbl->ii_key = cdx_ii_key;
   vtbl->ii_next = cdx_ii_next;
   vtbl->ii_prev = cdx_ii_prev;
   vtbl->calcfiltlist = cdx_calcfiltlist;
   vtbl->_rlock = cdx__rlock;
   vtbl->_wlock = cdx__wlock;
   vtbl->_ulock = cdx__ulock;
   return vtbl;
}
