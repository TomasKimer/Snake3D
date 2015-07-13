;
;  Project:     Snake 3D - Advanced Assembly Languages project, FIT BUT 2009
;  Author:      Tomas Kimer <tomas.kimer@gmail.com> <tomaskimer.com>
;  Date:        2009/12/18
;  Description: Main source. Unfortunately comments are in Czech language.
;

bits 32
;********************************************************************
; Vložené soubory
%include '.\3rd\nasm\inc\win32n.inc'
%include '.\3rd\nasm\inc\opengl.inc'
%include '.\3rd\nasm\inc\general.mac'
;********************************************************************
; Definice zdroju
%define IDI_MAINICON       101     ; large icon id
%define IDI_MAINICON_SMALL 102     ; small icon id
;********************************************************************
; Upravime makro invoke tak, aby pouzivalo misto instrukce PUSH
; direktivu %pushparam.
%macro invoke 1-*
    %if %0 > 1
        %rep %0-1
            %rotate -1            
             %pushparam dword %1
        %endrep
        %rotate -1
    %endif
    call [%1]
%endmacro
;********************************************************************
; Funkce z knihovny kernel32.dll
dllimport GetModuleHandle, kernel32.dll, GetModuleHandleA
dllimport ExitProcess, kernel32.dll 
dllimport GetTickCount, kernel32.dll  
;********************************************************************
; Funkce z knihovny user32.dll 
dllimport ShowWindow, user32.dll
dllimport UpdateWindow, user32.dll
dllimport TranslateMessage, user32.dll
dllimport RegisterClassEx, user32.dll, RegisterClassExA
dllimport LoadIcon, user32.dll, LoadIconA
dllimport CreateWindowEx, user32.dll, CreateWindowExA
dllimport PeekMessage, user32.dll, PeekMessageA
dllimport DispatchMessage, user32.dll, DispatchMessageA
dllimport PostQuitMessage, user32.dll
dllimport DefWindowProc, user32.dll, DefWindowProcA
dllimport ReleaseDC, user32.dll 
dllimport GetDC, user32.dll
;********************************************************************
; Funkce z knihovny gdi32.dll
dllimport SwapBuffers, gdi32.dll
dllimport ChoosePixelFormat, gdi32.dll
dllimport SetPixelFormat, gdi32.dll
dllimport CreateFont, gdi32.dll, CreateFontA
dllimport SelectObject, gdi32.dll
dllimport DeleteObject, gdi32.dll
dllimport AddFontResource, gdi32.dll, AddFontResourceA
dllimport RemoveFontResource, gdi32.dll, RemoveFontResourceA
;********************************************************************
; Funkce z knihovny glu32.dll a opengl32.dll
dllimport gluPerspective, glu32.dll
dllimport wglUseFontOutlines, opengl32.dll, wglUseFontOutlinesA
dllimport gluNewQuadric, glu32.dll
dllimport gluSphere, glu32.dll 
;********************************************************************
; Funkce z knihovny msvcrt.dll
dllimport sprintf, msvcrt.dll
dllimport strlen, msvcrt.dll
dllimport time, msvcrt.dll
dllimport srand, msvcrt.dll
dllimport rand, msvcrt.dll
;********************************************************************
; Datovy segment
[section .data class=DATA use32 align=16]

string szWndClassName,"Snake3D class"   ; nazev tridy okna
string szWndCaption,  "Snake 3D"        ;  -  © 2009 Tomas Kimer <tomaskimer.com>
string szFontName,    "Weltron Urban"   ; nazev pouziteho pisma
string szFontFilename,"font.ttf"        ; cesta k pismu
string szScoreFormat, "%d"              ; format zobrazeni skore
string szGameOver,    "Game Over!"      ; vypis "konec hry"
string szContinue,    "Press -Enter-"   ; vypis "zmackni enter"
string szSnake3D,     "S n a k e 3D"    ; vypis nazvu hry

hInstance       dd    0         ; handle instance
hWnd            dd    0         ; handle okna
dwWndWidth      dd    960       ; sirka okna
dwWndHeight     dd    600       ; vyska okna
hDC             dd    0         ; handle kontextu zarizeni
hRC             dd    0         ; handle kontextu renderingu

RIGHT          equ    0         ; konstanty smeru pohybu hada 
DOWN           equ    1
LEFT           equ    2
UP             equ    3

snakeLen      resd    1         ; delka hada
snakeBody     resd  200         ; souradnice tela hada (x1,y1,x2,y2,x3...)
snakeDir      resd    1         ; smer pohybu  

actDir        resd    1         ; aktualni smer
score         resd    1         ; hodnota skore
foodPos       resd    2         ; souradnice potravy (x,y)
colorInc      resd    1         ; hodnota inkrementace zelene slozky hada
gameOver        dd    0         ; zda byla ukoncena hra (=1)

levCnt          dd    0.0       ; citac levitace hraci plochy
levInc          dd    0.05      ; velikost inkrementace citace
levRes          dd    0.0       ; vystupni y-souradnice
levRng          dd   60.0       ; rozsah levitace (mensi hodnota = vetsi rozsah)
levMax          dd    6.2831853 ; max hodnota citace (2*PI)

rotCnt          dd    0.0       ; citac rotace potravy
rotInc          dd    3.0       ; velikost inkrementace citace
rotMax          dd  360.0       ; max hodnota citace (360°)

lastTime1       dd    0         ; cas minule aktualizace posunu hada
timerInterval1  dd  100         ; interval casovace 1 - posunu hada

lastTime2       dd    0         ; cas minule aktualizace levitace a rotace
timerInterval2  dd   30         ; interval casovace 2 - levitace a rotace

fontBase      resd    1         ; cislo display listu pisma 
gmf           resd 6144         ; info o fontu, 256*sizeof(GLYPHMETRICSFLOAT)
scoreBuff     resb    5         ; buffer pro retezec s hodnotou skore

quadric       resd    1         ; ukazatel na quadric objekt

spacer          dd    0.23      ; konstanta pro rozmisteni bloku na plose
planeX          dd    2.27      ; rozmery hraci plochy  
planeY          dd    0.1
planeZ          dd    2.27
bodyX           dd    0.2       ; rozmery clanku na hraci plose   
bodyY           dd    0.2
bodyZ           dd    0.2  

c_1             dd   -1.0       ; pomocne desetinne konstanty 
c01             dd    0.1
c0              dd    0.0      
c1              dd    1.0
c2              dd    2.0
c3              dd    3.0
c4              dd    4.0
c5              dd    5.0
c6              dd    6.0
c7              dd    7.0
c8              dd    8.0
c9              dd    9.0
c10             dd   10.0

