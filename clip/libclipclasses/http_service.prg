/*
	HTTP_Service - class for create HTTP Servers and Clients.

     Copyright (C) 2004  ITK
     Author  : Uri (uri@itk.ru)
     Licence : (GPL) http://www.itk.ru/clipper/licence.html
*/
**********************************************************************
#include <ci_http.ch>
#include <ci_tcp.ch>
**********************************************************************
#define HTTP_NREAD 10000
**********************************************************************
#include <http_service/static_HTTP_checkVhost.prg>
#include <http_service/static_HTTP_close.prg>
#include <http_service/static_HTTP_connect.prg>
#include <http_service/static_HTTP_defSets.prg>
#include <http_service/static_HTTP_ErrorBlock.prg>
#include <http_service/static_HTTP_fromIni.prg>
#include <http_service/static_HTTP_parseHeader.prg>
#include <http_service/static_HTTP_receive.prg>
#include <http_service/static_HTTP_receiveLine.prg>
#include <http_service/static_HTTP_runCommand.prg>
#include <http_service/static_HTTP_runGet.prg>
#include <http_service/static_HTTP_runModClip.prg>
#include <http_service/static_HTTP_runSSI.prg>
#include <http_service/static_HTTP_send.prg>
#include <http_service/static_http_sendAnswer.prg>
#include <http_service/static_HTTP_sendCgi.prg>
#include <http_service/static_HTTP_sendDir.prg>
#include <http_service/static_HTTP_sendError.prg>
#include <http_service/static_HTTP_sendFile.prg>
#include <http_service/static_HTTP_sendLine.prg>
#include <http_service/static_HTTP_VMsetValues.prg>
#include <http_service/static_HTTP_waitRequest.prg>
**********************************************************************
#include <http_service/_recover_httpservice.prg>
#include <http_service/httpServiceNew.prg>
**********************************************************************
