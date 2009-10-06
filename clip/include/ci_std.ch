#ifndef _STD_DEFINED
#define _STD_DEFINED

#include "ci_config.ch"

#ifdef CLIPPER50_COMPATIBLE
	#include "ci_std50.ch"
#else
	#ifdef FOXPRO_COMPATIBLE
        	// ���� ������� fox.ch
		#include "ci_std53.ch"
                #include "ci_fox.ch"
                #include "ci_foxsql.ch"
        #else
		#include "ci_std53.ch"
        #endif
#endif

#include "ci_clip.ch"
#include "ci_func_ref.ch"
//#include "common.ch"

#endif

