#ifdef CH_BRAZIL
#undef CH_BRAZIL

#undef ERROR_FILE
#define ERROR_FILE 						"file not found"
#undef ERROR_INCLUDE_END
#define ERROR_INCLUDE_END 				"missing end separator"
#undef ERROR_INCLUDE_STRING
#define ERROR_INCLUDE_STRING 			"unknown separator"
#undef ERROR_DEFINE
#define ERROR_DEFINE 					"already define : "
#undef ERROR_COMMENT
#define ERROR_COMMENT 					"'/*' without '*/'"
#undef ERROR_SOURCE
#define ERROR_SOURCE 					"no source filename"
#undef DEFINE PARAM_ERROR_AUTO_MEMVAR
#DEFINE PARAM_ERROR_AUTO_MEMVAR 		"--auto-memvar parameter error value"
#undef PARAM_ERROR_D
#DEFINE PARAM_ERROR_D 					"-D parameter error value"
#undef PARAM_ERROR_DEBUG
#DEFINE PARAM_ERROR_DEBUG 				"--debug parameter error value"
#undef PARAM_ERROR_DEFINE
#DEFINE PARAM_ERROR_DEFINE 			"--define parameter error value"
#undef PARAM_ERROR_E
#DEFINE PARAM_ERROR_E 					"-E parameter error value"
#undef PARAM_ERROR_ENVIRONMENT
#DEFINE PARAM_ERROR_ENVIRONMENT 		"--environment parameter error value"
#undef PARAM_ERROR_OUTDIR
#DEFINE PARAM_ERROR_OUTDIR 			"--outdir parameter error value"
#undef PARAM_ERROR_INCLUDE_PATH
#define PARAM_ERROR_INCLUDE_PATH 	"--include-path parameter error value"
#undef PARAM_ERROR_LIBRARY_PATH
#define PARAM_ERROR_LIBRARY_PATH 	"--library-path parameter error value"
#undef PARAM_ERROR_OUTPUT
#define PARAM_ERROR_OUTPUT 			"--output parameter error value"
#undef PARAM_ERROR_OPTIMISE
#define PARAM_ERROR_OPTIMISE 			"--optimise parameter error value"
#undef PARAM_ERROR_Q
#define PARAM_ERROR_Q 					"-q parameter error value"
#undef PARAM_ERROR_COMPAT
#define PARAM_ERROR_COMPAT 			"--compat parameter error value"
#undef PARAM_ERROR_SOURCE_CHARSET
#define PARAM_ERROR_SOURCE_CHARSET 	"--source-charset parameter error value"
#undef PARAM_ERROR_USE_STD_CH
#define PARAM_ERROR_USE_STD_CH 		"--use-std-ch parameter error value"
#undef PARAM_ERROR_V
#define PARAM_ERROR_V 					"-v parameter error value"
#undef PARAM_ERROR_VERBOSE
#define PARAM_ERROR_VERBOSE 			"--verbose parameter error value"
#undef PARAM_ERROR_NAMESPACE
#define PARAM_ERROR_NAMESPACE 		"--namespace-warning parameter error value"
#undef PARAM_ERROR_TARGET_CHARSET
#define PARAM_ERROR_TARGET_CHARSET 	"--target-charset parameter error value"
#undef PARAM_ERROR_INVALID
#define PARAM_ERROR_INVALID 			" invalid parameter error value"
#undef PRINT_FILENAME
#define PRINT_FILENAME 					"filename :"
#undef PRINT_COMMAND_LINE
#define PRINT_COMMAND_LINE 			"Command line : "
#undef PRINT_LINE
#define PRINT_LINE 						"line :"
#undef PRINT_WITHOUT
#define PRINT_WITHOUT 					"without :"
#endif
