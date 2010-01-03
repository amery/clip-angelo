int
getopt_long(argc, argv, options, long_options, opt_index)
	int       argc;

	char     *const *argv;

	const char *options;

	const struct option *long_options;

	int      *opt_index;
{
   return _getopt_internal(argc, argv, options, long_options, opt_index, 0);
}
