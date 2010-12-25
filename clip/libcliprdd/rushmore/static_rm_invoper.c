static int
rm_invoper(int oper)
{
   switch (oper)
   {
   case RM_LAR:
      return RM_LES;
   case RM_LARE:
      return RM_LESE;
   case RM_LES:
      return RM_LAR;
   case RM_LESE:
      return RM_LARE;
   }
   return oper;
}
