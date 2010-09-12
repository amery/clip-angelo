void
delete_Var(void *item)
{
   VAR(Var, vp, item);
   if (!item)
      return;
   if (vp->arr)
      delete_Coll(vp->arr);
   if (vp->alias)
      free(vp->alias);
   free(vp->name);
   free(item);
}
