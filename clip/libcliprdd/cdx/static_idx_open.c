static int
idx_open(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_INDEX * ri, const char *tag, const char *__PROC__)
{
   RDD_ORDER *ro = NULL;

   int er;

   unsigned int newheader;

   ri->loc = rd->loc;
   newheader = 0;
   ro = (RDD_ORDER *) calloc(1, sizeof(RDD_ORDER));
   if ((er = _cdx_init_order(ClipMachineMemory, rd->area, ri, newheader, ro, __PROC__)))
      goto err;

   rd->ords_opened++;
   rd->orders = (RDD_ORDER **) realloc(rd->orders, sizeof(RDD_ORDER *) * rd->ords_opened);
   rd->orders[rd->ords_opened - 1] = ro;
   ro->name = strdup(ri->name);
   ro->orderno = rd->ords_opened - 1;
   ri->norders = 1;
   ri->orders = (RDD_ORDER **) realloc(ri->orders, ri->norders * sizeof(RDD_ORDER *));
   ri->orders[ri->norders - 1] = ro;
   ro->canadd = 1;
   return 0;
 err:
   if (ro)
      free(ro);
   return er;
}
