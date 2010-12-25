void
destroy_rdd_memo(RDD_MEMO * rm)
{
   if (rm)
   {
      if (rm->name)
	 free(rm->name);
      if (rm->path)
	 free(rm->path);
      free(rm);
   }
}
