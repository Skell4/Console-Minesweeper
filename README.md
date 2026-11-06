# Minesweeper Terminal

[![C++11+](https://img.shields.io/badge/C%2B%2B-11%2B-00599C?logo=cplusplus&logoColor=white)](#compilation)
[![Platform: Windows](https://img.shields.io/badge/platform-Windows-7B68EE)](#requirements)

Classic Minesweeper for the Windows terminal, featuring colorful graphics and mouse controls.

## Description

A playable version of Minesweeper for the Windows console, with a colorful ANSI interface, native mouse controls, and all the mechanics of the original game.

## Features

- Mouse controls (left-click to reveal, right-click to flag)
- 5 difficulty levels (Easy → Impossible)
- Timer and flag counter
- Colorful terminal graphics
- Automatic expansion of empty cells
- Interactive menu

## Difficulty Levels

- **Easy**: 15×25, 25 mines
- **Medium**: 20×35, 60 mines
- **Hard**: 26×43, 180 mines
- **Insane**: 28×45, 250 mines
- **Impossible**: 28×45, 400 mines

## Compilation
```bash
g++ main.cpp r.o -o Minesweeper.exe
```

## Requirements

- Windows
- C++ compiler (C++11+)
- Terminal with ANSI support

## How to Play

1. Choose a difficulty level from the menu
2. Left-click to reveal a cell
3. Right-click to place or remove a flag
4. Reveal all cells without mines to win
