/*********************************************/
void
rational_normalize(rational * data)
{
   integer *g;

   int s = integer_sign(data->den);

   if (s == 0)
   {
      integer_clear(data->num);
      integer_destroy(data->den);
      data->den = integer_long_init(1L);
      //integer_clear(data->den);
      return;
   }
   if (integer_empty(data->num))
   {
      integer_destroy(data->den);
      data->den = integer_long_init(1L);
      //integer_clear(data->den);
      return;
   }
   if (s < 0)
   {
      integer_inverse(data->num);
      integer_inverse(data->den);
   }
   g = integer_gcd(data->num, data->den);
   integer_diva(data->num, g);
   integer_diva(data->den, g);
   integer_destroy(g);
   return;
}
