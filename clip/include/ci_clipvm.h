/*
 	Start total new system v. 0.0
	with hard coded long name variables to have clear system
	Angelo GIRARDI

 */

#ifndef CI_CLIPVM_H
#define CI_CLIPVM_H

/* clip virtual machine codes */

#define CLIP_POP		0
#define CLIP_LINE		1
#define CLIP_PUSH_NUM		2
#define CLIP_PUSH_STR		3
#define CLIP_PUSH_NIL		4
#define CLIP_PUSH_TRUE		5
#define CLIP_PUSH_FALSE		6
#define CLIP_MEMVAR_PUBLIC	7
#define CLIP_MEMVAR_PRIVATE	8
#define CLIP_MEMVAR_PARAM	9
#define CLIP_PUSH_PARAM		10
#define CLIP_PUSH_REF_PARAM	11
#define CLIP_PUSH_LOCAL		12
#define CLIP_PUSH_REF_LOCAL	13
#define CLIP_PUSH_STATIC	14
#define CLIP_PUSH_REF_STATIC	15
#define CLIP_PUSH_MEMVAR	16
#define CLIP_PUSH_REF_MEMVAR	17
#define CLIP_PUSH_PUBLIC	18
#define CLIP_PUSH_REF_PUBLIC	19
#define CLIP_MAKE_REF		20
#define CLIP_UNREF_ARR		21
#define CLIP_FIELD		22
#define CLIP_FIELD_POP		23
#define CLIP_PUSH_AREA		24
#define CLIP_PUSH_AREA_POP	25
#define CLIP_POP_AREA		26
#define CLIP_FUNC		27
#define CLIP_PROC		28
#define CLIP_ASSIGN		29
#define CLIP_IASSIGN		30
#define CLIP_ASSIGN_FIELD_POP	31
#define CLIP_IASSIGN_FIELD_POP	32
#define CLIP_ASSIGN_FIELD	33
#define CLIP_IASSIGN_FIELD	34
#define CLIP_ARRAY		35
#define CLIP_NEWARRAY		36
#define CLIP_GET		37
#define CLIP_SET		38
#define CLIP_RETURN_POP		39
#define CLIP_RETURN		40
#define CLIP_STORE		41
#define CLIP_ISTORE		43
#define CLIP_FETCH		44
#define CLIP_IFETCH		45
#define CLIP_CALL		46
#define CLIP_PUSH_CODE		47
#define CLIP_SFUNC		48
#define CLIP_SPROC		49
#define CLIP_MACRO		50
#define CLIP_PCOUNT		51
#define CLIP_PSHIFT		52
#define CLIP_PARN		53
#define CLIP_FUNCR		54
#define CLIP_PROCR		55
#define CLIP_SFUNCR		56
#define CLIP_SPROCR		57
#define CLIP_FUNC_NAME		58
#define CLIP_INCR		59
#define CLIP_DECR		60
#define CLIP_INCR_POST		61
#define CLIP_DECR_POST		62
#define CLIP_OPASSIGN		63
#define CLIP_OPIASSIGN		64
#define CLIP_OP			65
#define CLIP_NOT		66
#define CLIP_OPSTORE		67
#define CLIP_OPISTORE		68
#define CLIP_COND		69
#define CLIP_GOTO		70
#define CLIP_MINUS		71
#define CLIP_SETTRAP		72
#define CLIP_RESETTRAP		73
#define CLIP_RECOVER		74
#define CLIP_USING		75
#define CLIP_BREAK		76
#define CLIP_BREAK_EXPR		77
#define CLIP_PUSH_FMEMVAR	78
#define CLIP_MEMVAR_SPACE	79

#define CLIP_ASSIGN_PFIELD_POP	80
#define CLIP_IASSIGN_PFIELD_POP	81
#define CLIP_ASSIGN_PFIELD	82
#define CLIP_IASSIGN_PFIELD	83
#define CLIP_MACROASSIGN 	84

#define CLIP_DIMARRAY		85
#define CLIP_TCOND		86
#define CLIP_ITCOND		87

#define CLIP_PUSH_RLOCAL	88
#define CLIP_PUSH_REF_RLOCAL	89

#define CLIP_SWAP		90
#define CLIP_PUSH_HASH		91

#define CLIP_FM_ASSIGN		92
#define CLIP_FM_IASSIGN		93

#define CLIP_PUSH_ULOCAL	94
#define CLIP_PUSH_REF_ULOCAL	95
#define CLIP_CALC_HASH		96

#define CLIP_MEMVAR_PUBLIC_POP	97
#define CLIP_MEMVAR_PRIVATE_POP	98
#define CLIP_MEMVAR_SPACE_POP	99
#define CLIP_FORSTEP	100

#define CLIP_PUSH_STRDUP	101
#define CLIP_PUSH_LOCALE	102

#define CLIP_RDDFIELD		103
#define CLIP_CALC_HASH2		104

#define CLIP_MAP_FIRST		105
#define CLIP_MAP_NEXT		106

#define CLIP_FIELD_POP_NAME	107
#define CLIP_FETCHREF		108
#define CLIP_REFASSIGN		109
#define CLIP_UNREF		110
#define CLIP_REFMACRO		111
#define CLIP_REFFIELD		112
#define CLIP_REF_FMEMVAR	113
#define CLIP_CATSTR		114
#define CLIP_QUOT		115
#define CLIP_SWITCH		116
#define CLIP_NOP		117
#define CLIP_PUSH_MEMVARF	118
#define CLIP_REF_DESTROY	119
#define CLIP_PUSH_REF_MEMVAR_NOADD	120

typedef struct
{
   short len;
   short offs;
}
OffsBucket;

typedef struct
{
   long hash;
   long offs;
}
HashBucket;

#define CLIPMAG "!<pobj>\n"
#define CLIPMAGLEN 8

#define ELFMAG "\177ELF"
#define ELFMAGLEN 4

#endif
