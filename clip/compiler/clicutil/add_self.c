/* add __SELF__ to parameters */
void
add_self(void)
{
   VarColl *params;

   Var *vp;

   int i;

   if (curFunction->haveSelf_of_Function)
      return;

   curFunction->haveSelf_of_Function++;
   params = curFunction->VarColl_params_of_Function;
   vp = new_Var(strdup("__SELF__"));

   insert_Coll(&params->coll, vp);
   prepend_Coll(&params->unsorted, vp);
   add_VarColl(curFunction->VarColl_locals_of_Function, vp);

	for (i = 0; i < params->unsorted.count_of_Coll; ++i)
   {
		vp = (Var *) params->unsorted.items_of_Coll[i];
      vp->pno = i;
   }
}
