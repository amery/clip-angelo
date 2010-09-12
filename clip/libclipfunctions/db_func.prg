/*
	Copyright (C) 2001  ITK
	Author   : Uri (uri@itk.ru)
	License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include <ci_config.ch>
#include <ci_set.ch>
#include <ci_fileio.ch>
#include <ci_common.ch>
#include <ci_error.ch>
#include <ci_dbinfo.ch>
#include <ci_error.ch>
**********************************************************************
#include <db_func/static___dbiInfo.prg>
#include <db_func/static__m6_copyrec.prg>
**********************************************************************
#include <db_func/__dbApp.prg>
#include <db_func/__dbAppDelim.prg>
#include <db_func/__dbAppSDF.prg>
#include <db_func/__dbCopy.prg>
#include <db_func/__dbCopyDelim.prg>
#include <db_func/__dbCopyOptimize.prg>
#include <db_func/__dbCopySDF.prg>
#include <db_func/__dbCopyStruct.prg>
#include <db_func/__dbCopyXStruct.prg>
#include <db_func/__dbCreate.prg>
#include <db_func/__dbDelim.prg>
#include <db_func/__dbDelimOptimize.prg>
#include <db_func/__DbList.prg>
#include <db_func/__DbListOptimize.prg>
#include <db_func/__dbSDF.prg>
#include <db_func/__dbSDFOptimize.prg>
#include <db_func/__dbSort.prg>
#include <db_func/__dbSortOptimize.prg>
#include <db_func/__dbTotal.prg>
#include <db_func/__dbTotalOptimize.prg>
#include <db_func/__DBUPDATE.prg>
#include <db_func/__LabelFormOptimize.prg>
#include <db_func/__ReportFormOptimize.prg>
#include <db_func/aliasesInfo.prg>
#include <db_func/dbCheckStructure.prg>
#include <db_func/dbCreate.prg>
#include <db_func/dbStructInfo.prg>
#include <db_func/fieldblock.prg>
#include <db_func/fieldwblock.prg>
#include <db_func/indexesInfo.prg>
#include <db_func/m6_Copy.prg>
#include <db_func/m6_CopyDelim.prg>
#include <db_func/m6_CopySDF.prg>
#include <db_func/m6_CopyToArray.prg>
#include <db_func/m6_CountFor.prg>
#include <db_func/m6_LabelForm.prg>
#include <db_func/m6_ListFor.prg>
#include <db_func/m6_ReportForm.prg>
#include <db_func/m6_Search.prg>
#include <db_func/m6_SortFor.prg>
#include <db_func/m6_TotalFor.prg>
#include <db_func/recordInfo.prg>
#include <db_func/tmpalias.prg>
#include <db_func/waitRddLock.prg>
**********************************************************************

