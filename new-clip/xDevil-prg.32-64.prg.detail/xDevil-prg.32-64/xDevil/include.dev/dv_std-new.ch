#ifndef DV_STD_DEFINED
#define DV_STD_DEFINED

#include "dv_config.ch"

#ifdef CLIPPER50_COMPATIBLE
	#include "dv_std50.ch"
#else
    #ifdef FOXPRO_COMPATIBLE
	#include "dv_std53.ch"
        #include "dv_fox.ch"
        #include "dv_foxsql.ch"
    #else
	#include "dv_std53.ch"
   #endif
#endif

#include "dv_clip.ch"
#include "dv_func_ref.ch"
//#include "dv_common.ch"

#endif

