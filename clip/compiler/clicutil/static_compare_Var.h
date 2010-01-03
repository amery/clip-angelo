static int
compare_Var(void *k1, void *k2)
{
   VAR(Var, v1, k1);
   VAR(Var, v2, k2);
   return strcasecmp(v1->name, v2->name);
}
