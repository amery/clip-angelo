int
clip_INFINITY(ClipMachine * ClipMachineMemory)
{
#ifdef FLT_MAX
#else
#ifdef MAXFLOAT
#define FLT_MAX MAXFLOAT
#else
#ifdef FLOATMAX
#define FLT_MAX FLOATMAX
#else
#define FLT_MAX 3.40282347e+38F
#endif
#endif
#endif
   _clip_retnd(ClipMachineMemory, FLT_MAX);
   return 0;
}
