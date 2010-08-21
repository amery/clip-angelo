/*
	following the language settings will read the appropriate
	language support.
*/
#include <dv_english.ch>
#ifdef CH_BRAZIL
	#include <dv_brazil.ch>
#elifdef CH_ITALIAN
	#include <dv_italian.ch>
#elifdef CH_GERMAN
	#include <dv_german.ch>
#elifdef CH_DUTCH
	#include <dv_dutch.ch>
#elifdef CH_RUSSIAN
	#include <dv_russian.ch>
#elifdef CH_CHINESE_TW
	#include <dv_chinese_tw.ch>
#elifdef CH_CHINESE_CN
	#include <dv_chinese_cn.ch>
#elifdef CH_PORTUGESE
	#include <dv_portugese.ch>
#endif
