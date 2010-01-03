/*
	following the language settings will read the appropriate
	language support.
*/
#include <cn_english.ch>
#ifdef CH_BRAZIL
	#include <cn_brazil.ch>
#elifdef CH_ITALIAN
	#include <cn_italian.ch>
#elifdef CH_GERMAN
	#include <cn_german.ch>
#elifdef CH_DUTCH
	#include <cn_dutch.ch>
#elifdef CH_RUSSIAN
	#include <cn_russian.ch>
#elifdef CH_CHINESE_TW
	#include <cn_chinese_tw.ch>
#elifdef CH_CHINESE_CN
	#include <cn_chinese_cn.ch>
#elifdef CH_PORTUGESE
	#include <cn_portugese.ch>
#endif
