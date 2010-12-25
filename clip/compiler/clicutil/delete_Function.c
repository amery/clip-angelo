void
delete_Function(void *v)
{
   VAR(Function, f, v);
   if (!f)
      return;
   /*delete_Node((Node *) f->body); */
   delete_VarColl(f->VarColl_params_of_Function);
   delete_VarColl(f->VarColl_locals_of_Function);
   delete_VarColl(f->VarColl_publics_of_Function);
   delete_VarColl(f->VarColl_privates_of_Function);
   delete_VarColl(f->VarColl_parameters_of_Function);
   delete_VarColl(f->VarColl_statics_of_Function);
   delete_VarColl(f->VarColl_memvars_of_Function);
   delete_VarColl(f->VarColl_fmemvars_of_Function);
   delete_VarColl(f->VarColl_fields_of_Function);

   free(f->name_of_Function);
   free(f->seqStack_of_Function);
   free(f);
}
