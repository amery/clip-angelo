static function exit_eval(p,a)
local r
do case
  case a[p][1]==x_while
    r:=a[p][2][2]+1
  case a[p][1]==x_for
    r:=a[p][2][4]+1
end case
return r
