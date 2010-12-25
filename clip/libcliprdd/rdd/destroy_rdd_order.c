void
destroy_rdd_order(ClipMachine * ClipMachineMemory, RDD_ORDER * ro)
{
   if (ro)
   {
      if (ro->name)
	 free(ro->name);
      if (ro->expr)
	 free(ro->expr);
      if (ro->scopetop)
	 free(ro->scopetop);
      if (ro->scopebottom)
	 free(ro->scopebottom);
      if (ro->cforexpr)
	 free(ro->cforexpr);
      if (ro->key)
	 free(ro->key);
      if (ro->curpage)
	 free(ro->curpage);
      _clip_destroy(ClipMachineMemory, &ro->block);
      _clip_destroy(ClipMachineMemory, &ro->scopetopvar);
      _clip_destroy(ClipMachineMemory, &ro->scopebottomvar);
      _clip_destroy(ClipMachineMemory, &ro->bforexpr);
      free(ro);
   }
}
