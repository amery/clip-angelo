int
clip_DEPSORT(ClipMachine * ClipMachineMemory)
{
   ClipVar *ap = _clip_par(ClipMachineMemory, 1);

   ClipVar *tap = _clip_par(ClipMachineMemory, 2);

   ClipVar *rp, *rap;

   int i, setno;

   Coll graph;

   if (!ap || ap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType != ARRAY_type_of_ClipVarType)
      return EG_ARG;

   init_Coll(&graph, delete_DepNode, cmp_DepNode);

   for (i = 0; i < ap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar; i++)
   {
      DepNode *np = new_DepNode(ClipMachineMemory, ap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + i);

      insert_Coll(&graph, np);
   }

   setno = 0;

   if (tap)
   {
      DepNode *tp = new_DepNode(ClipMachineMemory, tap);

      DepNode *start = find_node(&graph, tp->hash);

      if (!start)
	 start = tp;

      while (depth_search(&graph, start, &setno))
	 ;

      delete_DepNode(tp);
   }
   else
   {
		for (i = 0; i < graph.count_of_Coll; i++)
      {
			DepNode *dp = (DepNode *) graph.items_of_Coll[i];

	 if (dp->setno >= 0)
	    continue;

	 while (depth_search(&graph, dp, &setno))
	    ;
      }
   }

   rp = RETPTR(ClipMachineMemory);

   CLEAR_CLIPVAR(rp);
   rp->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   rp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_MPTR_ClipFlags;
   rp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar = rap = (ClipVar *) calloc(sizeof(ClipVar), 1);
   rap->ClipType_t_of_ClipVar.ClipVartype_type_of_ClipType = ARRAY_type_of_ClipVarType;
   rap->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType = F_NONE_ClipFlags;
   rap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar = 0;
   rap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar = 0;
   rap->ClipType_t_of_ClipVar.count_of_ClipType = 1;

   for (i = 0; i < setno; i++)
   {
      int j;

		for (j = 0; j < graph.count_of_Coll; j++)
      {
			DepNode *dp = (DepNode *) graph.items_of_Coll[j];

	 if (dp->setno == i)
	 {
	    ClipVar *vpp;

	    rap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar =
	       (ClipVar *) realloc(rap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar, (rap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar + 1) * sizeof(ClipVar));

	    vpp = rap->ClipArrVar_a_of_ClipVar.ClipVar_items_of_ClipArrVar + rap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar;
	    (rap->ClipArrVar_a_of_ClipVar.count_of_ClipArrVar)++;

	    CLEAR_CLIPVAR(vpp);
	    _clip_mclone(ClipMachineMemory, vpp, dp->vp);
	 }
      }
   }

#if 0
   /* rest */
   for (i = 0; i < graph.count; i++)
   {
      DepNode *dp = (DepNode *) graph.items[i];

      if (dp->setno < 0)
      {
	 ClipVar *vpp;

	 rap->a.items = (ClipVar *) realloc(rap->a.items, (rap->a.count + 1) * sizeof(ClipVar));

	 vpp = rap->a.items + rap->a.count;
	 (rap->a.count)++;

	 CLEAR_CLIPVAR(vpp);
	 _clip_mclone(ClipMachineMemory, vpp, dp->vp);
      }
   }
#endif

   destroy_Coll(&graph);

   return 0;
}
