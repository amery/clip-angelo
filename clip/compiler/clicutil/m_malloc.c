void *
m_malloc(size_t size)
{
   m_sum += size;
   m_num++;
   return malloc(size);
}
