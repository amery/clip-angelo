static int
pass_LoopExitNode(void *self, Pass pass, int level, void *par)
{
   VAR(LoopExitNode, np, self);
   switch (pass)
    {
    case Print:
       fprintfOffs(stdout, level, "%s\n", np->isLoop ? "LOOP" : "EXIT");
       break;
    case PrintSrc:
       {
	  yyerror("%s not allowed in CODESTR", np->node.name);
       }
       return 0;
    case CText:
       {
	  VAR(FILE, out, par);
	  if (np->isLoop)
	     fprintfOffs(out, level, "goto _loop_%d;\n", loopNo);
	  else
	     fprintfOffs(out, level, "goto _endloop_%d;\n", loopNo);
       }
       return 0;
    case OText:
       {
	  VAR(StrBuf, out, par);
	  putByte_StrBuf(out, CLIP_GOTO);
	  np->offs = out->ptr_of_StrBuf - out->buf_of_StrBuf;
	  putShort_StrBuf(out, 0);
       }
       return 0;
    case OTextLoop:
       {
	  VAR(LoopData, ld, par);
	  if (!np->processed)
	   {
	      if (np->isLoop)
		 SETSHORT(ld->out, np->offs, ld->boffs - (np->offs + sizeof(short)));

	      else
		 SETSHORT(ld->out, np->offs, ld->eoffs - (np->offs + sizeof(short)));

	      np->processed++;
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
