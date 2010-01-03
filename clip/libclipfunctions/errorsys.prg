/*
	Copyright (C) 2001  ITK
	Author   : Uri (uri@itk.ru)
	License : (GPL) http://www.itk.ru/clipper/license.html
*/
**********************************************************************
#include "ci_error.ch"
**********************************************************************
#define NTRIM(n)		( LTrim(Str(n)) )
**********************************************************************
#include "errorsys/static_DefError.ch"
**********************************************************************
**********************************************************************
#include "errorsys/error2html.ch"
#include "errorsys/error2Log.ch"
#include "errorsys/ErrorMessage.ch"
#include "errorsys/ErrorSys.ch"
#include "errorsys/procStackDepth.ch"
**********************************************************************
