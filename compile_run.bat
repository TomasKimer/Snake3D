@echo off
REM  Project:     Snake 3D - Advanced Assembly Languages project, FIT BUT 2009
REM  Author:      Tomas Kimer <tomas.kimer@gmail.com> <tomaskimer.com>
REM  Date:        2009/12/18
REM  Description: Build & run.

@set path=.\3rd\nasm\bin;.\3rd\alink;.\3rd\GoRC

if not exist Snake3D.obj goto j1
del Snake3D.obj

:j1
if not exist Snake3D.exe goto j2
del Snake3D.exe

:j2
if not exist Snake3D.res goto j3
del Snake3D.res

:j3
if not exist Snake3D.asm goto input_err

echo Compiling resources "Snake3D.rc" with GoRC..
GoRC Snake3D.rc
if not exist Snake3D.res goto resource_err

echo Compiling source "Snake3D.asm" with NASMgl..
nasmgl -fobj Snake3D.asm
if not exist Snake3D.obj goto compile_err

echo Linking with ALINK..
alink -oPE Snake3D.obj Snake3D.res
if not exist Snake3D.exe goto link_err

del Snake3D.obj
del Snake3D.res

echo Run Snake3D.exe..
Snake3D.EXE
echo Exit Snake3D.exe.
goto quit

:input_err
	echo File Snake3D.asm not found.
	goto exit
:no_compiler
	echo Cannot locate NASM.
	goto exit
:no_linker
	echo Cannot locate ALINK.
	goto exit
:resource_err
	echo Failed to compile with GoRC.
	goto exit
:compile_err
	echo Failed to compile with NASMgl.
	del Snake3D.res
	goto exit
:link_err
	echo Failed to link with ALINK.
	del Snake3D.obj
	del Snake3D.res
:exit
	pause
:quit