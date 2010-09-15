syntax case match
setlocal iskeyword+=$
syntax keyword mrefalKeyword $MODULE $ENTRY $END $IMPORT $SWAP $PROGRAM $SWAP $DATA
syntax match mrefalBrokenKeyword /\$\a*\>/
syntax match mrefalVariable /\<[ste]\.[A-Za-z0-9!?_-]\+\>/
syntax match mrefalIdentifier /\<[A-Z!?][A-Za-z0-9!?_-]*\>/
syntax match mrefalNumber /\<\d*\>/
syntax match mrefalBrokenString /'[^']*$/
syntax match mrefalString /'\(\(\\.\)\|[^']\)*'/
syntax region mrefalComment start=/\/\*/ end=/\*\//
syntax region mrefalComment start=/\/\// end=/$/

set matchpairs=(:),[:],<:>,{:}

highlight link mrefalKeyword Operator
highlight link mrefalIdentifier Normal
highlight link mrefalVariable Special
highlight link mrefalNumber Number
highlight link mrefalString String
highlight link mrefalComment Comment
highlight link mrefalBrokenKeyword Error
highlight link mrefalBrokenString Error

set autoindent