Message:      resb   MSG_size

WndClass:
    istruc WNDCLASSEX
        at WNDCLASSEX.cbSize,          dd  WNDCLASSEX_size
        at WNDCLASSEX.style,           dd  CS_VREDRAW + CS_HREDRAW + CS_OWNDC
        at WNDCLASSEX.lpfnWndProc,     dd  WndProc
        at WNDCLASSEX.cbClsExtra,      dd  0
        at WNDCLASSEX.cbWndExtra,      dd  0
        at WNDCLASSEX.hInstance,       dd  NULL
        at WNDCLASSEX.hIcon,           dd  NULL
        at WNDCLASSEX.hCursor,         dd  NULL
        at WNDCLASSEX.hbrBackground,   dd  NULL
        at WNDCLASSEX.lpszMenuName,    dd  NULL
        at WNDCLASSEX.lpszClassName,   dd  szWndClassName
        at WNDCLASSEX.hIconSm,         dd  NULL
    iend

PixelFormatDescriptor:
    istruc PIXELFORMATDESCRIPTOR
        at PIXELFORMATDESCRIPTOR.nSize,             dw    PIXELFORMATDESCRIPTOR_size
        at PIXELFORMATDESCRIPTOR.nVersion,          dw    1
        at PIXELFORMATDESCRIPTOR.dwFlags,           dd    PFD_DOUBLEBUFFER + PFD_DRAW_TO_WINDOW + PFD_SUPPORT_OPENGL
        at PIXELFORMATDESCRIPTOR.iPixelType,        db    PFD_TYPE_RGBA
        at PIXELFORMATDESCRIPTOR.cColorBits,        db    24 
        at PIXELFORMATDESCRIPTOR.cRedBits,          db    0 
        at PIXELFORMATDESCRIPTOR.cRedShift,         db    0 
        at PIXELFORMATDESCRIPTOR.cGreenBits,        db    0 
        at PIXELFORMATDESCRIPTOR.cGreenShift,       db    0 
        at PIXELFORMATDESCRIPTOR.cBlueBits,         db    0 
        at PIXELFORMATDESCRIPTOR.cBlueShift,        db    0 
        at PIXELFORMATDESCRIPTOR.cAlphaBits,        db    0 
        at PIXELFORMATDESCRIPTOR.cAlphaShift,       db    0 
        at PIXELFORMATDESCRIPTOR.cAccumBits,        db    0 
        at PIXELFORMATDESCRIPTOR.cAccumRedBits,     db    0 
        at PIXELFORMATDESCRIPTOR.cAccumGreenBits,   db    0 
        at PIXELFORMATDESCRIPTOR.cAccumBlueBits,    db    0 
        at PIXELFORMATDESCRIPTOR.cAccumAlphaBits,   db    0 
        at PIXELFORMATDESCRIPTOR.cDepthBits,        db    32 
        at PIXELFORMATDESCRIPTOR.cStencilBits,      db    0 
        at PIXELFORMATDESCRIPTOR.cAuxBuffers,       db    0 
        at PIXELFORMATDESCRIPTOR.iLayerType,        db    PFD_MAIN_PLANE 
        at PIXELFORMATDESCRIPTOR.bReserved,         db    0 
        at PIXELFORMATDESCRIPTOR.dwLayerMask,       dd    0 
        at PIXELFORMATDESCRIPTOR.dwVisibleMask,     dd    0
        at PIXELFORMATDESCRIPTOR.dwDamageMask,      dd    0
    iend
;********************************************************************
[section .code use32 class=CODE]

 ..start:
    invoke GetModuleHandle,NULL             ; zjistíme handle modulu (programu)
    mov [hInstance],eax                     ; a hodnotu uložíme do [hInstance]
    mov [WndClass + WNDCLASSEX.hInstance],eax  ; a jednu kopii uložíme
                                            ; i do struktury trídy našeho okna
    invoke LoadIcon,[hInstance],IDI_MAINICON       ; nacteni velke ikony
    mov [WndClass + WNDCLASSEX.hIcon],eax
    invoke LoadIcon,[hInstance],IDI_MAINICON_SMALL ; nacteni male ikony
    mov [WndClass + WNDCLASSEX.hIconSm],eax    
    
    invoke RegisterClassEx,WndClass         ; zaregistrujeme naši trídu
    test eax,eax                            ; je-li EAX=0, pak nastala chyba
    jz near .Finish                         ; = ukoncíme program    
    
    invoke time,NULL                        ; inicializace generatoru nah.cisel
    invoke srand,eax 
                                            ; zavoláme funkci CreateWidnowEx
    invoke CreateWindowEx,\
         0,\
         szWndClassName,szWndCaption,\
         WS_CAPTION | WS_VISIBLE | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,\
         CW_USEDEFAULT, CW_USEDEFAULT, [dwWndWidth], [dwWndHeight],\
         NULL, NULL, [hInstance], NULL
    test eax,eax                       ; otestujeme úspešnost volání
    jz near .Finish                    ; pri neúspechu (EAX=0) ukoncíme program

    mov [hWnd],eax                     ; a nakonec schováme handle našeho okna
    invoke ShowWindow,eax,SW_SHOWDEFAULT    ; okno zobrazíme
    invoke UpdateWindow,[hWnd]              ; a necháme prekreslit
    
;********************************************************************
; Zde je smycka pro zpracovani zprav v hernich aplikacich.
; Pokud ji chcete pouzit, smazte klasickou smycku zprav.
.GameLoop:

    invoke PeekMessage,Message,NULL,0,0,PM_REMOVE   ; zjistíme, máme-li
    test eax,eax              ; nejakou zprávu, je-li EAX=0, pak ne => koncíme 
    jz near .NoMessage        ; skokem na místo .NoMessage
    
    cmp dword [Message + MSG.message],WM_QUIT  ; nejdríve zjistíme,je-li
    jz near .Finish                  ; prijatá zpráva WM_QUIT = konec programu,
                                     ; pokud ano ukoncíme program
    invoke TranslateMessage,Message  ; preložíme virtuální klávesy
    invoke DispatchMessage,Message   ; pošleme zprávu obslužné funkci

.NoMessage:
    call Update                      ; aktualizace
    call Render                      ; zde je místo pro vykreslení scény
    jmp .GameLoop                    ; a znovu …
;********************************************************************

