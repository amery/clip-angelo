static void
astrcat(char **d, int *l, char *s)
{
   int ls = strlen(s);

   *d = realloc(*d, *l + ls + 1);
   strcpy(*d + *l, s);
   *l += ls;
}
