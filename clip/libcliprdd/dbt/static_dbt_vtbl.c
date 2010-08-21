static RDD_MEMO_VTBL *
dbt_vtbl()
{
   RDD_MEMO_VTBL *vtbl = (RDD_MEMO_VTBL *) malloc(sizeof(RDD_MEMO_VTBL));

   memset(vtbl, 0, sizeof(RDD_MEMO_VTBL));
   strcpy(vtbl->id, "DBT");
   strcpy(vtbl->suff, ".dbt");
   strcpy(vtbl->desc, "Generic DBT memo files driver v0.0.1 (c) 2001 Copyright ITK Ltd.");

   vtbl->create = dbt_create;
   vtbl->zap = dbt_zap;
   vtbl->open = dbt_open;
   vtbl->close = dbt_close;
   vtbl->getvalue = dbt_getvalue;
   vtbl->setvalue = dbt_setvalue;
   vtbl->info = dbt_info;
   return vtbl;
}
