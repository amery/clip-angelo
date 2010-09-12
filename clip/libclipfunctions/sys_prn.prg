/*
    Copyright (C) 2001  ITK
    Author   : Uri (uri@itk.ru)
    License : (GPL) http://www.itk.ru/clipper/license.html

    functions for printing
*/
**************************************************************************
#include <ci_set.ch>
#include <ci_inkey.ch>
#include <ci_fileio.ch>
**************************************************************************
static print_desc
static print_queue
**************************************************************************
**************************************************************************
#include <sys_prn/static___translatePrintFile.prg>
**************************************************************************
#include <sys_prn/loadPrinters.prg>
#include <sys_prn/printBegin.prg>
#include <sys_prn/printChoice.prg>
#include <sys_prn/printEject.prg>
#include <sys_prn/printEnd.prg>
#include <sys_prn/printerNew.prg>
#include <sys_prn/printQueueNew.prg>
**************************************************************************
