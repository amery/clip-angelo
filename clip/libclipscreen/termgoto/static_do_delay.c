/*
   // Routine to perform cursor addressing.
   // CM is a string containing printf type escapes to allow
   // cursor addressing.  We start out ready to print the destination
   // line, and switch each time we print row or column.
   // The following escapes are defined for substituting row/column:
   //
   //      %d      as in printf
   //      %2      like %2d
   //      %3      like %3d
   //      %.      gives %c hacking special case characters
   //      %+x     like %c but adding x first
   //
   //      The codes below affect the state but don't use up a value.
   //
   //      %>xy    if value > x add y
   //      %r      reverses row/column
   //      %i      increments row/column (for one origin indexing)
   //      %%      gives %
   //      %B      BCD (2 decimal digits encoded in one byte)
   //      %D      Delta Data (backwards bcd)
   //
   // all other characters are ``self-inserting''.
 */

static void
do_delay(int ms)
{
   if (ms)
      usleep(ms * 1000);
}
