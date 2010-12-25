/**********************************************/
char *
rational_toString2(rational * data)
{
   char *s1, *s2, *buf;

   int lenstr, len1, len2;

   s1 = integer_toString(data->num, 10);
   s2 = integer_toString(data->den, 10);
   len1 = strlen(s1);
   len2 = strlen(s2);
   lenstr = len1 + len2 + 1;
   buf = calloc(lenstr + 1, 1);
   memcpy(buf, s1, len1);
   buf[len1] = '/';
   memcpy(buf + len1 + 1, s2, len2);
   free(s1);
   free(s2);
   return buf;
}
