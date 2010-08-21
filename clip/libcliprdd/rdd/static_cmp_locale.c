static int
cmp_locale(void *p1, void *p2)
{
   return strcmp(((LocData *) p1)->name, ((LocData *) p2)->name);
}
