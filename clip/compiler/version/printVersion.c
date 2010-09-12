void
printVersion(FILE * file)
{
   fprintf(file, "%s.%s\n", CLIP_VERSION, SEQ_NO);
   fprintf(file, "\n%s\n", CLIP_LICENSE);
}