.Finish
    invoke ExitProcess,[Message + MSG.wParam]    ; ukonceni procesu

;********************************************************************   
;* Procedura okna
;* @param hWnd   Handle okna.
;* @param wMsg   Zprava.
;* @param wParam 1. parametr.
;* @param lParam 2. parametr.
;********************************************************************
function WndProc,hWnd,wMsg,wParam,lParam
;********************************************************************
begin
    mov eax,dword [wMsg]    ; s hodnotou wMsg budeme casto pracovat, pro
                            ; urychlení si ji uložíme do registru EAX
    
    cmp eax,WM_KEYDOWN      ; KEYDOWN = stisknuta klavesa
    je near .Keydown
    cmp eax,WM_PAINT        ; PAINT = okno potrebuje prekreslit
    je near .Paint
    cmp eax,WM_SIZE         ; SIZE = velikost okna byla zmenena
    je near .Resize
    cmp eax,WM_CREATE       ; CREATE = bylo vytvoreno naše okno
    je near .Create   
    cmp eax,WM_CLOSE        ; totéž udeláme v prípade WM_CLOSE, tedy 
    je near .Destroy        ; zavrení okna
    cmp eax,WM_DESTROY      ; je-li zpráva WM_DESTROY, skoèíme na .Destroy
    je near .Destroy       

; nenašla se zpráva, kterou bychom umeli/chteli obsloužit, pošleme 
; ji tedy funkci DefWindowProc a její výsledek vrátíme jako výsledek naší funkce
    
    invoke DefWindowProc,[hWnd],[wMsg],[wParam],[lParam]
    return eax

.Close:
.Destroy:                            ; sem se dostaneme, chceme-li zavrít
                                     ; okno nebo je-li okno jinak zniceno
    invoke glDeleteLists,[fontBase],256 ; smaze display list pisma 
    invoke RemoveFontResource,szFontFilename ; odstrani font
    invoke wglMakeCurrent,NULL,NULL  ; nejdríve se zbavíme všeho, co se
    invoke wglDeleteContext,[hRC]    ; týká OpenGL (grafického kontextu)
    invoke ReleaseDC,[hWnd],[hDC]    ; uvolníme grafické prostredky
    invoke PostQuitMessage,0         ; pošleme príkaz k ukoncení aplikace

.Finish:
    return 0                         ; vrátíme hodnotu 0
    
.Create:
    invoke GetDC,[hWnd]              ; zjistíme kontext zarízení našeho okna
    mov [hDC],eax                    ; a uložíme ho do promìnné hDC
    invoke ChoosePixelFormat,eax,PixelFormatDescriptor      ; nastavíme formát
    invoke SetPixelFormat,[hDC],eax,PixelFormatDescriptor   ; pixelu
    invoke wglCreateContext,[hDC]    ; vytvoríme kontext OpenGL a uložíme ho
    mov [hRC],eax                    ; do promìnné hRC
    invoke wglMakeCurrent,[hDC],eax  ; vytvorený kontext aktivujeme
    call InitGL                  ; zavoláme naši funkci pro inicializaci OpenGL     
    call BuildFont               ; inicializace fontu
    invoke gluNewQuadric         ; vytvoreni Quadric objektu
    mov [quadric],eax            ; ulozeni ukazatele
    call GameInit                ; inicializace hry
    invoke GetTickCount          ; aktualni cas
    mov [lastTime1],eax          ; inicializace casovace 1
    mov [lastTime2],eax          ; inicializace casovace 2     
    jmp .Finish                  ; konec obsluhy zprávy

.Paint:
    call Render                  ; zavoláme funkci renderující scénu
    jmp .Finish                  ; konec obsluhy zprávy
    
.Keydown:
    cmp dword [wParam],VK_ESCAPE ; byla stisknuta klávesa ESC?
    jz near .Close               ; pokud ano, skocíme na .Close
    call OnKeyDown,[wParam]      ; jinak obslouzime dalsi klavesy
    jmp .Finish                  ; a ukoncíme obsluhu zprávy

.Resize:
    mov eax,[lParam]         ; lParam obsahuje novou výšku a šírku, uložíme
    shr eax,16               ; hodnotu do EAX, posuneme o 16 bitu do prava
    mov [dwWndHeight],eax    ; a vysledek (nova vyska okna) uložíme do
    push eax                 ; promenne dwWndHeight a na zasobnik (budeme
    mov eax,[lParam]         ; ji predávat funkci glViewport), opet ulozime     
    and eax,0x0000FFFF       ; hodnotu do EAX a vymaskujeme dolní cást hodnoty,    
    mov [dwWndWidth],eax     ; získáme tak sirku okna, a uložíme ji do promenné    
    push eax                 ; dwWndWidth a na zasobnik 
    call InitGL              ; nyní znovu inicializujeme OpenGL (kvuli zmene
    invoke glViewport,0,0    ; pomeru stran) a zavolame glViewport jen se dvema
    jmp .Finish              ; parametry, poslední dva uz jsou na zasobniku
    
end ;WndProc

