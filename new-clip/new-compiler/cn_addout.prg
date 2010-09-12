Procedure cn_AddOut
	Select OutPut
	Append Blank
	Field->Command := Temp->Command
	Field->FileName := Temp->FileName
	Field->LineNum := Temp->LineNum
	Select Temp

