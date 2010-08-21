static void
write_statics(FILE * out, VarColl * statics)
{
   int j;

	for (j = 0; j < statics->unsorted.count_of_Coll; j++)
   {
		VAR(Var, vp, statics->unsorted.items_of_Coll[j]);
#if 1
      fprintf(out, "static ClipVar s_%s_%s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} }", vp->func->name_of_Function, vp->name);
#else
      if (!vp->init)
	 fprintf(out, "static ClipVar s_%s_%s = { {UNDEF_type_of_ClipVarType, 0, 0,0,0} }", vp->func->name_of_Function, vp->name);
      else if (vp->init->isArray)
      {
	 fprintf(out, "static ClipVar s_%s_%s", vp->func->name_of_Function, vp->name);
      }
      else
      {
	 if (vp->init->isNum)
	    fprintf(out, "static ClipVarNum s_%s_%s = ", vp->func->name_of_Function, vp->name);
	 else if (vp->init->isStr)
	    fprintf(out, "static ClipVarStr s_%s_%s = ", vp->func->name_of_Function, vp->name);
	 else if (vp->init->isLog)
	    fprintf(out, "static ClipVarLog s_%s_%s = ", vp->func->name_of_Function, vp->name);
	 else if (vp->init->isCode)
	    fprintf(out, "static ClipVarCode s_%s_%s = ", vp->func->name_of_Function, vp->name);
	 else
	    fprintf(out, "static ClipVar s_%s_%s = ", vp->func->name_of_Function, vp->name);
	 vp->init->pass(vp->init, CTextInit, 0, out);
      }
#endif
      fprintf(out, ";\n");
   }

}