;********************************************************************   
;* Inicializuje OpenGL.
;********************************************************************
function InitGL
;********************************************************************
begin 
    invoke glShadeModel,GL_SMOOTH      ; povoli jemne stinovani
    invoke glHint,GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST       
                                       ; nejpeknejsi perspektivni korekce
    invoke glEnable,GL_DEPTH_TEST      ; povolíme test hloubky
    invoke glEnable,GL_LIGHTING        ; povolíme osvetlení
    invoke glEnable,GL_LIGHT0          ; zapneme jedno svetlo
    invoke glEnable,GL_COLOR_MATERIAL  ; zapneme barevne materialy
    invoke glMatrixMode,GL_PROJECTION  ; nastavíme matici projekce
    invoke glLoadIdentity              ; nahrajeme identitu

    fild dword [dwWndWidth]            ; st0 = width
    fidiv dword [dwWndHeight]          ; st0 = width/height
    
    %pushparam 100.0d          ; uložíme (double)10.0 = 2x32 bitu
    %pushparam 0.1d            ; uložíme (double)1.0 = 2x32 bitu
    push dword 0               ; uložíme dve nuly na zásobník (64 bitù), címž
    push dword 0               ; rezervujeme místo na výsledek výpoctu pomeru
    fstp qword [esp]           ; stran, výsledek z st0 uložíme na adresu, danou
                               ; hodnotou v ESP (aktuální místo na zásobníku,
                               ; které jsme práve rezervovali a zavoláme funkci
                               ; gluPerspective a predáme jí poslední (první)
                               ; parametr - fovy = (double)45.0
    invoke gluPerspective,45.0d    
    return
end ; InitGL

;********************************************************************   
;* Spravuje stisknute klavesy.
;* @param kCode Kod stisknute klavesy.
;********************************************************************
function OnKeyDown,kCode
;********************************************************************
begin
    ; switch porovnani klaves
    mov eax,[kCode]
    cmp eax,VK_UP              ; klavesa nahoru
    jne @okd_B                 ; jina klavesa -> jdeme na dalsi
    cmp [actDir],dword DOWN    ; pokud je aktualni smer dolu
    je near @okd_break         ; nedelame nic
    mov [snakeDir],dword UP    ; jinak nastaveni noveho smeru
    jmp @okd_break             ; break
@okd_B:
    cmp eax,VK_DOWN            ; - || -
    jne @okd_C                 
    cmp [actDir],dword UP
    je @okd_break  
    mov [snakeDir],dword DOWN
    jmp @okd_break
@okd_C:
    cmp eax,VK_LEFT            ; - || -
    jne @okd_D
    cmp [actDir],dword RIGHT
    je @okd_break  
    mov [snakeDir],dword LEFT
    jmp @okd_break
@okd_D:
    cmp eax,VK_RIGHT           ; - || -
    jne @okd_E
    cmp [actDir],dword LEFT
    je @okd_break  
    mov [snakeDir],dword RIGHT
    jmp @okd_break
@okd_E:
    cmp eax,VK_RETURN          ; klavesa enter
    jne @okd_break             ; jina klavesa -> nedelame nic
    cmp [gameOver],dword 1     ; zda je konec hry
    jne @okd_break             ; neni -> nedelame nic
    xor eax,eax
    mov [gameOver],eax         ; jinak nova hra
    call GameInit              ; nastavi novou hru
@okd_break: 
    return
end ; OnKeyDown

;********************************************************************   
;* Inicializuje hru. Nastavi implicitni delku a pozici hada,
;* vynuluje skore, nastavi implicitni smer pohybu a vytvori
;* novou pozici potravy.
;********************************************************************
function GameInit
;********************************************************************
begin    
    ; vynulovani skore
    xor eax,eax
    mov [score],eax
    
    ; nastaveni smeru pohybu
    mov eax,dword RIGHT
    mov [snakeDir],eax
    mov [actDir],eax    
    
    ; nastaveni delky hada
    mov eax,dword 3
    mov [snakeLen],eax 
    
    ; nastaveni inkrementace zelene slozky hada
    fld1
    fidiv dword [snakeLen]
    fstp dword [colorInc]]   
    
    ; nastaveni pozice hada (pro delku 3)
    mov eax,dword [c2]
    mov [snakeBody],eax
    mov eax,dword [c5]
    mov [snakeBody+4],eax 
    
    mov eax,dword [c3]
    mov [snakeBody+8],eax
    mov eax,dword [c5]
    mov [snakeBody+12],eax
    
    mov eax,dword [c4]
    mov [snakeBody+16],eax
    mov eax,dword [c5]
    mov [snakeBody+20],eax
    
    ; vytvoreni nove pozice potravy
    call NewFood     
    return
end ; GameInit

;********************************************************************   
;* Vytvori novou pozici potravy pro hada. Pouzije generator nahodnych
;* cisel. Pokud vygenerovana pozice lezi na souradnici tela hada,
;* generuje znovu.
;********************************************************************
function NewFood
;********************************************************************
begin
@cyc_foodGen:                ; cykleni dokud nejsou souradnice mimo telo
    ; generovani x-souradnice
    invoke rand              ; v eax nahodna hodnota
    xor edx,edx              ; vynulovani edx pro deleni
    mov ebx,dword 10         ; nastaveni delitele
    idiv ebx                 ; vydelime  edx:eax hodnotou 10 
    shl edx,2                ; v edx zbytek po deleni, vynasobeni sizeof(dw)
    mov eax,dword [c0+edx]   ; do eax souradnice prevedene na desetinne cislo
    mov [foodPos],eax        ; zkopirovani do x-hodnoty foodPos
    
    ; generovani y-souradnice
    invoke rand              ; - || -
    xor edx,edx
    mov ebx,dword 10
    idiv ebx    
    shl edx,2    
    mov eax,dword [c0+edx]
    mov [foodPos+4],eax      ; zkopirovani do y-hodnoty foodPos
    
    ; overeni zda nova pozice nelezi na nejake pozici hada
    mov edx,snakeBody
    mov ebx,[snakeLen]       ; horni hranice cyklu = delka hada
    shl ebx,3                ; vynasobeni 2*sizeof(dw) pro snazsi indexovani
    xor ecx,ecx              ; vynulovani citace cyklu
@cyc_validCheck:             ; cyklus porovnani souradnic celeho tela s foodPos
    cmp ecx,ebx              ; porovnani citace s horni hranici
    jnb @cyc_validCheck_end  ; kdyz citac >= horni hranice -> prosli jsme cele telo    
    
    mov eax,[foodPos]        ; do eax x-hodnotu nove vygenerovane pozice
    cmp eax,[edx+ecx]        ; porovnavani s odpovidajici x-hodnotou tela hada
    jne @cyc_validCheck_cont ; neshoduje se -> porovname dalsi
    mov eax,[foodPos+4]      ; do eax y-hodnotu nove vygenerovane pozice
    cmp eax,[edx+ecx+4]      ; porovnavani s odpovidajici y-hodnotou tela hada
    jne @cyc_validCheck_cont ; neshoduje se -> porovname dalsi
    
    jmp @cyc_foodGen         ; nejaka hodnota se shodovala -> nove generovani
@cyc_validCheck_cont:    
    add ecx,dword 8          ; inkrementace citace
    jmp @cyc_validCheck      ; na zacatek cyklu
@cyc_validCheck_end:         ; konec cyklu    
    return
end ; NewFood

;********************************************************************   
;* Pohyb tela hada. Provadi pohyb hada na zaklade aktualniho
;* smeru a projizdeni skrz konce plochy.
;********************************************************************
function SnakeMove
;********************************************************************
begin
    ; pohyb tela hada
    mov edx,snakeBody
    mov ebx,[snakeLen]       ; horni hranice cyklu = delka hada
    dec ebx
    shl ebx,3                ; vynasobeni 2*sizeof(dw) pro snazsi indexovani
    xor ecx,ecx              ; vynulovani citace cyklu
