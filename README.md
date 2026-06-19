# PoolRide – Digital Ride-Share Passenger Carpool Engine

## 1. Project Title

**PoolRide – Digital Ride-Share Passenger Carpool Engine Using Data Structures and Algorithms (C++)**

---

## 2. Problem Statement

PoolRide is a ride-sharing carpool system designed to match multiple passengers traveling in the same direction into a single vehicle. The system must efficiently manage users, process ride requests in order, calculate optimal routes, handle dynamic pricing, and minimize travel costs.

Traditional ride-sharing systems face several challenges:

* Large user databases become slow to access.
* Surge pricing changes cannot be reverted instantly.
* Ride requests are processed randomly instead of fairly.
* Cars are not ranked according to available seats.
* Route optimization is difficult without a street network model.
* Toll charges increase travel costs unnecessarily.

The objective of this project is to solve these challenges using appropriate Data Structures and Algorithms from the DSA curriculum. The system simulates real-world carpooling services such as Uber Pool and Ola Share.

---

## 3. Objectives

The major objectives of the project are:

1. Store user information efficiently using a disk-optimized structure.
2. Maintain a history of surge pricing and support rollback operations.
3. Process ride requests in First-In-First-Out (FIFO) order.
4. Quickly locate passenger information using IDs and request tokens.
5. Rank available cars according to vacant seats.
6. Represent a city road network using graphs.
7. Calculate the shortest travel-time route between locations.
8. Find routes with minimum toll costs.
9. Demonstrate practical applications of Data Structures and Algorithms.
10. Simulate real-world ride-sharing systems such as Uber Pool and Ola Share.

---

## 4. System Overview / Architecture

The PoolRide system consists of eight major modules:

### Module 1: User Directory

Stores registered users and their frequent travel routes.

**Data Structure Used:** B-Tree

### Module 2: Price Log

Maintains surge pricing history and allows rollback of recent changes.

**Data Structure Used:** Stack

### Module 3: Ride Request Line

Processes ride requests in the order they arrive.

**Data Structure Used:** Queue

### Module 4: Location Finder

Stores passenger location information.

**Data Structure Used:** Singly Linked List

### Module 5: Capacity Sorter

Ranks cars based on available seats.

**Data Structure Used:** Array + Insertion Sort

### Module 6: Street Network

Represents the city road map.

**Data Structure Used:** Weighted Graph (Adjacency List)

### Module 7: Carpool Route Finder

Calculates the shortest-time route.

**Algorithm Used:** Dijkstra's Algorithm

### Module 8: Toll Optimizer

Finds routes with minimum toll charges.

**Algorithm Used:** Dijkstra's Algorithm

---

## 5. Data Structures and Algorithms Used

| Feature            | Data Structure / Algorithm | Purpose                           |
| ------------------ | -------------------------- | --------------------------------- |
| User Directory     | B-Tree                     | Efficient user storage and search |
| Price Log          | Stack                      | Rollback latest surge price       |
| Ride Request Line  | Queue                      | FIFO request processing           |
| Location Finder    | Linked List                | Store passenger locations         |
| Location Search    | Linear Search              | Search by passenger ID            |
| Capacity Sorter    | Array                      | Store available cars              |
| Capacity Ranking   | Insertion Sort             | Sort cars by empty seats          |
| Street Network     | Graph                      | Represent city roads              |
| Route Optimization | Dijkstra Algorithm         | Minimum travel time               |
| Toll Optimization  | Dijkstra Algorithm         | Minimum toll cost                 |

This implementation directly follows topics from the DSA syllabus including Linear Search, Insertion Sort, Linked Lists, Stacks, Queues, B-Trees, Graphs, and Dijkstra's Algorithm. 

---

## 6. Implementation Approach

### Step 1: User Registration

* User enters name and phone number.
* System generates a unique User ID.
* Information is stored in a B-Tree.

### Step 2: Login

* User logs in using User ID.
* B-Tree search operation retrieves user information.

### Step 3: Ride Booking

* User selects pickup and drop location.
* Request is added to Ride Request Queue.

### Step 4: Capacity Matching

* Available cars are sorted according to empty seats.
* Best matching car is selected.

### Step 5: Route Calculation

* Street network graph is created.
* Dijkstra's Algorithm calculates shortest-time route.

### Step 6: Fare Calculation

Fare = Base Price × Distance × Surge Multiplier

