/* Like getopt_long, but '-' as well as '--' can indicate a long option.
   If an option that starts with '-' (not '--') doesn't match a long option,
   but does match a short option, it is parsed as a short option
   instead.  */

int
getopt_long_only(argc, argv, options, long_options, opt_index)
   int argc;

   char *const *argv;

   const char *options;

   const struct option *long_options;

   int *opt_index;
{
   return _getopt_internal(argc, argv, options, long_options, opt_index, 1);
}
