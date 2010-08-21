function HK_get(name)

   name=upper(name)

   if name $ key_maps
      name=hashstr(name)
      return key_maps[name]
   endif

return map()
