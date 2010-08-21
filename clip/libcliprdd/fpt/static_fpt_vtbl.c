static RDD_MEMO_VTBL *
fpt_vtbl()
{
   RDD_MEMO_VTBL *vtbl = (RDD_MEMO_VTBL *) calloc(1, sizeof(RDD_MEMO_VTBL));

   strcpy(vtbl->id, "FPT");
   strcpy(vtbl->suff, ".fpt");
   strcpy(vtbl->desc, "Generic FPT memo files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");

   vtbl->create = fpt_create;
   vtbl->pack = fpt_pack;
   vtbl->zap = fpt_zap;
   vtbl->open = fpt_open;
   vtbl->close = fpt_close;
   vtbl->getvalue = fpt_getvalue;
   vtbl->setvalue = fpt_setvalue;
   vtbl->getvchar = fpt_getvchar;
   vtbl->setvchar = fpt_setvchar;
   vtbl->getvalue64 = NULL;
   vtbl->setvalue64 = NULL;
   vtbl->getvchar64 = NULL;
   vtbl->setvchar64 = NULL;
   vtbl->info = fpt_info;
   return vtbl;
}
