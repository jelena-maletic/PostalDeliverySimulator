# Postal Delivery Simulator

## Project Overview

This project is a simulation of a postal delivery service operating within a city represented as a weighted undirected graph. The goal is to efficiently deliver packages arriving at a postal office using a single delivery vehicle. The project is implemented in the C programming language.

## Key Features

- **Graph-Based City Model:**  
  The city is modeled as an n x n adjacency matrix, where nodes represent addresses and edges represent streets with associated distances.

- **Package Handling and Vehicle Capacity Constraint:**  
  Packages have attributes including name, description, weight, delivery address, and priority (randomly assigned between 0 and 100). Packages are loaded onto the delivery vehicle based on priority (highest priority first), ensuring the total weight does not exceed the vehicle’s maximum capacity. Packages that cannot be loaded are saved to a separate file.

- **Optimized Delivery Routing:**  
  The delivery route is optimized using the Floyd-Warshall algorithm for shortest paths, ensuring the vehicle delivers packages to the nearest address successively until all are delivered and returns to the postal office.

- **Comprehensive Simulation:**  
  The program loads the city graph and packages from input files, simulates loading and delivery, and outputs detailed progress and statistics, including the total distance traveled and percentage of delivered packages per area.

## Input Files

- **City Graph File:** Text file containing the size of the graph, list of addresses, and the adjacency matrix representing street connections and distances.

- **Packages CSV File:** Contains package data generated externally (e.g., using [Mockaroo](https://www.mockaroo.com/)), with fields: name, description, weight, address, and priority.

- This repository includes four city graph TXT files and two package CSV files provided for testing and running the simulation program.

## Program Demonstration

Below are screenshots showcasing the simulation in action:

![Simulation screenshot 1](screenshots/simulation-screenshot1.png)
![Simulation screenshot 2](screenshots/simulation-screenshot2.png)

### How to Use the Program

1. Provide the adjacency matrix file that represents the city graph.  
2. Provide the CSV file containing the package data.  
3. Specify the maximum load capacity of the delivery vehicle.  
4. Run the simulation to observe package loading, optimized routing and detailed delivery statistics.
