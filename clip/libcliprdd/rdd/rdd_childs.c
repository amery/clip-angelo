int
rdd_childs(ClipMachine * ClipMachineMemory, RDD_DATA * rd, const char *__PROC__)
{
   RDD_DATA *child;

   int i, er;

   for (i = 0; i < rd->rels_opened; i++)
   {
      child = rd->relations[i]->child;
      child->pending_child_parent = rd;
      if ((er = rdd_childs(ClipMachineMemory, child, __PROC__)))
	 return er;
   }
   return 0;
}
