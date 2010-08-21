static int
_rdd_clearparent(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_DATA * parent)
{
   int i;

   for (i = 0; i < rd->pars_opened; i++)
   {
      if (rd->parents[i]->parent == parent)
      {
	 memcpy(rd->parents + i, rd->parents + i + 1, (rd->pars_opened - 1) * sizeof(void *));
	 rd->pars_opened--;
	 rd->parents = realloc(rd->parents, rd->pars_opened * sizeof(void *));
      }
   }
   return 0;
}
