Coll *
new_Coll(void (*Free) (void *), int (*Compare) ())
{
   Coll *ret = (Coll *) malloc(sizeof(Coll));

   init_Coll(ret, Free, Compare);
   return ret;
}
