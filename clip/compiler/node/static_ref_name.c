static const char *
ref_name(CallNode * np)
{
#if 1
   return "_localvars";
#else
   if (!np || !np->cfunc || !np->cfunc->reflocals)
      return "0";
   else
      return "_reflocals";
#endif
}
