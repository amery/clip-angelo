/***********************************************/
rational *
rational_div(rational * data, rational * y)
{
   rational *ret = rational_copy(data);

   rational_diva(ret, y);
   return ret;
}
