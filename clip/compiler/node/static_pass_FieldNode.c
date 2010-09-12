static int
pass_FieldNode(void *self, Pass pass, int level, void *par)
{
   VAR(FieldNode, np, self);
   switch (pass)
   {
   case Print:
      fprintfOffs(stdout, level, "Field node:\n");
      if (np->area)
	 fprintfOffs(stdout, level + 1, "area: %s\n", np->area);
      if (np->name)
	 fprintfOffs(stdout, level + 1, "name: %s\n", np->name);
      if (np->areaExpr)
      {
	 fprintfOffs(stdout, level + 1, "areaExpr:\n");
	 np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
      }
      if (np->nameExpr)
      {
	 fprintfOffs(stdout, level + 1, "nameExpr:\n");
	 np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
      }
      return 0;
   case PrintSrc:
      {
	 VAR(CodestrNode, cp, par);
	 VAR(StrBuf, out, cp->buf);
	 if (np->area)
	 {
	    putString_StrBuf(out, np->area);
	    putString_StrBuf(out, "->");
	 }
	 else if (np->areaExpr)
	 {
	    if (np->areaExpr->isMacro)
	    {
	       np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	       putString_StrBuf(out, "->");
	    }
	    else
	    {
	       putString_StrBuf(out, "(");
	       np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	       putString_StrBuf(out, ")->");
	    }
	 }
	 else
	 {
	    putString_StrBuf(out, "FIELD->");
	 }

	 if (np->name)
	 {
	    putString_StrBuf(out, np->name);
	 }
	 else if (np->nameExpr)
	 {
	    if (!np->nameExpr->isMacro)
	       putString_StrBuf(out, "(");
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    if (!np->nameExpr->isMacro)
	       putString_StrBuf(out, ")");
	 }
      }
      return 0;
   case CalcDeep:
      {
	 VAR(Function, fp, par);
	 addDeep(fp, 3);
	 if (np->areaExpr)
	    np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
	 if (np->nameExpr)
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
      }
      return 0;
   case CText:
      {
	 VAR(FILE, out, par);
	 if (np->name && !np->area && !np->areaExpr)
	    fprintfOffs(out, level, "if ( (_ret=_clip_field( _mp, %ld /* %s */, -1 ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	 else if (np->name && np->area)
	    fprintfOffs(out, level,
			"if ( (_ret=_clip_field( _mp, %ld /* %s */, %ld /* %s */ ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, (long) n_hashstr(np->area), np->area, np->node.seqNo);
	 else if (np->name && np->areaExpr)
	 {
	    if (np->areaExpr->isMacro)
	       np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
	    else
	       np->areaExpr->pass(np->areaExpr, pass, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_field( _mp, %ld /* %s */, _clip_pop_hash(_mp) ))) goto _trap_%d;\n", (long) hashstr(np->name), np->name, np->node.seqNo);
	 }
	 else if (np->area && np->nameExpr)
	 {
	    fprintfOffs(out, level, "if ( (_ret=_clip_push_area( _mp, %ld /* %s */ ))) goto _trap_%d;\n", (long) n_hashstr(np->area), np->area, np->node.seqNo);
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_pop_area( _mp ) )) goto _trap_%d;\n", np->node.seqNo);
	 }
	 else if (np->areaExpr && np->nameExpr)
	 {
	    if (np->areaExpr->isMacro)
	       np->areaExpr->pass(np->areaExpr, CTextLval, level, par);
	    else
	       np->areaExpr->pass(np->areaExpr, pass, level, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_push_area( _mp, _clip_pop_hash(_mp) ) )) goto _trap_%d;\n", np->node.seqNo);
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    fprintfOffs(out, level, "if ( (_ret=_clip_pop_area( _mp ) )) goto _trap_%d;\n", np->node.seqNo);
	 }
	 else if (np->nameExpr)
	 {
	    if (np->nameExpr->isMacro)
	       np->nameExpr->pass(np->nameExpr, CTextLval, level + 1, par);
	    else
	       np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    fprintfOffs(out, level, "{ long _a, _f; _clip_pop_fieldhash(_mp, &_a, &_f); if ( (_ret=_clip_field( _mp, _f, _a )) ) goto _trap_%d; }\n", np->node.seqNo);
	 }
      }
      return 0;
   case OText:
      {
	 VAR(StrBuf, out, par);
	 if (np->name && !np->area && !np->areaExpr)
	 {
	    putByte_StrBuf(out, CLIP_FIELD);
	    putLong_StrBuf(out, hashstr(np->name));
	    putLong_StrBuf(out, -1);
	 }
	 else if (np->name && np->area)
	 {
	    putByte_StrBuf(out, CLIP_FIELD);
	    putLong_StrBuf(out, hashstr(np->name));
	    putLong_StrBuf(out, n_hashstr(np->area));
	 }
	 else if (np->name && np->areaExpr)
	 {
	    if (np->areaExpr->isMacro)
	       np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
	    else
	       np->areaExpr->pass(np->areaExpr, pass, level, par);
	    putByte_StrBuf(out, CLIP_FIELD_POP);
	    putLong_StrBuf(out, hashstr(np->name));
	 }
	 else if (np->area && np->nameExpr)
	 {
	    putByte_StrBuf(out, CLIP_PUSH_AREA);
	    putLong_StrBuf(out, n_hashstr(np->area));
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    putByte_StrBuf(out, CLIP_POP_AREA);
	 }
	 else if (np->areaExpr && np->nameExpr)
	 {
	    if (np->areaExpr->isMacro)
	       np->areaExpr->pass(np->areaExpr, OTextLval, level, par);
	    else
	       np->areaExpr->pass(np->areaExpr, pass, level, par);
	    putByte_StrBuf(out, CLIP_PUSH_AREA_POP);
	    np->nameExpr->pass(np->nameExpr, pass, level + 1, par);
	    putByte_StrBuf(out, CLIP_POP_AREA);
	 }
	 else if (np->nameExpr)
	 {
	    if (np->nameExpr->isMacro)
	       np->nameExpr->pass(np->nameExpr, OTextLval, level, par);
	    else
	       np->nameExpr->pass(np->nameExpr, pass, level, par);
	    putByte_StrBuf(out, CLIP_FIELD_POP_NAME);
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

   if (np->areaExpr)
      np->areaExpr->pass(np->areaExpr, pass, level + 1, par);
   if (np->nameExpr)
      np->nameExpr->pass(np->nameExpr, pass, level + 1, par);

   return 0;
}
