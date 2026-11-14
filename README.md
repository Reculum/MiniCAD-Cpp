# NURBS Visualizer

A 3D NURBS (Non-Uniform Rational B-Spline) visualizer developed in **C++** using **raylib**.

The project provides an interactive environment for creating, manipulating, and visualizing NURBS curves and surfaces, with an emphasis on implementing the underlying mathematics from scratch.

## Features

- **NURBS curves:** evaluation using rational B-spline basis functions.
- **NURBS surfaces:** tensor-product NURBS surface evaluation.
- **Control points:** creation, selection, movement, and deletion.
- **Weight editing:** modify control-point weights to manipulate rational geometry.
- **Adaptive curve tessellation:** dynamically refine curve representations.
- **Surface rendering:** tessellation, shading, and 3D visualization.
- **Interactive 3D environment:** camera controls and geometric object manipulation.

## Implementation

The project is written in **C++17** and uses:

- [raylib](https://www.raylib.com/) for graphics, rendering, and input handling.
- [CMake](https://cmake.org/) for project configuration and building.

The NURBS evaluation algorithms and geometric data structures are implemented within the project.

## Building

Requirements:
- C++17-compatible compiler
- CMake
- raylib

Build using:

```bash
cmake -S . -B build
cmake --build build
```

Run the resulting executable from the build directory.

## Motivation

This project was developed as a practical exploration of NURBS geometry, computational geometry, and 3D graphics programming. Its main objective was to implement the mathematical foundations of NURBS curves and surfaces and integrate them into an interactive visualization tool.
