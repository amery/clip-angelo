void
add_VarColl(VarColl * coll, Var * var)
{
   int no;

   if (!var)
      return;
   if (search_Coll(&coll->coll, var, &no))
   {
		VAR(Var, vp, coll->coll.items_of_Coll[no]);
      yywarning("duplicate name '%s', previous name defined near line %d pos %d in file '%s'", vp->name, vp->line, vp->pos, fileName(vp->file));
#if 0
      remove_Coll(&coll->coll, vp);
      remove_Coll(&coll->unsorted, vp);
      delete_Var(vp);
#endif
   }

   var->no = coll->coll.count_of_Coll;
   insert_Coll(&coll->coll, var);
   insert_Coll(&coll->unsorted, var);
}
