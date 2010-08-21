static int
compare_Function(void *k1, void *k2)
{
   VAR(Function, v1, k1);
   VAR(Function, v2, k2);
   long h1 = hashstr(v1->name_of_Function + (v1->isPublic_of_Function == 0 ? 3 : 0));

   long h2 = hashstr(v2->name_of_Function + (v2->isPublic_of_Function == 0 ? 3 : 0));

   /*return strcasecmp(v1->name_of_Function, v2->name_of_Function); */
   if (h1 < h2)
      return -1;
   else if (h1 > h2)
      return 1;
/*
else if (!v1->isPublic_of_Function && v2->isPublic_of_Function)
   return -1;
else if (v1->isPublic_of_Function && !v2->isPublic_of_Function)
   return 1;
 */
   else
      return 0;
}
