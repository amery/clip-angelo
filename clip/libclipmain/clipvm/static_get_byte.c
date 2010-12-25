static unsigned char
get_byte(void *ptr)
{
   unsigned char **pp = (unsigned char **) ptr;

   return *(*pp)++;
}
