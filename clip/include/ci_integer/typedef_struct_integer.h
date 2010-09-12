typedef struct integer
{
	int sign_of_integer;			/* sign, nonzero is negative */
   int len_of_integer;			/* number of values in array */
   INTEGER_VECTOR vec_of_integer;		/* pointer to array of values */
}
integer;
