STATIC FUNCTION Skipper(n, browse)
    LOCAL i
    curr_record:=max(1,curr_record)
    curr_record:=min(len(__a2_data[1]),curr_record)
    i=curr_record
    curr_record+=n
    curr_record:=max(1,curr_record)
    curr_record:=min(len(__a2_data[1]),curr_record)
RETURN curr_record-i
