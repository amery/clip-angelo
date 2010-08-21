void
destroy_rdd_i_order(void *order)
{
   RDD_ORDER *ro = (RDD_ORDER *) order;

   if (ro)
      free(ro);
}
