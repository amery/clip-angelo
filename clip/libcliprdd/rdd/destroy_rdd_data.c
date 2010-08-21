void
destroy_rdd_data(void *data)
{
   RDD_DATA *rd = (RDD_DATA *) data;

   if (rd)
   {
      if (rd->name)
	 free(rd->name);
      if (rd->path)
	 free(rd->path);
      if (rd->fields)
	 free(rd->fields);
      if (rd->indices)
	 free(rd->indices);
      if (rd->orders)
	 free(rd->orders);
      if (rd->word)
	 free(rd->word);
      if (rd->yylval)
	 free(rd->yylval);
      if (rd->locks)
	 free(rd->locks);
      if (rd->os.cForCondition)
	 free(rd->os.cForCondition);
      if (rd->data)
	 free(rd->data);
      if (rd->record)
	 free(rd->record);
      delete_HashTable(rd->hashes);
      free(rd);
   }
}
