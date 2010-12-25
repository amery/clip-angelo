int
rdd_setrelation(ClipMachine * ClipMachineMemory, RDD_DATA * rd, RDD_DATA * child, ClipVar * block, const char *expr, int scoped, const char *__PROC__)
{
   RDD_RELATION *rel = (RDD_RELATION *) calloc(1, sizeof(RDD_RELATION));

   int r = _clip_parni(ClipMachineMemory, 1);

   int er;

   rel->scoped = scoped;
   rel->child = child;
   rel->parent = rd;
   if (!expr)
      expr = "";
   rel->expr = (char *) malloc(strlen(expr) + 1);
   strcpy(rel->expr, expr);
   if (block)
   {
      _clip_clone(ClipMachineMemory, &rel->block, block);
   }
   else
   {
      if ((rel->simpfno = _rdd_fieldno(rd, _clip_casehashword(expr, strlen(expr)))) != -1)
	 rel->simpexpr = 1;
      if (!rel->simpexpr)
      {
	 char expexp[PATH_MAX];

	 memset(expexp, 0, sizeof(expexp));
	 rdd_expandmacro(rd->area, r, expr, expexp);
	 if ((er = _clip_eval_macro(ClipMachineMemory, expexp, strlen(expexp), &rel->block)))
	    goto err;
      }
   }
   rd->rels_opened++;
   rd->relations = (RDD_RELATION **) realloc(rd->relations, rd->rels_opened * sizeof(RDD_RELATION *));
   rd->relations[rd->rels_opened - 1] = rel;

   child->pars_opened++;
   child->parents = (RDD_RELATION **) realloc(child->parents, child->pars_opened * sizeof(RDD_RELATION *));
   child->parents[child->pars_opened - 1] = rel;
   if ((er = rdd_childs(ClipMachineMemory, rd, __PROC__)))
      return er;
   return 0;
 err:
   free(rel->expr);
   free(rel);
   return er;
}
