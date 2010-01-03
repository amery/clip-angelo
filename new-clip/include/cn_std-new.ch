#ifndef CN_STD_DEFINED
#define CN_STD_DEFINED

#include "cn_config.ch"

#ifdef CLIPPER50_COMPATIBLE
	#include "cn_std50.ch"
#else
    #ifdef FOXPRO_COMPATIBLE
	#include "cn_std53.ch"
        #include "cn_fox.ch"
        #include "cn_foxsql.ch"
    #else
	#include "cn_std53.ch"
   #endif
#endif

#include "cn_clip.ch"
#include "cn_func_ref.ch"
//#include "cn_common.ch"

#endif

