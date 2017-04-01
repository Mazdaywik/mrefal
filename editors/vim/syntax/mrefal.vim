syntax case match
syntax keyword mrefalKeyword 
      \ $PROGRAM $MODULE $IMPORT $DATA $ENTRY $SWAP $INIT $FINAL $END
syntax match mrefalBrokenKeyword /\$\a*\>/
syntax match mrefalVariable /\<[ste]\.[A-Za-z0-9!?_-]\+\>/
syntax match mrefalIdentifier /\<[A-Z!?][A-Za-z0-9!?_-]*\>/
syntax match mrefalNumber /\<\d*\>/
syntax match mrefalBrokenString /'[^']*$/
syntax match mrefalString /'\(\(\\.\)\|[^']\)*'/
syntax region mrefalString start=/\z(@\+\)'/ skip=/\n/ end=/'\z1/ contains=NONE
syntax region mrefalComment start=/\/\*/ skip=/\n/ end=/\*\//
syntax region mrefalComment start=/\/\// end=/$/
syntax region mrefalComment start=/^\*/ end=/$/

highlight link mrefalKeyword Keyword
highlight link mrefalIdentifier Normal
highlight link mrefalVariable Special
highlight link mrefalNumber Number
highlight link mrefalString String
highlight link mrefalComment Comment
highlight link mrefalBrokenKeyword Error
highlight link mrefalBrokenString Error

setlocal iskeyword+=$,-
setlocal matchpairs=(:),[:],<:>,{:}
setlocal autoindent
