static int
cmp_DepNode(void *p1, void *p2)
{
   DepNode *np1 = (DepNode *) p1;

   DepNode *np2 = (DepNode *) p2;

   if (np1->hash < np2->hash)
      return -1;
   else if (np1->hash > np2->hash)
      return 1;
   else
      return 0;
}
