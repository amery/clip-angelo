static int
pass_PcountNode(void *self, Pass pass, int level, void *par)
{
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "PcountNode\n");
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
	  fprintfOffs(out, level, "_clip_pcount( _mp );\n");
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  putByte_StrBuf(out, CLIP_PCOUNT);
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
