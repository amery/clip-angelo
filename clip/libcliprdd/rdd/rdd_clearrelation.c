int
rdd_clearrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   int i;

   for (i = 0; i < rd->rels_opened; i++)
   {
      rd->relations[i]->child->pending_child_parent = NULL;
      _rdd_clearparent(ClipMachineMemory, rd->relations[i]->child, rd);
      if (rd->relations[i]->expr)
	 free(rd->relations[i]->expr);
      _clip_destroy(ClipMachineMemory, &rd->relations[i]->block);
      free(rd->relations[i]);
   }
   free(rd->relations);
   rd->relations = NULL;
   rd->rels_opened = 0;
   return 0;
}
