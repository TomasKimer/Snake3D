;************************************
; Written by                    made in  Visualwin32asm                      
;************************************
;#######   TITLE OF THE PROJECT    #######
;*************************************
;gorc file.rc
;nasmw -fobj file.asm
;alink -oPE file.obj win32.lib file.res
;*************************************
;win32 functions used in this aplication 
;*************************************
extern SetMenu
extern LoadMenuA
extern GetModuleHandleA
extern ExitProcess
extern DialogBoxParamA
extern MessageBoxA
extern EndDialog
extern SendDlgItemMessageA

extern GetOpenFileNamePreview 
import GetOpenFileNamePreview MSVFW32.DLL

;********************************************************************
; win32n.inc include
;********************************************************************

%include '..\..\inc\win32n.inc'

;*********************************************************************
; Macro it´s better this way... call function ,x1,..,xn
;********************************************************************

%macro call 2-*
	%define _j %1
	%rep %0-1
		%rotate -1
		push dword %1
	%endrep
	call _j
%endmacro

;********************************************************************

[SECTION DATA CLASS=DATA USE32]

opfNameYouWant: 
ISTRUC OPENFILENAME 
	AT OPENFILENAME.lStructSize,		DD	OPENFILENAME_size 
	AT OPENFILENAME.hWndOwner,			DD	0 
	AT OPENFILENAME.hInstance,			DD	0 
	AT OPENFILENAME.lpstrFilter,		DD	FiltroFile         ;filter of ...
	AT OPENFILENAME.lpstrCustomFilter,	DD	0 
	AT OPENFILENAME.nMaxCustFilter,		DD	0 
	AT OPENFILENAME.nFilterIndex,		DD	0 
	AT OPENFILENAME.lpstrFile,			DD	BufferForFile    ;integer .... 
	AT OPENFILENAME.nMaxFile,			DD	nmaxfile 
	AT OPENFILENAME.lpstrFileTitle,		DD	0 
	AT OPENFILENAME.nMaxFileTitle,		DD	0 
	AT OPENFILENAME.lpstrInitialDir,	DD	0 
	AT OPENFILENAME.lpstrTitle,			DD	Titulo             ; constant.... 
	AT OPENFILENAME.Flags,				DD	OFN_LONGNAMES | OFN_EXPLORER 
	AT OPENFILENAME.nFileOffset,		DW	0 
	AT OPENFILENAME.nFileExtension, 	DW	0 
	AT OPENFILENAME.lpstrDefExt,		DD	0 
	AT OPENFILENAME.lCustData,			DD	0 
	AT OPENFILENAME.lpfnHook,			DD	0 
	AT OPENFILENAME.lpTemplateName, 	DD	0 
IEND 

BufferForFile							RESD 260
FiltroFile                      		DB	'TXT',0,'*.txt',0,0  ;cxhange to your needs...
nmaxfile                        		EQU	256
Titulo 
Instance								dd	0
hInst									dd	0

[SECTION CODE USE32 CLASS=CODE]

 ..start:
 
	enter	0,0
	call	GetModuleHandleA,NULL
	mov		[hInst],eax
	call	DialogBoxParamA,[hInst],1000,0,@Dialog1Proc,0	;defwinproc is the boss...
	call	ExitProcess,[hInst]


;*************************************************************************
;  DialogWndProc	proc	hwnd:DWORD, wmsg:DWORD, wparam:DWORD, lparam:DWORD  
;                       ebp+8        ebp+12     ebp+16         ebp+20    
;  it´s allways like this           
;*************************************************************************

@Dialog1Proc:   ;change to your name to this Dialog... or keep if you like the name.. :)

%define hdlg1		ebp+8	;hwnd ... like this name better :)
%define dlg1msg		ebp+12	;wmsg ... like this name better :)
%define wparam		ebp+16
%define lparam		ebp+20

	enter	0,0
	cmp		dword [dlg1msg],WM_INITDIALOG
	je		wminitdialog
	cmp		dword [dlg1msg],WM_COMMAND
	je		near wmcommand
	;cmp	dword [dlg1msg],WM_PAINT
	;je		near Paint
	cmp		dword [dlg1msg],WM_CLOSE
	je		near wmclose
	jmp		unhandled


;--------------
unhandled:

	mov		eax,0			;return false see win32api.hlp
	leave
	ret		16

;-------------- 
wminitdialog:

;->set the menu
	push	byte 103		; {lpszMenuName}          ;Name or resource ID
	push	dword [hInst]	; {hInstance}             ;hInstance with the resource
	call	LoadMenuA
	PUSH	dword eax		;identifies handle of the menu 
	PUSH	dword [hdlg1]	; handle of dialog
	CALL	SetMenu

;your code here... if needed... ;)

	mov		eax,1 
	leave
	ret		16

;--------------
wmcommand:
	;cmp	dword [wparam],   ;number of ID resouce   
	;je		near New1

;change the above to feet your code...

	mov		eax,1
	leave
	ret		16

;-------------
New1:
	jmp		unhandled
;-------------
New2:
	jmp		unhandled
;-------------
wmclose:

	call	ExitProcess,0            

	ret16

;------------