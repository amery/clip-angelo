static int
pass_NewArrayNode(void *self, Pass pass, int level, void *par)
{
   VAR(NewArrayNode, np, self);
   int i;

   switch (pass)
   {
   case Print:
		fprintfOffs(stdout, level, "NewArrayNode (%d):\n", np->cp->count_of_Coll);
		for (i = 0; i < np->cp->count_of_Coll; ++i)
      {
			VAR(char, s, np->cp->items_of_Coll[i]);

	 fprintfOffs(stdout, level + 1, "%ldL\n", atol(s));
      }
      break;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 1);
      }
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 int i;

	 fprintfOffs(out, level, "_clip_newarray( _mp, %d ", np->cp->count_of_Coll);
	 for (i = 0; i < np->cp->count_of_Coll; ++i)
	 {
		 VAR(char, s, np->cp->items_of_Coll[i]);

	    fprintf(out, ", %ldL", atol(s));
	 }
	 fprintf(out, " );\n");
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 int i;

	 putByte_StrBuf(out, CLIP_NEWARRAY);
	 putLong_StrBuf(out, np->cp->count_of_Coll);
	 for (i = 0; i < np->cp->count_of_Coll; ++i)
	 {
		 VAR(char, s, np->cp->items_of_Coll[i]);

	    putLong_StrBuf(out, atol(s));
	 }
      }
      return 0;
   case Traverse:
      {
	 VAR(TraversePar, tp, par);
	 tp->func((Node *) self, tp->par);
      }
      break;
   default:
      break;
   }
   return 0;
}
