/*
	HTTP_Service - class for create HTTP Servers and Clients.

     Copyright (C) 2004  ITK
     Author  : Uri (uri@itk.ru)
     Licence : (GPL) http://www.itk.ru/clipper/licence.html
*/
**********************************************************************
#include "ci_http.ch"
#include "ci_tcp.ch"
**********************************************************************
#define HTTP_NREAD 10000
**********************************************************************
#include "http_service/static_HTTP_checkVhost.ch"
#include "http_service/static_HTTP_close.ch"
#include "http_service/static_HTTP_connect.ch"
#include "http_service/static_HTTP_defSets.ch"
#include "http_service/static_HTTP_ErrorBlock.ch"
#include "http_service/static_HTTP_fromIni.ch"
#include "http_service/static_HTTP_parseHeader.ch"
#include "http_service/static_HTTP_receive.ch"
#include "http_service/static_HTTP_receiveLine.ch"
#include "http_service/static_HTTP_runCommand.ch"
#include "http_service/static_HTTP_runGet.ch"
#include "http_service/static_HTTP_runModClip.ch"
#include "http_service/static_HTTP_runSSI.ch"
#include "http_service/static_HTTP_send.ch"
#include "http_service/static_http_sendAnswer.ch"
#include "http_service/static_HTTP_sendCgi.ch"
#include "http_service/static_HTTP_sendDir.ch"
#include "http_service/static_HTTP_sendError.ch"
#include "http_service/static_HTTP_sendFile.ch"
#include "http_service/static_HTTP_sendLine.ch"
#include "http_service/static_HTTP_VMsetValues.ch"
#include "http_service/static_HTTP_waitRequest.ch"
**********************************************************************
#include "http_service/_recover_httpservice.ch"
#include "http_service/httpServiceNew.ch"
**********************************************************************
