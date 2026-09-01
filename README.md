# Data Structures - Final Project: Steam Playtime Estimator

## Overview
This repository contains the final project for the INF01203 - Data Structures course at the Federal University of Rio Grande do Sul (UFRGS). The goal of this application is to estimate the total time required to play a given list of video games, based on public Steam statistics. 

Beyond the estimation, the project serves as a practical comparative analysis of different tree data structures implemented in C, measuring their performance regarding insertion and search operations.

## Features
* **Data Structures:** Implements and compares at least two types of trees (e.g., Binary Search Tree (BST), AVL, Red-Black, or Splay).
* **Case-Insensitive Search:** The search algorithm treats uppercase and lowercase letters as equal when looking for game titles.
* **Performance Metrics:** Outputs detailed statistics for each tree structure, including:
  * Total number of nodes
  * Tree height
  * Number of rotations
  * Number of comparisons during queries

## How to Run
The program is built in C and must be executed via the command line, passing the input and output files as arguments.

**Compilation:**
```bash
gcc -o estimador_horas main.c [other_files.c]
