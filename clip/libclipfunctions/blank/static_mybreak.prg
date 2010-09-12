static function myBreak(_error,flagm)
if _error:gencode==5
  return(0)
endif
if flagm==NIL
messagep("�� ��������� � �������, � ������� ���� ������. ��������� ��������;"+;
	 " ���-�� �� ��. �������������� �����, ����� ��� ��������� ��� �������.;"+;
	 " ������ : "+_error:description+" "+iif(empty(_error:operation),"",_error:operation)+";";
	 +iif(!empty(_error:filename),_error:filename,"") )
endif
Break(_error)
return