### Step 7: Toll Optimization

* Alternative routes are analyzed.
* Route with minimum toll cost is selected.

### Step 8: Surge Rollback

* Admin can revert the latest surge using Stack Pop operation.

---

## 7. Time and Space Complexity Analysis

| Operation          | Data Structure | Time Complexity |
| ------------------ | -------------- | --------------- |
| User Insert        | B-Tree         | O(log n)        |
| User Search        | B-Tree         | O(log n)        |
| Apply Surge        | Stack Push     | O(1)            |
| Rollback Surge     | Stack Pop      | O(1)            |
| Add Request        | Queue Enqueue  | O(1)            |
| Process Request    | Queue Dequeue  | O(1)            |
| Location Search    | Linear Search  | O(n)            |
| Add Car            | Insertion Sort | O(n²)           |
| Graph Storage      | Adjacency List | O(V + E)        |
| Dijkstra Algorithm | Graph          | O(V²)           |

### Space Complexity

| Structure   | Complexity |
| ----------- | ---------- |
| B-Tree      | O(n)       |
| Stack       | O(n)       |
| Queue       | O(n)       |
| Linked List | O(n)       |
| Array       | O(n)       |
| Graph       | O(V + E)   |

---

## 8. Execution Steps

### Compilation

```bash
g++ PoolRide_Carpool_Engine.cpp -o PoolRide
```

### Run

```bash
./PoolRide
```

### Main Menu

```text
1. Register
2. Login
3. Admin Panel
4. View Design Justification
0. Exit
```

### User Menu

```text
1. Book a Ride
2. View My Profile
3. Add Frequent Route
4. View Current Fare
5. Find Shortest Route
6. Find Cheapest Toll Route
7. Logout
0. Exit
```

---

## 9. Sample Inputs and Outputs

### Sample Input 1

```text
Register User

Name : Nimish
Phone : 9876543210
```

### Sample Output

```text
Registration Successful
User ID : 201
```

---

### Sample Input 2

```text
Book Ride

Pickup  : Andheri
Dropoff : BKC
Seats   : 2
```

### Sample Output

```text
Ride Booked Successfully

Request Token : REQ-1001
Fare : Rs.180
Surge : 1.5x
```

---

### Sample Input 3

```text
Find Shortest Route

Source : Andheri
Destination : Worli
```

### Sample Output

```text
Andheri -> Dadar -> Worli

Total Time : 38 Minutes
```

---

### Sample Input 4

```text
Rollback Surge
```

### Sample Output

```text
Latest Surge Removed Successfully
```

---

## 10. Screenshots

<img width="505" height="231" alt="image" src="https://github.com/user-attachments/assets/e8e14b56-136a-45a5-a5b7-71d4b4b9902a" />

<img width="459" height="302" alt="image" src="https://github.com/user-attachments/assets/08720d66-0170-4dc5-91d6-2c940b475ca1" />

<img width="480" height="193" alt="image" src="https://github.com/user-attachments/assets/45c37079-7c66-48b8-9b05-25588de9c643" />




---

## 11. Results and Observations

1. B-Tree successfully provides efficient user management.
2. Stack enables instant rollback of surge pricing.
3. Queue ensures fair ride request processing.
4. Linked List effectively stores and searches passenger locations.
5. Insertion Sort correctly ranks cars by available seats.
6. Graph representation accurately models city roads.
7. Dijkstra's Algorithm efficiently finds shortest travel-time routes.
8. Toll optimization reduces overall passenger travel costs.
9. The project successfully demonstrates real-world applications of DSA concepts.
10. The system closely resembles the working principles of Uber Pool and Ola Share.

---

## 12. Conclusion

The **PoolRide Digital Ride-Share Passenger Carpool Engine** successfully demonstrates the practical implementation of Data Structures and Algorithms in a real-world transportation system. The project integrates B-Trees, Stacks, Queues, Linked Lists, Arrays, Sorting Algorithms, Graphs, and Dijkstra's Algorithm to solve problems related to user management, ride allocation, route optimization, and fare management.

The system provides efficient ride request handling, dynamic pricing management, shortest path calculation, and toll optimization while maintaining simplicity suitable for academic learning. This project effectively bridges theoretical DSA concepts with practical industry applications and showcases how modern ride-sharing platforms such as Uber Pool and Ola Share can be modeled using fundamental data structures and algorithms.

---
