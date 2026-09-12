# Dijkstra's Algorithm — Navigation & Pathfinding

## Project Overview

This C++ project implements a navigation and pathfinding system using Dijkstra's shortest-path algorithm on weighted graphs.

The program reads map data from an input file, constructs the graph using both an **adjacency matrix** and an **adjacency list**, and calculates the shortest route between user-selected source and destination vertices.

The two graph representations are also benchmarked to compare their execution times when running Dijkstra's algorithm.

This project was originally developed as part of my Algorithm Design and Analysis coursework.

## Features

- Weighted graph representation
- Adjacency matrix implementation
- Adjacency list implementation
- Dijkstra's shortest-path algorithm
- Shortest-distance calculation
- Path reconstruction
- File-based map loading
- Multiple source-destination queries
- Execution-time measurement
- Performance comparison between graph representations

## Graph Representation

The program represents maps as weighted graphs where:

- Vertices represent locations
- Edges represent connections between locations
- Edge weights represent distances

The graph is constructed using two different data structures:

### Adjacency Matrix

The graph is represented using a matrix containing the connections and edge weights between vertices.

### Adjacency List

Each vertex maintains a list of its neighboring vertices and corresponding edge weights.

Both representations are used independently with Dijkstra's algorithm, allowing their execution times to be compared.

## Dijkstra's Algorithm

Dijkstra's algorithm is used to determine the shortest path between a source and destination vertex in the weighted graph.

For each query, the program:

1. Receives a source and destination from the user.
2. Computes the shortest path using the adjacency matrix.
3. Computes the shortest path using the adjacency list.
4. Reconstructs the resulting route.
5. Displays the shortest distance and route.
6. Measures the execution time of both implementations.

The user can perform multiple pathfinding queries before exiting the program.

## Input Files

The program supports map data stored in text files, including:

- `input-graph.txt`
- `canada.txt`

The input data defines the vertices and weighted connections used to construct the graph.

## Compilation

Using g++:

```bash
g++ map.cpp -o map
```

## Running the Program

### Windows

```powershell
.\map.exe
```

### macOS / Linux

```bash
./map
```

When prompted, enter the name of a supported map file.

The program then allows multiple source-destination queries. Enter `quit` when finished.

## Example

Using `input-graph.txt` with source vertex `0` and destination vertex `5`:

```text
--- Adjacency Matrix Result ---
Shortest distance: 6273.99
Route: 0 -> 1 -> 2 -> 5

--- Adjacency List Result ---
Shortest distance: 6273.99
Route: 0 -> 1 -> 2 -> 5
```

Both graph representations produce the same shortest path while allowing their execution performance to be compared.

## Performance Comparison

The program measures the execution time of Dijkstra's algorithm for both the adjacency matrix and adjacency list representations.

After the user finishes querying routes, the program reports the accumulated execution time for both implementations and identifies which representation was faster during that execution.

Actual execution times may vary depending on the input graph, hardware, compiler, and system conditions.

## Concepts Demonstrated

- Dijkstra's Algorithm
- Graph Algorithms
- Weighted Graphs
- Shortest-Path Algorithms
- Adjacency Lists
- Adjacency Matrices
- Path Reconstruction
- File Parsing
- Algorithm Performance Analysis
- Runtime Benchmarking
- Data Structures and Algorithms

## Technologies

- C++
- C++ Standard Library

## Author

Mahsa Fazli
