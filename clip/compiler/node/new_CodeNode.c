Node *
new_CodeNode(VarColl * params)
{
   char buf[32], *s;

   int i;

   NEWVAR(CodeNode, ret);

   init_Node(ret, pass_CodeNode, "code");
	sprintf(buf, "code_%d", curFile->Coll_codeblocks_of_File.count_of_Coll);
   ret->fp = curFunction;
   curFunction = ret->func = new_Function(strdup(buf), params, 0, 0, curFunction, 1);
   for (s = ret->func->name_of_Function; *s; ++s)
      *s = tolower(*s);
   insert_Coll(&curFile->Coll_codeblocks_of_File, ret->func);
   init_Coll(&ret->inits, 0, 0);
   ret->node.isCode = 1;

   if (params)
		for (i = 0; i < params->unsorted.count_of_Coll; i++)
      {
			VAR(Var, vpp, params->unsorted.items_of_Coll[i]);
	 vpp->isCodeParam = 1;
	 vpp->isParam = 1;
      }

   return (Node *) ret;
}
