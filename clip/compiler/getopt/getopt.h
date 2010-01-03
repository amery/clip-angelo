int
getopt(int argc, char *const *argv, const char *optstring)
{
   return _getopt_internal(argc, argv, optstring, (const struct option *) 0, (int *) 0, 0);
}
