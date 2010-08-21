void
destroy_rdd_index(void *index)
{
   RDD_INDEX *ri = (RDD_INDEX *) index;

   if (ri)
   {
      if (ri->name)
	 free(ri->name);
      if (ri->path)
	 free(ri->path);
      if (ri->orders)
	 free(ri->orders);
      free(ri);
   }
}
