
" editorconfig/local.vimrc
" Copyright © 2023-2024 Saul D. Beniquez  @{
"
" Redistribution and use in source and binary forms, with or without
" modification, are permitted provided that the following conditions are met:
"
" 1. Redistributions of source code must retain the above copyright notice,
"    this list of conditions and the following disclaimer.
"
" 2. Redistributions in binary form must reproduce the above copyright notice,
"    this list of conditions and the following disclaimer in the documentation
"    and/or other materials provided with the distribution.
"
" THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS”
" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
" IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
" ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
" LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
" CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
" SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
" INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
" CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
" ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
" POSSIBILITY OF SUCH DAMAGE. @}



let s:build_dir = 'debug'
let s:build_cores = 6
let s:make_args =  '-C '. s:build_dir . ' -j ' . s:build_cores . ' all'

let s:cmake_path = substitute(system('which ' . 'cmake'),'\n$', '', '')
let s:ninja_path = substitute(system('which ' . 'ninja'),'\n$', '', '')
let s:make_prg = substitute(system('which ' . 'make'),'\n$', '','')

let s:cmake_generator = 'Unix Makefiles'
if (s:ninja_path != '')
	let s:make_prg = s:ninja_path
	let s:cmake_generator = 'Ninja'
endif

if (s:cmake_path != '')
	let s:cmake_call = s:cmake_path .
				\' -B ' . s:build_dir .
				\' -D ' . 'CMAKE_BUILD_TYPE=Debug' .
				\' -G ' . s:cmake_generator
endif

let s:make_prg = "nice -20 "  . s:make_prg
let s:make_call =   s:make_prg  . " " .  s:make_args

let &makeprg = s:make_prg

if ! get(s:, 'defined', 0) " -- prevents the function from being redefined after compiling
function! BuildDebug()
	wall
	if (s:ninja_path != '')
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/build.ninja'))
		\ )


			exec ':Dispatch ' . s:cmake_call . ' && ' . s:make_call
		endif

		exec ':Make ' . s:make_args
	else
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/Makefile'))
		\ )
			exec ':Dispatch ' . s:cmake_call
		endif

		if exists(':Make')
			exec ':Make ' . s:make_args
			cd ..
		else
			exec 'make ' . s:make_args
			vert botright copen
			vert resize +100
		endif
	endif
	" Check if .compile_commands.json exists in the current working directory
	if !filereadable('.compile_commands.json')
  	  " File does not exist, create a symbolic link
  	  execute 'silent !ln -s ' . $PWD . '/.vim/lspbuild/compile_commands ' . $PWD
	endif
endfunction

function! RunTests()
	if (s:ninja_path != '')
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/build.ninja'))
		\ )


			exec ':Dispatch ' . s:cmake_call
		endif

		set makeprg='ninja'
		exec ':Make ' . s:make_args. ' ctest'
	else
		if (
		\ (!filereadable(s:build_dir . '/CMakeCache.txt')) ||
		\ (!filereadable(s:build_dir . '/Makefile'))
		\ )
			exec ':Dispatch ' . s:cmake_call
		endif

		if exists(':Make')
			exec ':Make ' . s:make_args . ' ctest'
			cd ..
		else
			exec 'make ' . s:make_args . ' ctest'
			vert botright copen
			vert resize +100
		endif
	endif

endfunction
let s:defined = 1
endif

nnoremap <leader>bd :call BuildDebug()<CR>
nnoremap <leader>rt :call RunTests()<CR>

set path+=src
set path+=include
set path+=app

autocmd! BufWritePre *.c,*.h,*.cpp,*.hpp LspDocumentFormat

" vim: set ts=4 sts=4 noet sw=4 foldmethod=marker foldmarker=@{,@} :

