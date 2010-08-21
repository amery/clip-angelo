static int
cmp_Locale(void *p1, void *p2)
{
   return strcmp(((Locale *) p1)->module, ((Locale *) p2)->module);
}
