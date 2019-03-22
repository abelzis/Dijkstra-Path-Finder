cmake_minimum_required (VERSION 3.0)
project ( DijkstraAlgorithm )

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")

add_executable( DijkstraPathFinding 
		DijkstraAlgorithm.cpp
		DijkstraAlgorithm.h
		Dijkstra.cpp
		Dijkstra.h
		Resource.h
		stdafx.h
		targetver.h
		DijkstraAlgorithm.ico
		DijkstraAlgorithm.rc
		small.ico)