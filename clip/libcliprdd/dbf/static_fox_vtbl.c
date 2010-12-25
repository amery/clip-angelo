static RDD_DATA_VTBL *
fox_vtbl()
{
   RDD_DATA_VTBL *vtbl = (RDD_DATA_VTBL *) malloc(sizeof(RDD_DATA_VTBL));

   memset(vtbl, 0, sizeof(RDD_DATA_VTBL));
   strcpy(vtbl->id, "FOX");
   strcpy(vtbl->suff, ".dbf");
   strcpy(vtbl->desc, "Generic FoxPro data files driver v0.0.1 (c) 2002 Copyright ITK Ltd.");
   vtbl->dbfsig = 0x03;
   vtbl->dbfwithmemosig = 0xF5;

   vtbl->open = dbf_open;
   vtbl->create = dbf_create;
   vtbl->pack = dbf_pack;
   vtbl->zap = dbf_zap;
   vtbl->close = dbf_close;
   vtbl->lastrec = dbf_lastrec;
   vtbl->deleted = dbf_deleted;
   vtbl->gotop = dbf_gotop;
   vtbl->gobottom = dbf_gobottom;
   vtbl->next = dbf_next;
   vtbl->prev = dbf_prev;
   vtbl->go = dbf_goto;
   vtbl->rawgo = dbf_rawgoto;
   vtbl->append = dbf_append;
   vtbl->getrecord = dbf_getrecord;
   vtbl->getvalue = dbf_getvalue;
   vtbl->setrecord = dbf_setrecord;
   vtbl->setvalue = dbf_setvalue;
   vtbl->getmemoid = dbf_getmemoid;
   vtbl->setmemoid = dbf_setmemoid;
   vtbl->delete = dbf_delete;
   vtbl->recall = dbf_recall;
   vtbl->rlock = dbf_rlock;
   vtbl->flock = dbf_flock;
   vtbl->ulock = dbf_ulock;
   vtbl->rlocked = dbf_rlocked;
   vtbl->forlock = dbf_forlock;
   vtbl->_rlock = dbf__rlock;
   vtbl->_wlock = dbf__wlock;
   vtbl->_ulock = dbf__ulock;
   vtbl->lupdate = dbf_lupdate;
   vtbl->info = dbf_info;
   vtbl->setstruct = dbf_setstruct;
   vtbl->verify = dbf_verify;
   vtbl->calcfiltlist = dbf_calcfiltlist;
   return vtbl;
}
