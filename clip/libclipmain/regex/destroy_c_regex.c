void
destroy_c_regex(void *item)
{
   regex_t *preg = (regex_t *) item;

   regfree(preg);
   free(preg);
}
