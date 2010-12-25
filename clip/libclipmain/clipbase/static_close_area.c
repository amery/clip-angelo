static int
close_area(ClipMachine * ClipMachineMemory, DBWorkArea * wa, const char *__PROC__)
{
   int er = 0;

   if (wa)
   {
      if (wa->rd)
      {
	 int re;

	 if ((er = rdd_event(ClipMachineMemory, EVENT_PRECLOSE, wa->rd->area, 0, NULL, &re, __PROC__)))
	    return er;
	 if (!re)
	    return 0;

	 if ((er = rdd_clearrelation(ClipMachineMemory, wa->rd, __PROC__)))
	    return er;
	 if ((er = _clip_flushbuffer(ClipMachineMemory, wa, __PROC__)))
	    return er;
	 if (wa->rd->readonly)
	    wa->rd->vtbl->_rlock(ClipMachineMemory, wa->rd, __PROC__);
	 else
	    wa->rd->vtbl->_wlock(ClipMachineMemory, wa->rd, __PROC__);
	 if ((er = rdd_closearea(ClipMachineMemory, wa->rd, __PROC__)))
	    return er;
	 if ((er = rdd_event(ClipMachineMemory, EVENT_POSTCLOSE, wa->rd->area, 0, NULL, &re, __PROC__)))
	    return er;
	 wa->rd = NULL;
      }
      _clip_destroy_c_item(ClipMachineMemory, wa->rd_item, _C_ITEM_TYPE_RDD);
      HashTable_remove(ClipMachineMemory->aliases, wa->aliasHash);
      ClipMachineMemory->areas->items_of_ClipVect[wa->no] = NULL;
      if (wa->trigger)
	 free(wa->trigger);
      free(wa->name);
      free(wa->alias);
      free(wa);
   }
   return 0;
}
