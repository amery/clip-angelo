int
compareConstNode(void *n1, void *n2)
{
   VAR(ConstNode, np1, n1);
   VAR(ConstNode, np2, n2);
   if (np1->type != np2->type)
      return np1->type < np2->type ? -1 : 1;
   switch (np1->type)
    {
    case CONST_NUMBER:
       {
	  double    d1 = strtod(np1->val, NULL);

	  double    d2 = strtod(np2->val, NULL);

	  if (d1 < d2)
	     return -1;
	  else if (d1 > d2)
	     return 1;
	  else
	     return 0;
       }
    case CONST_STRING:
       return strcmp(np1->val, np2->val);
    }
   return 0;
}
