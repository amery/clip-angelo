int
cmp_PoEntry(void *p1, void *p2)
{
   PoEntry *ep1 = (PoEntry *) p1;

   PoEntry *ep2 = (PoEntry *) p2;

   return strcmp(ep1->msg, ep2->msg);
}
