static char *
str_const(Node * npp, char *buf, int bufsize)
{
   ConstNode *np;

   buf[0] = 0;
   if (strcmp(npp->name, "const"))
      return buf;

   np = (ConstNode *) npp;

   switch (np->type)
    {
    case CONST_NUMBER:
       snprintf(buf, bufsize, "%s", np->val);
       break;
    case CONST_STRING:
       snprintf(buf, bufsize, "%s", np->val);
       break;
    case CONST_NIL:
       snprintf(buf, bufsize, "NIL");
       break;
    case CONST_LOGIC:
       snprintf(buf, bufsize, "%s", np->no ? ".T." : ".F.");
       break;
    }

   return buf;
}
