if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
map! <D-v> *
noremap  h
noremap <NL> j
noremap  k
noremap  l
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(netrw#GX(),netrw#CheckIfRemote(netrw#GX()))
noremap <C-L> l
noremap <C-H> h
noremap <C-K> k
noremap <C-J> j
map <F9> :mak
map <F8> :cnextzz
map <F7> :cprevzz
map <F5> :e %:p:s,.h$,.X123X,:s,.c$,.h,:s,.X123X$,.c,
map <F4> :e %:p:s,.h$,.X123X,:s,.cpp$,.h,:s,.X123X$,.cpp,
vmap <BS> "-d
vmap <D-x> "*d
vmap <D-c> "*y
vmap <D-v> "-d"*P
nmap <D-v> "*P
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set background=dark
set backspace=indent,eol,start
set expandtab
set exrc
set fileencodings=ucs-bom,utf-8,default,latin1
set grepprg=ag
set helplang=en
set history=100
set incsearch
set modelines=0
set ruler
set secure
set shiftwidth=4
set showmatch
set softtabstop=4
set tabstop=4
set window=0
" vim: set ft=vim :
