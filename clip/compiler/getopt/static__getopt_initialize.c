/* Initialize the internal data when the first call is made.  */

static const char *
_getopt_initialize(const char *optstring)
{
   /* Start processing options with ARGV-element 1 (since ARGV-element 0
      is the program name); the sequence of previously skipped
      non-option ARGV-elements is empty.  */

   first_nonopt = last_nonopt = optind = 1;

   nextchar = NULL;

   /* Determine how to handle the ordering of options and nonoptions.  */

   if (optstring[0] == '-')
   {
      ordering = RETURN_IN_ORDER;
      ++optstring;
   }
   else if (optstring[0] == '+')
   {
      ordering = REQUIRE_ORDER;
      ++optstring;
   }
   else if (getenv("POSIXLY_CORRECT") != NULL)
      ordering = REQUIRE_ORDER;
   else
      ordering = PERMUTE;

   return optstring;
}
