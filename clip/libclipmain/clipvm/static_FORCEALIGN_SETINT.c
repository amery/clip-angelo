static void
SETINT(void *ptr, int l)
{
   memcpy(ptr, &l, sizeof(l));
}
