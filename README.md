This is a 1 ECTS project from my Bachelor's.

It came after the API (Algoritmi e Principi dell'Informatica -> literally *Algorthms and Informatics Principles*) course and was about optimizing a solution in c for a space and time constraints problem.

Since the project descrption is in Italian, here is an english overview gently provided by Gemini:

## 🛣️ Highway Route Planner (Algorithms & Data Structures Project)

Optimal route planner for a highway system.

### 📌 Project Overview
The program manages a highway represented as a sequence of service stations. Each station is uniquely identified by its distance from the beginning of the highway and features a rental fleet of up to 512 electric vehicles, each with a specific driving range. 

When a driver rents a car at a station, they can travel to any upcoming station within that vehicle's maximum range. At each new stop, the driver rents a fresh car with the maximum available range at that station.

### 🎯 Objective
Given a starting station and a destination, the system calculates the route with the **fewest number of stops**. 

* **Tie-breaking Rule:** If multiple routes share the same minimum number of stops, the algorithm automatically selects the route that prefers stations closer to the start of the highway (the shortest absolute distances).
* **Direction:** The system supports traveling both forward and backward along the highway.

### ⚙️ Features & Commands
The program processes an input file containing a sequence of real-time commands:
* `aggiungi-stazione`: Adds a new service station with its fleet of cars.
* `demolisci-stazione`: Removes an existing service station.
* `aggiungi-auto`: Adds a rental car to a specific station.
* `rottama-auto`: Removes a rental car from a specific station.
* `pianifica-percorso`: Calculates and prints the shortest path (by number of stops) between two stations.
