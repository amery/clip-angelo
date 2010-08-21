void
rdd_expandmacro(int area, int r, const char *key, char *expr)
{
   if (area != -1)
   {
      sprintf(expr, "{|| %s}", key);
      return;
   }
   sprintf(expr, "{|| __rdd__ %d %s __rdd__}", r, key);
   return;
}
