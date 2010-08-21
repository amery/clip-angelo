/**********************************************/
int
integer_cmp(integer * d1, integer * d2)
{
	if (d1->sign_of_integer != d2->sign_of_integer)
   {
		if (d1->sign_of_integer)
	 return 1;
      else
	 return -1;
   }
   return integer_abscmp(d1, d2);
}
