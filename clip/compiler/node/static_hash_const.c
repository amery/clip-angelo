static clip_hash_t
hash_const(Node * npp)
{
   ConstNode *np;

   long r = 0;

   if (strcmp(npp->name, "const"))
      return 0;

   np = (ConstNode *) npp;

   switch (np->type)
   {
   case CONST_NUMBER:
      r = strtod(np->val, 0);
      break;
   case CONST_STRING:
      r = _clip_hashstr(np->val);
      break;
   case CONST_NIL:
      r = 0;
      break;
   case CONST_LOGIC:
      r = np->no ? 1 : 0;
      break;
   }

   if (npp->isMinus)
      r = 0 - r;
   return (clip_hash_t) r;
}
