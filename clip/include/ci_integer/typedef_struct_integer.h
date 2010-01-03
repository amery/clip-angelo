typedef struct integer
{
   int       sign;		/* sign, nonzero is negative */
   int       len;		/* number of values in array */
   INTEGER_VECTOR vec;		/* pointer to array of values */
}
integer;
