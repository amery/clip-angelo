static int
pass_PshiftNode(void *self, Pass pass, int level, void *par)
{
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "PshiftNode\n");
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
	 fprintfOffs(out, level, "_clip_pshift( _mp );\n");
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 putByte_StrBuf(out, CLIP_PSHIFT);
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
