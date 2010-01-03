

#ifndef CH_ENGLISH

#DEFINE CH_ENGLISH
#ifdef _LINUX_
	#define DIR_SEPARATOR 	"/"
#else
	#define DIR_SEPARATOR 	"\"
#endif
#define ERROR_FILE 						"file not found"
#define ERROR_INCLUDE_STRING 			"unknown separator"
#define ERROR_INCLUDE_END 				"missing end separator"
#define ERROR_DEFINE 					"already define : "
#define ERROR_COMMENT 					"'/*' without '*/'"
#define ERROR_SOURCE 					"no source filename"
#DEFINE PARAM_ERROR_AUTO_MEMVAR 		"--auto-memvar parameter error value"
#DEFINE PARAM_ERROR_D 					"-D parameter error value"
#DEFINE PARAM_ERROR_DEBUG 				"--debug parameter error value"
#DEFINE PARAM_ERROR_DEFINE 			"--define parameter error value"
#DEFINE PARAM_ERROR_E 					"-E parameter error value"
#DEFINE PARAM_ERROR_ENVIRONMENT 		"--environment parameter error value"
#DEFINE PARAM_ERROR_OUTDIR 			"--outdir parameter error value"
#define PARAM_ERROR_INCLUDE_PATH 	"--include-path parameter error value"
#define PARAM_ERROR_LIBRARY_PATH 	"--library-path parameter error value"
#define PARAM_ERROR_OUTPUT 			"--output parameter error value"
#define PARAM_ERROR_OPTIMISE 			"--optimise parameter error value"
#define PARAM_ERROR_Q 					"-q parameter error value"
#define PARAM_ERROR_COMPAT 			"--compat parameter error value"
#define PARAM_ERROR_SOURCE_CHARSET 	"--source-charset parameter error value"
#define PARAM_ERROR_USE_STD_CH 		"--use-std-ch parameter error value"
#define PARAM_ERROR_V 					"-v parameter error value"
#define PARAM_ERROR_VERBOSE 			"--verbose parameter error value"
#define PARAM_ERROR_NAMESPACE 		"--namespace-warning parameter error value"
#define PARAM_ERROR_TARGET_CHARSET 	"--target-charset parameter error value"
#define PARAM_ERROR_INVALID 			" invalid parameter error value"
#define PRINT_FILENAME 					"filename :"
#define PRINT_COMMAND_LINE 			"Command line : "
#define PRINT_LINE 						"line :"
#define PRINT_WITHOUT 					"without :"
#endif
