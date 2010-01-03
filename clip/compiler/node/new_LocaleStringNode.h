Node     *
new_LocaleStringNode(char *val)
{
   Node     *sp;

   int       lm, l;

   char     *s;

   NEWVAR(ExprNode, ret);

   l = strlen(val) + 1;
   lm = strlen(CLIP_MODULE) + 1;
   s = (char *) malloc(l + lm);
   memcpy(s, CLIP_MODULE, lm);
   memcpy(s + lm, val, l);

   free(val);
   s[lm - 1] = 1;
   sp = new_StringConstNode(s);

   init_Node(ret, pass_LocaleStringNode, "locale");

   ret->expr = sp;
   ret->node.isExpr = 1;

   return (Node *) ret;
}
