void     *
m_calloc(size_t nmemb, size_t size)
{
   int       l = nmemb * size;

   void     *ret = m_malloc(l);

   memset(ret, 0, l);
   c_sum += l;
   c_num++;
   return ret;
}