@cyc_bodyMove:               ; cyklus posunu pole se souradnicemi tela hada
    cmp ecx,ebx              ; porovnani citace s horni hranici
    jnb @cyc_bodyMove_end    ; kdyz citac >= horni hranice -> posunuli jsme cele telo
    push ebx
    mov ebx,[edx+ecx+8]      ; ebx = snakeBody[i].x
    mov [edx+ecx],ebx        ; snakeBody[i-1].x = ebx
    mov ebx,[edx+ecx+12]     ; ebx = snakeBody[i].y
    mov [edx+ecx+4],ebx      ; snakeBody[i-1].y = ebx
    pop ebx
    add ecx,dword 8          ; inkrementace citace
    jmp @cyc_bodyMove    
@cyc_bodyMove_end:           ; konec cyklu
    
    ; pohyb hlavy hada
    add edx,ebx              ; posun na posledni clanek hada (=hlava)
    mov eax,[snakeDir]       
    ; switch porovnani smeru pohybu
    cmp eax,dword UP         ; smer nahoru
    jne @move_B              ; jiny smer
    fld dword [edx+4]        ; nacteni y-souradnice hlavy
    fsub dword [c1]          ; odecteni jednicky
    fld dword [c_1]          ; nacteni -1 (st0 = -1, st1 = [edx+4]) 
    fcomip st1               ; porovnani
    jnz @Anot_onEnd          ; pokud se neshoduji, nejsme na zacatku plochy   
    fadd dword [c10]         ; jsme na zacatku plochy, objevime se na konci
@Anot_onEnd:     
    fstp dword [edx+4]       ; nahrani nove hodnoty do y-souradnice hlavy
    jmp @move_break          ; break
@move_B:
    cmp eax,dword DOWN       ; smer dolu
    jne @move_C              ; jiny smer
    fld dword [edx+4]        ; nacteni y-souradnice hlavy
    fadd dword [c1]          ; pricteni jednicky
    fld dword [c10]          ; nacteni 10 (st0 = 10, st1 = [edx+4])
    fcomip st1               ; porovnani
    jnz @Bnot_onEnd          ; pokud se neshoduji, nejsme na konci plochy    
    fsub dword [c10]         ; jsme na konci plochy, objevime se na zacatku
@Bnot_onEnd:     
    fstp dword [edx+4]       ; nahrani nove hodnoty do y-souradnice hlavy
    jmp @move_break          ; break
@move_C:
    cmp eax,dword LEFT       ; smer doleva
    jne @move_D              ; jiny smer
    fld dword [edx]          ; nacteni x-souradnice hlavy
    fsub dword [c1]          ; odecteni jednicky
    fld dword [c_1]          ; nacteni -1 (st0 = -1, st1 = [edx])
    fcomip st1               ; porovnani
    jnz @Cnot_onEnd          ; pokud se neshoduji, nejsme na zacatku plochy   
    fadd dword [c10]         ; jsme na zacatku plochy, objevime se na konci
@Cnot_onEnd:     
    fstp dword [edx]         ; nahrani nove hodnoty do x-souradnice hlavy
    jmp @move_break          ; break
@move_D:
    cmp eax,dword RIGHT      ; smer doprava
    jne @move_break          ; jiny smer
    fld dword [edx]          ; nacteni x-souradnice hlavy
    fadd dword [c1]          ; pricteni jednicky
    fld dword [c10]          ; nacteni 10 (st0 = 10, st1 = [edx])
    fcomip st1               ; porovnani
    jnz @Dnot_onEnd          ; pokud se neshoduji, nejsme na konci plochy    
    fsub dword [c10]         ; jsme na konci plochy, objevime se na zacatku
@Dnot_onEnd:     
    fstp dword [edx]         ; nahrani nove hodnoty do x-souradnice hlavy
@move_break:    
    return    
end ; SnakeMove

;********************************************************************   
;* Provadi detekci "sezrani" potravy a nasledne zvetseni tela hada i
;* hodnoty skore.
;********************************************************************
function FoodEating
;********************************************************************
begin
    mov edx,snakeBody
    mov ebx,[snakeLen]
    dec ebx
    shl ebx,3
    add edx,ebx              ; posun na posledni clanek hada (=hlava)     
    ; test "sezrani" potravy
    mov eax,[edx]            ; do eax x-souradnici hlavy
    cmp eax,[foodPos]        ; porovnani s x-souradnici potravy
    jne @not_eating          ; kdyz se neshoduje, nebude se zrat
    mov eax,[edx+4]          ; do eax y-souradnici hlavy
    cmp eax,[foodPos+4]      ; porovnani s y-souradnici potravy
    jne @not_eating          ; kdyz se neshoduje, nebude se zrat
    ; potrava sezrana -> zvyseni delky hada
    inc dword [snakeLen]     ; zvyseni delky
    mov eax,[edx]            ; zkopirovani puvodni x-souradnice
    mov ebx,[edx+4]          ; zkopirovani puvodni y-souradnice
    add edx,dword 8          ; posun na nove souradnice hlavy
    mov [edx],eax            ; zaplneni nove x-souradnice predeslou
    mov [edx+4],ebx          ; zaplneni novy y-souradnice predeslou
    ; potrava sezrana -> posunuti pole (rust)
    mov edx,snakeBody
    mov ecx,[snakeLen]       ; horni hranice cyklu = delka hada
    dec ecx
    shl ecx,3                ; vynasobeni 2*sizeof(dw) pro snazsi indexovani
@cyc_bodyShift:              ; cyklus zpetneho posouvani souradnic tela
    cmp ecx,dword 0          ; porovnani citace s dolni hranici
    jna @cyc_bodyShift_end   ; kdyz citac <= dolni hranice -> posunuli jsme cele telo
    mov ebx,edx              ; ebx = ukazatel na zacatek tela hada
    add ebx,ecx              ; posun na snakeBody[i]
    sub ebx,dword 8          ; posun na snakeBody[i-1].x
    mov eax,[ebx]            ; eax = snakeBody[i-1].x
    mov [edx+ecx],eax        ; snakeBody[i].x = eax
    mov ebx,edx              ; ebx = ukazatel na zacatek tela hada
    add ebx,ecx              ; posun na snakeBody[i]
    sub ebx,dword 4          ; posun na snakeBody[i-1].y
    mov eax,[ebx]            ; eax = snakeBody[i-1].y
    mov [edx+ecx+4],eax      ; snakeBody[i].y = eax
    sub ecx,dword 8          ; dekrementace citace
    jmp @cyc_bodyShift
