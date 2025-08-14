## Project Overview

This project is the final assessment for the **Algorithms and Principles of Computer Science** course. It implements a solution to optimize routing on a highway system with electric vehicle rental stations. The objective is to determine the route with the minimum number of stops between two given stations, with a preference for routes that include stations closer to the start of the highway when multiple options have the same number of stops.

### Problem Context

- Each station is uniquely identified by its distance from the start of the highway.
- Stations maintain a fleet of electric vehicles, each with a specific battery autonomy (maximum range per charge).
- Travel is one-directional (forward only), and a new vehicle is rented at each stop.
- The solution must efficiently find the optimal route and ensure correctness under tie-breaking rules.

### Input and Output Commands

- **Add Station**  
  Command: `aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n`  
  Description: Adds a station at the specified `distanza` (distance from the highway start) with a fleet of `numero-auto` vehicles, each with a specified autonomy.

  - Example: `aggiungi-stazione 10 3 100 200 300`

  Output: `aggiunta` if successful, `non aggiunta` if a station already exists at the distance.

- **Remove Station**  
  Command: `demolisci-stazione distanza`  
  Description: Removes the station at the specified `distanza` if it exists.

  - Example: `demolisci-stazione 10`

  Output: `demolita` if successful, `non demolita` if no station exists at the distance.

- **Add Vehicle**  
  Command: `aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere`  
  Description: Adds a vehicle with the specified `autonomia-auto-da-aggiungere` to the station at `distanza-stazione`.

  - Example: `aggiungi-auto 10 150`

  Output: `aggiunta` if successful, `non aggiunta` if the station does not exist.

- **Remove Vehicle**  
  Command: `rottama-auto distanza-stazione autonomia-auto-da-rottamare`  
  Description: Removes one vehicle with the specified `autonomia-auto-da-rottamare` from the station at `distanza-stazione` if it exists.

  - Example: `rottama-auto 10 150`

  Output: `rottamata` if successful, `non rottamata` if the vehicle or station does not exist.

- **Plan Route**  
  Command: `pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo`  
  Description: Plans the optimal route from `distanza-stazione-partenza` to `distanza-stazione-arrivo`, minimizing stops and preferring shorter distances for tie-breaking.

  - Example: `pianifica-percorso 20 50`

  Output:

  - A space-separated sequence of distances (a.k.a. stations) representing the route, ending with a newline.
  - If start and destination are the same, the single station is output once.
  - `nessun percorso` if no valid route exists.

## Development Details

### Tools and Platform

Due to platform-specific constraints on macOS, I utilized an **Ubuntu Virtual Machine** to access all recommended tools for performance measurement and debugging:

- **Valgrind**: Memory management and performance optimization.
- **Callgrind**: Profiling for time efficiency.
- **Massif**: Heap usage analysis.
- **gcc** with flags `-Wall -Werror -std=gnu11 -O2`.

### Data Structures and Optimization

Initially, the solution was implemented using a **hashmap (dictionary)**, but the approach proved too slow, resulting in a runtime of approximately **2 hours** for large datasets. To address this:

- Replaced the hashmap with a **doubly linked list** as well as optimizing the algorithm, which reduced the runtime to around **0.7 seconds** for equivalent workloads.

This optimization enabled efficient route planning and demonstrated significant computational improvements.

## Results and Recognition

The project was rigorously tested and achieved full marks, scoring **30/30 CUM LAUDE**, reflecting both correctness and performance excellence.
