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
