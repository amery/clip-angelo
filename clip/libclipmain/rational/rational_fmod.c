/***********************************************/
rational *
rational_fmod(rational * data, rational * y)
{
   integer *itmp;

   rational *tmp;

   rational *ret = rational_copy(data);

   integer_mula(ret->num, y->den);
   integer_mula(ret->den, y->num);
   rational_normalize(ret);
   itmp = rational_toInteger(ret);
   tmp = rational_integer_init(itmp);
   integer_destroy(itmp);
   rational_mula(tmp, y);
   rational_destroy(ret);
   ret = rational_copy(data);
   rational_suba(ret, tmp);
   rational_destroy(tmp);
   return ret;
}
