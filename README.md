# 2048

## Popis hry:

Hra 2048 spočívá v postupném spojování čísel stejné hodnoty, dokud nevznikne číslo 2048. Na haracím poli 4x4 začíná hráč se dvěma čísly hodnoty 2 na náhodné pozici. Při každém tahu hráč zvolí jeden ze 4 směrů a všechna čísla se posunou v daném směru o maximální možnou vzdálenost. Následně se na náhodnou volnou pozici vloží další číslo s hodnotou 2 nebo 4. Pokud do sebe narazí dvě stejná čísla, dojde k jejich spojení. Hra končí pokud dojde k dosažení hodnoty 2048, nebo jsou plně obsazeny všechny pozice a nelze se již dále pohnout.

## Specifikace programu:

- při spuštění se v konzoli vykreslí menu kde si bude možné:
  - zvolit si obtížnost
    - číslo které je po třeba složit
    - velikost hrací plochy
  - zvolit si ovládání (arrow keys / wasd)
  - začít novou hru
  - sledovat hru hranou solverem
  - ukončit program
- při zvolení nové hry se v konzoli vykreslí hrací plocha a pomcí zvolených kláves se bude volit směr tahu
- při dosasžení potřebné hodnoty nebo zaplnění celé hrací plochy (bez možnosti dalšího pohybu) se v konzoli vykreslí win/lose obrazovka ke bude možné:
  - restartovat hru
  - vrátit se do úvodního menu
  - ukončit program

## Požadavky pro instalaci

- Gcc
- Cmake
- Make
- Ncurses

nebo

- Docker

## Instalace

1. `mkdir build && cd build`
2. `cmake ../`
3. `make && ./2048`

nebo

1. `sudo docker build -t cpp_2048 .`
2. `sudo docker run -it cpp_2048`

## Ovládání

V celé aplikaci je ovládání řešeno pomocí klávesnice. Konkrétně jsou využity následující klávesy.

- Pohyb: <kbd>↑</kbd><kbd>←</kbd><kbd>↓</kbd><kbd>→</kbd> nebo <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>
- Potvrzení: <kbd>⏎ Enter</kbd>
- Ukončení: <kbd>Q</kbd>,<kbd>X</kbd>
- Krokování: <kbd>N</kbd>

V libovolné chvíli lze aplikaci ukončit stlačením <kbd>Q</kbd>.

### Menu

V každém menu je pohyb mezi možnostmi vertikální. Mezi možnostmi se tak lze pohybovat pomocí dvojice <kbd>↑</kbd><kbd>↓</kbd> nebo <kbd>W</kbd><kbd>S</kbd> (obojí funguje po celou dobu) a aktuálně zvýrazněnou možnost potvrdit <kbd>⏎ Enter</kbd>.

### Hra v režimu hráče

Pro volbu směru, kterým se mají dlaždice posunout, slouží <kbd>↑</kbd><kbd>←</kbd><kbd>↓</kbd><kbd>→</kbd> nebo <kbd>W</kbd><kbd>A</kbd><kbd>S</kbd><kbd>D</kbd>. Aktivní je vždy pouze jedna kombinace dle volby v hlavním menu. Ukončit aktuální stav hry a vrátit se do hlavního menu lze stlačením <kbd>X</kbd>.

### Hra v režimu solveru

V režimu solveru probíhá krokování jednotlivých tahů stlačením <kbd>N</kbd>. Ukončit aktuální stav hry a vrátit se do hlavního menu lze opět stlačením <kbd>X</kbd>.

## Architektura

Aplikace je rozdělena do čtyř hlavních čáští: Control, States, Engine, Solver.

### Control

Třída `Control` slouží jako entry point celé aplikace. Uchovává globální informace (Aktuální stav, nastavení, poslední input...). Zároveň zpracovává uživatelský vstup a spravuje jednotlivé stavy aplikace.

### States

Třídy jednotlivých stavů vychází ze základní virtuální třídy `State`. `State` požaduje implementaci následujících metod:

- `render` - vykreslování stavu
- `user_input` - zpracování uživatelkého vstupu
- `toggle` - přepínání na následující stav
- `enter` - inicializace stavu při přepunutí

Pro všechny menu stavy je definována zákaldní třída `Menu : State`, která definuje motody `render` a `user_input`. Jednotlivá menu jsou pak třídy `XY : Menu`.

Posledním stavem je `Game`, který interaguje s Enginem a Solverem.

### Engine

Třída `Engine` definuje chování samotné hrací plochy. Udržuje aktuální stav hrací plochy a poskytuje metody pro její manipulaci.

### Solver

Solver je složen ze čtyř funkcí:

- `find_score_left` - vypočítá skóre pohybu vlevo
- `find_score_up`- vypočítá skóre pohybu vpravo
- `find_score_right`- vypočítá skóre pohybu dolů
- `solve` - vyhodnotí skóre a provede tah pomocí `Engine`

Skóre je počítáno jako součet volných polí daným směrem (1 bod za každé) a počtu dlaždic které lze spojit s tím, že dlaždice vyšší hodnoty mají větší váhu (konkrétně za spojení dlaždice je $2\cdot\log_2(hodnota)$ bodů). Funkce `solve` se pak snaží preferovat pouze tahy vlevo a nahoru, pokud ani jeden nelze provést dojde k tahu vpravo, až v případě že nelze provést ani ten táhne dolů.
