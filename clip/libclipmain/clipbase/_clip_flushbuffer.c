int
_clip_flushbuffer(ClipMachine * ClipMachineMemory, DBWorkArea * wa, const char *__PROC__)
{
   return rdd_flushbuffer(ClipMachineMemory, wa->rd, __PROC__);
}
