# README.txt for Transportation

## The Project
This C++ project simulates and manages complex networks, potentially reflecting transportation or communication systems. Using object-oriented principles and advanced C++ features, the project models interactions within a multigraph environment, highlighting network nodes and edge dynamics.

## Decision Rationale
In designing the system, I chose to utilize an unordered map to store station data, anticipating potentially vast datasets where efficient data retrieval is critical. Additionally, I deliberated the introduction of inheritance in the Edge class based on vehicle type but decided against it. This approach, while initially appealing, would limit system flexibility and add unnecessary complexity. This decision was based on the current scope and significance of edge types within the project.

## Key Components

### Station Class
Handles individual network nodes, encompassing properties such as name and connectivity. This class offers robust methods for node interactions and data management, ensuring efficient operation within the network.

### Edge Class
Manages the connections between stations, dealing with characteristics like connection strength, type, and direction. It facilitates dynamic updates and interactions between nodes, supporting the network's fluidity.

### MultiGraph Class
Oversees the network structure, managing the integration and interaction of multiple nodes and their connections. It enables the addition and removal of nodes and edges, addressing complex network queries effectively.

### Interaction Class
Enables various network components to interact, possibly simulating data transfers or resource allocations. This class plays a pivotal role in the dynamic aspects of network management.

### netOmsterdam.cpp
Serves as the main driver of the project, initializing the simulation environment, integrating all components, and orchestrating the start of simulation processes.

## Running the Simulation
Execute the following to start the simulation:
```sh
./HW02ADV
