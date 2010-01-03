static void
exchange(argv)
	char    **argv;
{
   int       bottom = first_nonopt;

   int       middle = last_nonopt;

   int       top = optind;

   char     *tem;

  /* Exchange the shorter segment with the far end of the longer segment.
     That puts the shorter segment into the right place.
     It leaves the longer segment in the right place overall,
     but it consists of two parts that need to be swapped next.  */

   while (top > middle && middle > bottom)
    {
       if (top - middle > middle - bottom)
	{
	  /* Bottom segment is the short one.  */
	   int       len = middle - bottom;

	   register int i;

	  /* Swap it with the top part of the top segment.  */
	   for (i = 0; i < len; i++)
	    {
	       tem = argv[bottom + i];
	       argv[bottom + i] = argv[top - (middle - bottom) + i];
	       argv[top - (middle - bottom) + i] = tem;
	    }
	  /* Exclude the moved bottom segment from further swapping.  */
	   top -= len;
	}
       else
	{
	  /* Top segment is the short one.  */
	   int       len = top - middle;

	   register int i;

	  /* Swap it with the bottom part of the bottom segment.  */
	   for (i = 0; i < len; i++)
	    {
	       tem = argv[bottom + i];
	       argv[bottom + i] = argv[middle + i];
	       argv[middle + i] = tem;
	    }
	  /* Exclude the moved top segment from further swapping.  */
	   bottom += len;
	}
    }

  /* Update records for the slots the non-options now occupy.  */

   first_nonopt += (optind - last_nonopt);
   last_nonopt = optind;
}
