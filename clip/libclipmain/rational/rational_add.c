/***********************************************/
rational *
rational_add(rational * data, rational * y)
{
   rational *ret = rational_copy(data);

   rational_adda(ret, y);
   return ret;
}