@cyc_bodyShift_end:          ; konec cyklu    
    ; potrava sezrana -> uprava inkrementace zelene slozky hada
    fld1                     
    fidiv dword [snakeLen]
    fstp dword [colorInc]      
    ; potrava sezrana -> zvyseni hodnoty skore a vytvoreni nove potravy
    inc dword [score]
    call NewFood
@not_eating:
    return
end ; FoodEating

;********************************************************************   
;* Detekuje kolizi hlavy s telem hada a nasledne ukonci
;* hru (gameOver=1).
;********************************************************************
function Collision
;********************************************************************
begin
    mov edx,snakeBody
    mov ebx,[snakeLen]   ; horni hranice cyklu = delka hada
    dec ebx
    shl ebx,3            ; vynasobeni 2*sizeof(dw) pro snazsi indexovani
    mov eax,edx
    add eax,ebx          ; do eax adresa posledniho clanku hada (=hlava)   
    sub ebx,dword 8
    xor ecx,ecx          ; vynulovani citace cyklu
@cyc_coll:               ; cyklus porovnavani souradnic hlavy s telem hada
    cmp ecx,ebx          ; porovnani citace s horni hranici
    jnb @cyc_coll_end    ; kdyz citac >= horni hranice -> prosli jsme cele telo
    push ebx      
    mov ebx,[eax]        ; do ebx x-souradnici hlavy hada
    cmp ebx,[edx+ecx]    ; porovnani s x-souradnici prochazenym telem hada
    pop ebx
    jne @cyc_col_cont    ; pokud se neshodovaly, kontrola dalsiho clanku
    push ebx
    mov ebx,[eax+4]      ; do ebx y-souradnici hlavy hada
    cmp ebx,[edx+ecx+4]  ; porovnani s y-souradnici prochazenym telem hada
    pop ebx      
    jne @cyc_col_cont    ; pokud se neshodovaly, kontrola dalsiho clanku
    mov eax,dword 1      ; jinak konec hry
    mov [gameOver],eax   ; nastaveni promenne gameOver na 1
    jmp @cyc_coll_end    ; skok na konec cyklu
@cyc_col_cont:
    add ecx,dword 8      ; inkrementace citace
    jmp @cyc_coll
@cyc_coll_end:           ; konec cyklu
    return
end ; Collision

;********************************************************************   
;* Funkce volana casovacem 1. Vola funkce pro pohyb hada, pojidani
;* potravy a detekci kolizi.
;********************************************************************
function OnTimer1
;********************************************************************
begin
    ; pomocne nastaveni pro zmenu smeru
    mov eax,[snakeDir]
    mov [actDir],eax
    
    ; obsluha pohybu hada
    call SnakeMove
    
    ; obsluha pojidani potravy
    call FoodEating
        
    ; obsluha kolizi
    call Collision    
    
    return
end ; OnTimer1

;********************************************************************   
;* Funkce volana casovacem 2. Provadi aktualizaci levitace hraci
;* plochy a rotaci potravy.
;********************************************************************
function OnTimer2
;********************************************************************
begin
    ; levitace herni plochy
    fld dword [levCnt]          ; nahrani citace levitace
    fadd dword [levInc]         ; inkrementace
    fld dword [levMax]          ; nahrani 2*PI (st0 = 2*PI, st1 = levCnt+levInc)
    fcomip st1                  ; porovnani
    ja @levCntLessThanPI2       ; pokud je citac >= 2*PI -> nulujeme
    fldz                        ; nahrani nuly (st0 = 0, st1 = citac)
@levCntLessThanPI2:    
    fstp dword [levCnt]         ; ulozeni hodnoty citace
    finit                       ; init FPU (v st0 mohla zustat hodnota citace)
    fld dword [levCnt]          ; nahrani nove hodnoty citace
    fsin                        ; st0 = sin(levCnt)
    fdiv dword [levRng]         ; uprava rozsahu (st0 = sin(levCnt)/levRng)
    fstp dword [levRes]         ; ulozeni vysledne hodnoty
    
    ; rotace potravy
    fld dword [rotCnt]          ; nahrani citace rotace
    fadd dword [rotInc]         ; inkrementace
    fld dword [rotMax]          ; nahrani 360 (st0 = 360, st1 = rotCnt+rotInc)
    fcomip st1                  ; porovnani
    ja @rotCntLessThan360       ; pokud je citac >= 360 -> nulujeme
    fldz                        ; nahrani nuly (st0 = 0, st1 = citac)
@rotCntLessThan360:
    fstp dword [rotCnt]         ; ulozeni hodnoty citace
    finit                       ; init FPU (v st0 mohla zustat hodnota citace)    
    return
end ; OnTimer2

;********************************************************************   
;* Funkce pocita casovy rozdil provedeny od posledni aktualizace a
;* na zaklade ubehnuteho intervalu vola funkce casovacu.
;* Pokud je konec hry, funkci casovace 1 nevola.
;********************************************************************
function Update
;********************************************************************
begin    
    ; casovac 1
@cyc_timing1:                   ; cyklus zajistujici potrebny pocet aktualizaci
    mov ebx,[lastTime1]         ; do ebx cas minule aktualizace
    add ebx,[timerInterval1]    ; pricteme interval casovace
    invoke GetTickCount         ; zjisteni aktualniho casu
    cmp eax,ebx                 ; porovnani casu
    jna @cyc_timing_end1        ; pokud je aktualni vetsi, je cas aktualizovat      
    cmp [gameOver],dword 1      ; je konec hry?
    je @gameOver                ; pokud ano, preskakujeme
    call OnTimer1               ; pokud ne, volame funkci casovace 1
@gameOver:    
    ; nastaveni noveho casu posledni aktualizace
    mov eax,[lastTime1]         ; do eax stary cas posledni aktualizace
    add eax,[timerInterval1]    ; pricteni intervalu casovace
    mov dword [lastTime1],eax   ; nastaveni promenne
    jmp @cyc_timing1
@cyc_timing_end1:               ; konec cyklu       

    ; casovac 2
@cyc_timing2:                   ; cyklus zajistujici potrebny pocet aktualizaci
    mov ebx,[lastTime2]         ; do ebx cas minule aktualizace
    add ebx,[timerInterval2]    ; pricteme interval casovace
    invoke GetTickCount         ; zjisteni aktualniho casu
    cmp eax,ebx                 ; porovnani casu
    jna @cyc_timing_end2        ; pokud je aktualni vetsi, je cas aktualizovat
    call OnTimer2               ; volani funkce casovace 2
    ; nastaveni noveho casu posledni aktualizace
    mov eax,[lastTime2]         ; do eax stary cas posledni aktualizace
    add eax,[timerInterval2]    ; pricteni intervalu casovace
    mov dword [lastTime2],eax   ; nastaveni promenne
    jmp @cyc_timing2
