static int
clip_count(ClipVar * vp)
{
   int count;

   if (vp && vp->ClipType_t_of_ClipVar.ClipFlags_flags_of_ClipType & F_MPTR_ClipFlags)
   {
      count = vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar->ClipType_t_of_ClipVar.count_of_ClipType + clip_count(vp->ClipRefVar_p_of_ClipVar.ClipVar_of_ClipRefVar);
   }
   else
      count = 0;
   return count;
}
