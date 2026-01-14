# Minesweeper Terminal

Campo Minato classico per terminale Windows con grafica colorata e controlli mouse.

## Descrizione

Versione giocabile del Minesweeper per console Windows. Interfaccia colorata con ANSI, controlli mouse nativi e tutte le meccaniche del gioco originale.

## Caratteristiche

- Controlli con mouse (sinistro scopre, destro bandiera)
- 5 livelli di difficoltà (Easy → Impossible)
- Timer e contatore bandierine
- Grafica colorata nel terminale
- Espansione automatica celle vuote
- Menu interattivo

## Livelli

- **Easy**: 15×25, 25 mine
- **Medium**: 20×35, 60 mine
- **Hard**: 26×43, 180 mine
- **Insane**: 28×45, 250 mine
- **Impossible**: 28×45, 400 mine

## Compilazione
```bash
g++ main.cpp r.o -o Minesweeper.exe
```

## Requisiti

- Windows
- Compilatore C++ (C++11+)
- Terminale con supporto ANSI

## Come Giocare

1. Scegli difficoltà dal menu
2. Click sinistro = scopri cella
3. Click destro = piazza/rimuovi bandiera
4. Scopri tutte le celle senza mine per vincere
