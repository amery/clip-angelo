
m->msg:='Hello, World!'
m->key='asdf'

? evp_alg_list()
?

m->msg1:=evp_encrypt(m->msg, m->key)
m->msg2:=evp_decryptm->(m->msg1, m->key)

? 'source   : "', m->msg, '"'
? 'encrypted: "', m->msg1, '"'
? 'decrypted: "', m->msg2, '"'
?
