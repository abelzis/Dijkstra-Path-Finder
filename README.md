# Dijkstra-Path-Finder
Desktop application which visualizes Dijkstra's (pathfinding) algorithm principles by allowing user to interact with the map.

# How to use?
1. Download the files
2. Open CMake and select *source path* to the files downloaded
3. Select where you will build the project ( *build binaries* )
4. Click *Configure*, select IDE/Compiler and proceed to *Generate*
5. Open the project file (if using Visual Studio *DijkstraAlgorithm.sln*)
6. Select *DijkstraPathFinding* project as start-up
7. Go to Properties->Configuration Properties and change the **Character Set* to *Use Unicode Character Set**
8. Go to Properties->Linker->System and change SubSystem to **Windows (/SUBSYSTEM:WINDOWS)**
9. Done. Debug, launch and enjoy.

# Changelog
### [v0.2.0](https://github.com/abelzis/Dijkstra-Path-Finder/releases/tag/v0.2.0) (2019-03-22)
  **Added:**
  - Eraser brush button
  - Start button to start the Dijkstra's shortest path finding algorithm
  - Clear and Reset buttons
  - Visualization of the algorithm
  - CMake support
### [v0.1.1a](https://github.com/abelzis/Dijkstra-Path-Finder/releases/tag/v0.1.1a) (2019-03-18)
  **Added:**
  - Start and End position buttons
  - Obstacle button
### [v0.1](https://github.com/abelzis/Dijkstra-Path-Finder/releases/tag/v0.1) (initial) (2019-03-15)
  **Added:**
  - Grid of the map.
  - Start position button, marking a tile green.
