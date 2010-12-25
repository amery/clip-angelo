/*********************************************/
rational *
rational_powa(rational * data, long y)
{
   if (y >= 0)
   {
      integer_powa(data->num, y);
      integer_powa(data->den, y);
   }
   return data;
}
