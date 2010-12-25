/**********************************************/
rational *
rational_double_init(double x)
{
   int sgn = x < 0;

   int expt, k;

   long exponent;

   double mantissa;

   double intpart;

   integer *z;

   rational *ret = (rational *) calloc(sizeof(rational), 1);

   ret->num = integer_long_init(0L);
   ret->den = integer_long_init(1L);
   if (x == 0.0)
      return ret;
   if (sgn)
      x = -x;
   mantissa = frexp(x, &expt);
   exponent = expt;
   for (k = 0; k < 20 && mantissa != 0.0; k++)
   {
      mantissa *= 32768.0;
      mantissa = modf(mantissa, &intpart);
      integer_lshifta(ret->num, 15);
      z = integer_long_init((long) intpart);
      integer_adda(ret->num, z);
      integer_destroy(z);
      exponent -= 15;
   }
   if (exponent > 0)
      integer_lshifta(ret->num, exponent);
   if (exponent < 0)
   {
      integer_lshifta(ret->den, -exponent);
   }
   if (sgn)
      integer_inverse(ret->num);
   rational_normalize(ret);
   return ret;
}