@cyc_timing_end2:               ; konec cyklu 
    return
end ; Update

;********************************************************************   
;* Vykresli celou scenu.
;********************************************************************
function Render
;********************************************************************
begin            
    invoke glMatrixMode,GL_MODELVIEW             ; nastavi modelview matici
    invoke glClear,GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ; vymaze obrazovku
    invoke glLoadIdentity                        ; nahraje jednotkovou matici
 
    invoke glTranslatef,-1.0f,-0.2f,-2.1f        ; zakladni posun sceny
    
    ; tisk hodnoty skore
    invoke glPushMatrix                          ; ulozeni aktualni matice
    invoke glTranslatef,3.9f,1.5f,-4.0f          ; posun na pozici tisku
    invoke glColor3f,1.0f,0.0f,0.0f              ; nastaveni cervene barvy
    call PrintScore,[score]                      ; tisk skore
    invoke glPopMatrix                           ; navrat k puvodni matici
    
    ; tisk vertikalniho textu 'Snake 3D'
    invoke glPushMatrix                          ; ulozeni aktualni matice
    invoke glTranslatef,-2.35f,-2.07f,-4.0f      ; posun na pozici tisku
    invoke glRotatef,90.0f,0.0f,0.0f,1.0f        ; natoceni o 90 stupnu v ose z
    invoke glColor3f,0.1f,0.1f,0.1f              ; nastaveni sede barvy
    call PrintText,szSnake3D,lenof.szSnake3D     ; tisk textu
    invoke glPopMatrix                           ; navrat k puvodni matici
    
    ; tisk textu 'Game Over!'
    invoke glColor3f,0.03f,0.03f,0.03f           ; nastaveni tmave sede barvy
    cmp [gameOver],dword 1                       ; je konec hry?
    jne near @notGameOver                        ; pokud ne, preskoc zmenu barvy
    invoke glColor3f,0.0f,0.0f,0.5f              ; nastaveni modre barvy
@notGameOver: 
    invoke glPushMatrix                          ; ulozeni aktualni matice
    invoke glTranslatef,3.2f,-2.87f,-7.0f        ; posun na pozici tisku
    call PrintText,szGameOver,lenof.szGameOver   ; tisk textu
    invoke glPopMatrix                           ; navrat k puvodni matici
    
    ; tisk textu 'Press -Enter-'
    invoke glPushMatrix                          ; ulozeni aktualni matice
    invoke glTranslatef,7.9f,-7.5f,-18.0f        ; posun na pozici tisku
    call PrintText,szContinue,lenof.szContinue   ; tisk textu
    invoke glPopMatrix                           ; navrat k puvodni matici 
    
    ; vykresleni hraci plochy
    invoke glRotatef,35.0f,1.0f,0.0f,0.0f        ; rotace hraci plochy v ose x
    invoke glRotatef,20.0f,0.0f,1.0f,0.0f        ; rotace hraci plochy v ose y
    
    invoke glTranslatef,0.04f,0.0f,-2.03f        ; posun hraci plochy
    invoke glRotatef,20.0f,1.0f,0.0f,0.0f        ; dalsi uprava rotace     
    invoke glTranslatef,0.0f,[levRes],0.0f       ; levitace
    
    invoke glColor3f,0.2f,0.2f,0.2f              ; nastaveni sede barvy
    call DrawBlock,0,0,0,[planeX],[planeY],[planeZ] ; vykresleni plochy
    
    ; vykresleni hada na hraci plose                   
    invoke glTranslatef,0.0f,[planeY],0.0f       ; posun nad hraci plochu
    call DrawSnake                               ; vykresleni hada
    
    ; vykresleni potravy na hraci plose
    invoke glColor3f,0.0f,1.0f,0.0f              ; nastaveni zelene barvy
    call DrawFood                                ; vykresleni potravy
    
    invoke SwapBuffers,[hDC]        ; prohozeni bufferu (double buffering)    
    return
end ; Render

;********************************************************************   
;* Vykresli hada.
;********************************************************************
function DrawSnake
;********************************************************************
var bPosX,bPosZ,color
begin        
    xor eax,eax           ; vynuluje zelenou slozku barvy
    mov [color], eax  
    mov ecx,[snakeLen]    ; do citace delku hada
    mov edx,snakeBody     ; do edx ukazatel na zacatek tela
@cyc_bodyDraw:            ; cyklus kresleni tela (vsech jaho clanku)
    fld  dword [edx]      ; nahraje x-souradnici clanku tela
    fmul dword [spacer]   ; vynasobi konstantou pro rozmisteni
    fstp dword [bPosX]    ; ulozi do pomocne promenne
    
    fld dword [edx+4]     ; nahraje y-souradnici clanku tela
    fmul dword [spacer]   ; vynasobi konstantou pro rozmisteni
    fstp dword [bPosZ]    ; ulozi do pomocne promenne   
       
    push ecx              ; zachova pouzivane registry ulozenim na zasobnik
    push edx
    invoke glColor3f,1.0f,[color],0.0f   ; nastaveni barvy
    ; vykresli kvadr zadanych rozmeru na vypocitane souradnici 
    call DrawBlock,[bPosX],0,[bPosZ],[bodyX],[bodyY],[bodyZ]     
    pop edx               ; nahraje zpet hodnoty registru
    pop ecx      
    
    fld dword [color]     ; nahraje aktualni slozku zelene
    fadd dword [colorInc] ; zvysi jeji hodnotu
    fstp dword [color]    ; ulozi novou hodnotu
    
    add edx,dword 8       ; posune se na dalsi clanek
    loop @cyc_bodyDraw  
    return
end ; DrawSnake

