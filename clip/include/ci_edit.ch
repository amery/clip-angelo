#include "ci_inkey.ch"
#include "ci_config.ch"
#include "ci_set.ch"

#ifndef CN_TE_DEFINED
#define CN_TE_DEFINED

#define TE_SCORE_ROW		0
#define TE_SCORE_COL		60

#define TE_MAXSTRINGS		16000
#define TE_MAX_LEN_STRING	2048

#define TE_TABSIZE		8
#define TE_MARGIN_LEFT     	4
#define TE_MARGIN_RIGHT    	80
#define TE_AUTO_SAVE		.f.
#define TE_TIME_AUTO_SAVE	10
#define TE_CREATE_BAK		.t.
#define TE_SAVE_STATUS		.t.
#define TE_STEP_UNDO		100
#define TE_HISTORY_SIZE		25
#define TE_HYPHEN 		.f.
#define TE_AUTO_MARGIN		.f.
#define TE_AUTO_WRAP		.f.
#define TE_AUTO_INDENT		.t.


#define TE_EXIT		-1
#define TE_CONTINUE	0
#define TE_EXCEPTION	1


#define ME_EXIT			-1
#define ME_CONTINUE		0
#define ME_EXCEPTION		1



#ifdef LANG_RISSIAN
   #define TE_LANGUAGE		"ru"
#else
   #define TE_LANGUAGE		"en"
#endif

#endif

