TermcapBucket *
tgetentry(const char *name)
{
   return (TermcapBucket *) bsearch(name, termcapNames, TCAPNUM, sizeof(TermcapBucket), cmp_bucket);
}
