char *
m_strdup(const char *s)
{
   int l = strlen(s) + 1;

   void *ret = m_malloc(l);

   memcpy(ret, s, l);
   s_sum += l;
   s_num++;
   return ret;
}
