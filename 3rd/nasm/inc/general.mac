%macro win32fn 2-3
	extern %1
	%if %0 > 2
		import %1 %2 %3
	%else
		import %1 %2
	%endif
%endmacro

%macro dllimport 2-3
	extern %1
	%if %0 > 2
		import %1 %2 %3
	%else
		import %1 %2
	%endif
%endmacro

%macro dllexport 1-2
	global %1
	%if %0 > 1
		export %1 %2
	%else
		export %1
	%endif
%endmacro


%macro function 1-*	
	; definuje funkci
	; 1. parametr = jmeno funkce
	; dalsi parametry = jmena parametru, kazdy z nich ma 4 byty = 32bitu
	
	%push function
	%define %$fname %1

    %1:
		%if %0 > 1
			%push param_function			
			%assign repcnt %0-1
			%assign disp 4
			%assign retp 0
			%rep repcnt
				%assign disp disp+4		; kazdy parametr ma 4 byty
				%assign retp retp+4
				%rotate 1
				%xdefine %1 EBP+disp			
			%endrep
			%$fname %+ _PARAMBYTES EQU retp
		%else
			%push simple_function
			%$fname %+ _PARAMBYTES EQU 0
		%endif
%endmacro

%macro var 1-*
	%if %0 > 0
		%ifctx simple_function
			%repl param_local_function
		%endif

		%ifctx param_function		
			%repl param_local_function
		%endif

		%ifctx param_local_function		
			%assign disp 4
			%assign varlen 0
			%assign varcnt 0
			%rep %0
				%xdefine %1 EBP-disp				
				%assign disp disp+4		; kazdy parametr ma 4 byty
				%assign varlen varlen+4
				%rotate 1
			%endrep
			%$fname  %+ _LOCALBYTES EQU varlen 
		%else
			%error "'var': must be immediately after 'function' before 'fnbegin'!"
		%endif
	%else
		%error "'var': missing local variables!"
	%endif
%endmacro

%macro begin 0
	%ifctx param_function
		enter 0,0
	%elifctx param_local_function
		enter %$fname %+ _LOCALBYTES,0
	%elifnctx simple_function
		%error "'fnbegin': missing 'function' definition!"
	%endif
%endmacro

%macro return 0-1	; makro return pouzije instrukci leave a vola ret
		
	%if %0 = 1
		%ifnidn %1,eax
 			%if %1 = 0
				xor eax,eax
			%else
				mov eax,%1
			%endif
		%endif
	%endif
	%ifctx param_function
		leave
		ret %$fname %+ _PARAMBYTES ; pocet bytu u ret by mel byt roven poctu parametru funkce*4
	%elifctx param_local_function
		leave
		ret %$fname %+ _PARAMBYTES ; pocet bytu u ret by mel byt roven poctu parametru funkce*4
	%elifctx simple_function
		ret
	%else
		%error "'return': must be used within 'fnbegin' and 'fnend', may be 'function' is missing?"
	%endif
	%define returnOK 1
%endmacro

%macro end 0
	%ifctx param_local_function
		%assign i 0
		%rep varcnt
			%undef param%i
			%assign i i+1
		%endrep 
	%endif
	%ifdef returnOK
		%pop
	%else
		%error "'fnend': missing 'return'?"
	%endif
	%ifctx function
		%pop
	%else
		%error "'fnend': missing 'function'?"
	%endif
%endmacro

%macro invoke 1-*
		%if %0 > 1
			%rep %0-1
				%rotate -1
				push dword %1
			%endrep
			%rotate -1
		%endif
		call [%1]
%endmacro

%macro call 1-*
		%if %0 > 1
			%rep %0-1
				%rotate -1
				push dword %1
			%endrep
			%rotate -1
		%endif
		call %1 ; Zde je zmena oproti invoke!!! Chybi [...]
%endmacro

%macro string 2+
		%1: DB %2
		endof.%1: DB 0
		lenof.%1 EQU endof.%1 - %1
		sizeof.%1 EQU endof.%1 - %1
%endmacro

;%macro sizeof 1
;	sizeof.%1 EQU endof.%1 - %1
;%endmacro