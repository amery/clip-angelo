function RDDNew(Driver,f_name,Shared,ReadOnly)
  local RDDobj

  RDDobj:=map()
// ��������
  RDDobj:classname:="RDD"
  RDDobj:handle:=NIL
  RDDobj:table_driver:="DBF"		// ��� �������� ������ �� ���������
  RDDobj:index_driver:="NTX"		// ��� �������� �������� �� ���������
  RDDobj:memo_driver:="DBT"		// ��� �������� ����-������ �� ���������
  RDDobj:share_mode:=RDD_SHARED		// ����� �������� ������ �� ���������
  RDDobj:read_only:=RDD_READ_WRITE	// ����� �������� ������ �� ���������
  RDDobj:soft_seek:=RDD_SEEK_SOFT	// ����� ���������� ������ �� ���������
  RDDobj:search_result:=.f.    		// ����� �������� ��������� ���������� ������

  _recover_RDD(RDDObj)

  if f_name<>NIL
    RDDobj:open(Driver,f_name,Shared,ReadOnly)
  endif
return RDDobj
