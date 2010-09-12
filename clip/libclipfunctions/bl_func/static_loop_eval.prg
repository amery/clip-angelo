static function loop_eval(p,pc)
memvar a
local r
do case
  case a[p][1]==x_while
    r:=p
  case a[p][1]==x_for
    r:=a[p][2][4]
end case
return r
