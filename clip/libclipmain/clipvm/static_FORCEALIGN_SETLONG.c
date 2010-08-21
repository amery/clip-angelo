static void
SETLONG(void *ptr, long l)
{
   memcpy(ptr, &l, sizeof(l));
}
