static int
pass_LangNode(void *self, Pass pass, int level, void *par)
{
   VAR(LangNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "LangNode: '%s'\n", np->ctext);
      break;
   case CText:
      {
	 VAR(FILE, out, par);
	 char *s;

	 fprintfOffs(out, level, "");
	 for (s = np->ctext; *s; ++s)
	 {
	    fputc(*s, out);
	    if (*s == '\n' && s[1])
	       fprintfOffs(out, level, "");
	 }
      }
      break;
   case OText:
      {
	 yyerror("CTEXT in pcode module");
	 break;
      }
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