;********************************************************************   
;* Vykresli potravu.
;********************************************************************
function DrawFood
;********************************************************************
var fPosX,fPosZ
begin     
    fld  dword [foodPos]     ; nahraje x-souradnice potravy
    fmul dword [spacer]      ; vynasobi konstantou pro rozmisteni
    fadd dword [c01]         ; posune na stred policka
    fstp dword [fPosX]       ; ulozi do pomocne promenne
    
    fld  dword [foodPos+4]   ; nahraje y-souradnice potravy
    fmul dword [spacer]      ; vynasobi konstantou pro rozmisteni
    fadd dword [c01]         ; posune na stred policka
    fstp dword [fPosZ]       ; ulozi do pomocne promenne
    
    invoke glTranslatef,[fPosX],[planeY],[fPosZ] ; posun na vypocitane souradnice
    invoke glRotatef,[rotCnt],1.0f,1.0f,1.0f     ; rotace
    invoke gluSphere,[quadric],0.115d,10,10      ; vykresleni koule
   
    return
end ; DrawFood

;********************************************************************   
;* Vykresli kvadr zadanych rozmeru na zadanou pozici.
;* @param xPos,yPos,zPos Pozice kvadru.
;* @param xLen,yLen,zLen Rozmery kvadru.
;********************************************************************
function DrawBlock,xPos,yPos,zPos,xLen,yLen,zLen
;********************************************************************
var xShf,yShf,zShf
begin    
    ; nastavi posun vzdalenejsich x-vrcholu
    fld  dword [xLen]          ; nahraje delku
    fadd dword [xPos]          ; pricte pozici
    fstp dword [xShf]          ; ulozi do promenne
    
    ; nastavi posun vzdalenejsich y-vrcholu
    fld  dword [yLen]          ; nahraje delku
    fadd dword [yPos]          ; pricte pozici
    fstp dword [yShf]          ; ulozi do promenne
    
    ; nastavi posun vzdalenejsich z-vrcholu
    fld  dword [zLen]          ; nahraje delku
    fadd dword [zPos]          ; pricte pozici
    fstp dword [zShf]          ; ulozi do promenne   
    
    invoke glBegin,GL_QUADS                     ; zacatek kresleni obdelniku

    invoke glNormal3f,   0.0f,   0.0f,   1.0f   ; normala (kvuli spravnemu osvetleni)
    invoke glVertex3f, [xShf], [yShf], [zShf]   ; souradnice vertexu
    invoke glVertex3f, [xPos], [yShf], [zShf]   ; ...
    invoke glVertex3f, [xPos], [yPos], [zShf]
    invoke glVertex3f, [xShf], [yPos], [zShf]

    invoke glNormal3f,   0.0f,   0.0f,  -1.0f   ; - || -
    invoke glVertex3f, [xPos], [yPos], [zPos]
    invoke glVertex3f, [xPos], [yShf], [zPos]
    invoke glVertex3f, [xShf], [yShf], [zPos]
    invoke glVertex3f, [xShf], [yPos], [zPos]

    invoke glNormal3f,   0.0f,   1.0f,   0.0f   ; - || -
    invoke glVertex3f, [xShf], [yShf], [zShf]
    invoke glVertex3f, [xShf], [yShf], [zPos]
    invoke glVertex3f, [xPos], [yShf], [zPos]
    invoke glVertex3f, [xPos], [yShf], [zShf]

    invoke glNormal3f,   0.0f,  -1.0f,   0.0f   ; - || -
    invoke glVertex3f, [xPos], [yPos], [zPos]
    invoke glVertex3f, [xShf], [yPos], [zPos]
    invoke glVertex3f, [xShf], [yPos], [zShf]
    invoke glVertex3f, [xPos], [yPos], [zShf]

    invoke glNormal3f,   1.0f,   0.0f,   0.0f   ; - || -
    invoke glVertex3f, [xShf], [yShf], [zShf]
    invoke glVertex3f, [xShf], [yPos], [zShf]
    invoke glVertex3f, [xShf], [yPos], [zPos]
    invoke glVertex3f, [xShf], [yShf], [zPos]

    invoke glNormal3f,  -1.0f,   0.0f,   0.0f   ; - || -
    invoke glVertex3f, [xPos], [yPos], [zPos]
    invoke glVertex3f, [xPos], [yPos], [zShf]
    invoke glVertex3f, [xPos], [yShf], [zShf]
    invoke glVertex3f, [xPos], [yShf], [zPos]
  
    invoke glEnd                                ; konec kresleni obdelniku
 
    return
end ; DrawBlock

;********************************************************************
;* Vytvori font.
;********************************************************************
function BuildFont
;********************************************************************
var font,oldFont
begin
    invoke AddFontResource,szFontFilename        ; prida font
    invoke glGenLists,256                        ; generuje 256 znaku
    mov [fontBase],eax                           ; ulozi cislo display listu
    invoke CreateFont,\
         -24,0,0,0,\
         FALSE,FALSE,FALSE,FALSE,\
         ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,4,\
         FF_DONTCARE | DEFAULT_PITCH,szFontName  ; vytvori font
    mov [font],eax    
    invoke SelectObject,[hDC],[font]             ; vyber fontu do DC
    mov [oldFont],eax                            ; ulozeni puvodniho fontu
    invoke wglUseFontOutlines,[hDC],0,255,[fontBase],0.0f,0.1f,1,gmf ; vytvori 3D font
    invoke SelectObject,[hDC],[oldFont]          ; vybere zpet puvodni font
    invoke DeleteObject,[font]                   ; smaze nas font
    return
end ; BuildFont

;********************************************************************
;* Vytiskne 3D text s hodnotou skore na scenu.
;* @param value Hodnota skore
;********************************************************************
function PrintScore,value
;********************************************************************
begin
    invoke glPushAttrib,GL_LIST_BIT  ; ulozi soucasny stav display listu
    invoke glListBase,[fontBase]     ; nastavi prvni display list
    invoke sprintf,scoreBuff,szScoreFormat,[value] ; prevede skore na retezec
    add esp, 12                      ; uklidi zasobnik
    invoke strlen,scoreBuff          ; spocita delku retezce
    invoke glCallLists,eax,GL_UNSIGNED_BYTE,scoreBuff ; nastavi prvni display list 
    invoke glPopAttrib               ; obnovi puvodni stav display listu
    return
end ; PrintScore

;********************************************************************   
;* Vytiskne 3D text na scenu.
;* @param text Text k vytisknuti.
;* @param len  Delka textu.
;********************************************************************
function PrintText,text,len
;********************************************************************
begin
    invoke glPushAttrib,GL_LIST_BIT  ; ulozi soucasny stav display listu
    invoke glListBase,[fontBase]     ; nastavi prvni display list              
    invoke glCallLists,[len],GL_UNSIGNED_BYTE,[text] ; nastavi prvni display list
    invoke glPopAttrib               ; obnovi puvodni stav display listu
    return
end ; PrintText

; < konec Snake3D.asm > 
    