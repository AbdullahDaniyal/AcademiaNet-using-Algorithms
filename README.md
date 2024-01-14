# AcademiaNet-using-Algorithms
AcademiaNet is a comprehensive graph-based system designed to model and analyze educational networks, specifically focusing on the relationships between students and teachers. It leverages advanced graph algorithms to facilitate the understanding of these complex interactions.
## Description
This project involves the creation of a graph data structure to model the intricate network of relationships in an educational setting. The C++ component of the project implements several fundamental graph algorithms, including Dijkstra's algorithm for shortest path finding, Kruskal's algorithm for minimum spanning tree construction, and breadth-first and depth-first searches for graph traversal. Additionally, the project uses Python with NetworkX and Matplotlib for visualizing the graph, thereby providing an intuitive representation of the educational network.
## Features
1. Graph creation and manipulation in a classroom context.
2. Implementation of key graph algorithms:
Dijkstra's algorithm for shortest path calculation.
Kruskal's algorithm for minimum spanning tree.
Breadth-First and Depth-First Searches.
3. Export functionality to JSON for graph data.
4. Python-based graph visualization using NetworkX and Matplotlib.
5. Custom handling of educational entities like students, teachers, and workshops.
## Installation
1. To set up the project, clone the repository to your local machine:
git clone [URL-of-Your-GitHub-Repository]
Ensure you have a C++ compiler and Python installed to run both parts of the project.
## Usage
1. To run the C++ application, navigate to the project directory and compile the code:
g++ -o eduGraphConnect Source.cpp
./eduGraphConnect
2. To visualize the graph in Python, ensure you have NetworkX and Matplotlib installed:
pip install networkx matplotlib
python visualize_graph.py
## Visualizations
The Python script visualize_graph.py uses NetworkX and Matplotlib to create and display the graphical representation of the educational network. The visualization differentiates between students and teachers and displays the weighted edges representing their connections.
## License
This project is licensed under the MIT License - see the LICENSE.md file for details.

![graph_visualization](https://github.com/AbdullahDaniyal/AcademiaNet-using-Algorithms/assets/91824833/32c6fa0c-0746-4d35-af73-01f1fc66039a)
