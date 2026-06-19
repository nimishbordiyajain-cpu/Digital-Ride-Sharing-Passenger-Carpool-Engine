#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define RED     "\033[31m"
#define MAGENTA "\033[35m"

void printHeader(const string& title) {
    cout << "\n" << CYAN << BOLD
         << "╔══════════════════════════════════════════════╗\n"
         << "║  " << left << setw(44) << title << "║\n"
         << "╚══════════════════════════════════════════════╝"
         << RESET << "\n";
}

// ─────────────────────────────────────────────────────────────
//  1. USER DIRECTORY  —  B-Tree  (Module 4: B and B+ Trees)
// ─────────────────────────────────────────────────────────────

struct UserRecord {
    int            userID;
    string         name;
    string         phone;
    vector<string> frequentRoutes;

    string toString() const {
        string routes;
        for (int i = 0; i < (int)frequentRoutes.size(); i++) {
            routes += frequentRoutes[i];
            if (i + 1 < (int)frequentRoutes.size()) routes += ", ";
        }
        return "[ID:" + to_string(userID) + " | " + name +
               " | " + phone + " | Routes: " + routes + "]";
    }
};

struct BTreeNode {
    int                t;
    vector<UserRecord> keys;
    vector<BTreeNode*> children;
    bool               isLeaf;

    BTreeNode(int _t, bool _leaf) : t(_t), isLeaf(_leaf) {}

    UserRecord* search(int id) {
        int i = 0;
        while (i < (int)keys.size() && id > keys[i].userID) i++;
        if (i < (int)keys.size() && keys[i].userID == id)
            return &keys[i];
        if (isLeaf) return nullptr;
        return children[i]->search(id);
    }

    void insertNonFull(UserRecord rec) {
        int i = (int)keys.size() - 1;
        if (isLeaf) {
            keys.push_back(rec);
            while (i >= 0 && keys[i].userID > rec.userID) {
                keys[i + 1] = keys[i]; i--;
            }
            keys[i + 1] = rec;
        } else {
            while (i >= 0 && keys[i].userID > rec.userID) i--;
            i++;
            if ((int)children[i]->keys.size() == 2 * t - 1) {
                splitChild(i, children[i]);
                if (rec.userID > keys[i].userID) i++;
            }
            children[i]->insertNonFull(rec);
        }
    }

    void splitChild(int i, BTreeNode* y) {
        BTreeNode* z = new BTreeNode(t, y->isLeaf);
        int mid = t - 1;
        for (int j = 0; j < t - 1; j++)
            z->keys.push_back(y->keys[j + t]);
        if (!y->isLeaf)
            for (int j = 0; j < t; j++)
                z->children.push_back(y->children[j + t]);
        UserRecord midKey = y->keys[mid];
        y->keys.resize(mid);
        if (!y->isLeaf) y->children.resize(t);
        children.insert(children.begin() + i + 1, z);
        keys.insert(keys.begin() + i, midKey);
    }

    void print(int depth = 0) const {
        string indent(depth * 4, ' ');
        for (int i = 0; i < (int)keys.size(); i++) {
            if (!isLeaf) children[i]->print(depth + 1);
            cout << indent << keys[i].toString() << "\n";
        }
        if (!isLeaf) children[keys.size()]->print(depth + 1);
    }
};

class BTree {
    BTreeNode* root;
    int t;
public:
    BTree(int _t = 3) : root(nullptr), t(_t) {}

    void insert(UserRecord rec) {
        if (!root) {
            root = new BTreeNode(t, true);
            root->keys.push_back(rec);
            return;
        }
        if ((int)root->keys.size() == 2 * t - 1) {
            BTreeNode* s = new BTreeNode(t, false);
            s->children.push_back(root);
            s->splitChild(0, root);
            root = s;
        }
        root->insertNonFull(rec);
    }

    UserRecord* search(int id) {
        return root ? root->search(id) : nullptr;
    }

    void printAll() const {
        if (root) root->print();
        else cout << "  (empty)\n";
    }
};

// ─────────────────────────────────────────────────────────────
//  2. PRICE LOG  —  Stack  (Module 3: Stacks, LIFO principle)
// ─────────────────────────────────────────────────────────────

struct PriceEntry {
    double multiplier;
    string reason;
    string timestamp;
};

class PriceLog {
    stack<PriceEntry> priceStack;
    double basePrice;
public:
    PriceLog(double base = 10.0) : basePrice(base) {}

    void applySurge(double mult, const string& reason, const string& ts) {
        priceStack.push({mult, reason, ts});
        cout << GREEN << "  [PRICE] Surge applied: " << mult
             << "x (" << reason << ") @ " << ts << RESET << "\n";
    }

    void rollback() {
        if (priceStack.empty()) {
            cout << RED << "  [PRICE] Nothing to rollback.\n" << RESET;
            return;
        }
        PriceEntry e = priceStack.top(); priceStack.pop();
        cout << YELLOW << "  [PRICE] Rolled back: " << e.multiplier
             << "x (" << e.reason << ")\n" << RESET;
    }

    double currentMultiplier() const {
        return priceStack.empty() ? 1.0 : priceStack.top().multiplier;
    }

    double effectivePrice(double distKm) const {
        return basePrice * distKm * currentMultiplier();
    }

    void printLog() const {
        stack<PriceEntry> tmp = priceStack;
        cout << "  Current multiplier: " << BOLD
             << currentMultiplier() << "x" << RESET
             << "  (base Rs." << basePrice << "/km)\n";
        cout << "  Surge history (top = latest):\n";
        int i = 1;
        while (!tmp.empty()) {
            cout << "    " << i++ << ". " << tmp.top().multiplier
                 << "x - " << tmp.top().reason
                 << " @ " << tmp.top().timestamp << "\n";
            tmp.pop();
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  3. RIDE REQUEST LINE  —  Queue  (Module 4: Queues, FIFO)
// ─────────────────────────────────────────────────────────────

struct RideRequest {
    int    requestID;
    int    passengerID;
    string pickup;
    string dropoff;
    string submittedAt;
    int    seatsNeeded;
};

class RideRequestLine {
    queue<RideRequest> requestQueue;
    int nextID = 1001;
public:
    void enqueue(int passengerID, const string& pickup,
                 const string& dropoff, const string& time,
                 int seats = 1) {
        RideRequest r{nextID++, passengerID, pickup, dropoff, time, seats};
        requestQueue.push(r);
    }

    RideRequest dequeue() {
        if (requestQueue.empty())
            throw runtime_error("No pending ride requests.");
        RideRequest r = requestQueue.front();
        requestQueue.pop();
        cout << YELLOW << "  [QUEUE] Processing Request #" << r.requestID
             << " (Passenger " << r.passengerID << ")\n" << RESET;
        return r;
    }

    bool isEmpty() const { return requestQueue.empty(); }
    int  size()    const { return (int)requestQueue.size(); }

    void printAll() const {
        queue<RideRequest> tmp = requestQueue;
        cout << "  Pending requests (" << tmp.size() << "):\n";
        while (!tmp.empty()) {
            RideRequest r = tmp.front();
            cout << "    #" << r.requestID << " | Passenger "
                 << r.passengerID << " | " << r.pickup
                 << " -> " << r.dropoff << " | Seats: "
                 << r.seatsNeeded << " | " << r.submittedAt << "\n";
            tmp.pop();
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  4. LOCATION FINDER  —  Singly Linked List with Linear Search
//     (Module 3: Linked Lists; Module 2: Linear Search)
//
//  REPLACED: unordered_map (hash map, not in syllabus)
//  WITH:     Linked list — insertion O(1), search O(n) linear
//            This directly maps to Module 3 (linked list
//            operations) and Module 2 (linear search).
// ─────────────────────────────────────────────────────────────

struct LocationRecord {
    int    passengerID;
    string token;
    double lat, lon;
    string label;
    string updatedAt;
};

struct LocationNode {
    LocationRecord data;
    LocationNode*  next;
    LocationNode(LocationRecord d) : data(d), next(nullptr) {}
};

class LocationFinder {
    LocationNode* head;
public:
    LocationFinder() : head(nullptr) {}

    void update(int id, const string& token,
                double lat, double lon,
                const string& label, const string& ts) {
        // Linear search: check if entry already exists, update it
        LocationNode* cur = head;
        while (cur) {
            if (cur->data.passengerID == id) {
                cur->data.lat = lat;
                cur->data.lon = lon;
                cur->data.label = label;
                cur->data.updatedAt = ts;
                cur->data.token = token;
                return;
            }
            cur = cur->next;
        }
        // Not found: insert at head O(1)
        LocationNode* node = new LocationNode({id, token, lat, lon, label, ts});
        node->next = head;
        head = node;
    }

    // Linear search by passenger ID  (Module 2: Linear Search)
    LocationRecord* findByID(int id) {
        LocationNode* cur = head;
        while (cur) {
            if (cur->data.passengerID == id) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    // Linear search by request token  (Module 2: Linear Search)
    LocationRecord* findByToken(const string& token) {
        LocationNode* cur = head;
        while (cur) {
            if (cur->data.token == token) return &cur->data;
            cur = cur->next;
        }
        return nullptr;
    }

    void print() const {
        cout << "  Passenger Locations (Linked List):\n";
        LocationNode* cur = head;
        while (cur) {
            cout << "    ID " << cur->data.passengerID
                 << " | Token: " << cur->data.token
                 << " | (" << cur->data.lat << ", " << cur->data.lon << ")"
                 << " | " << cur->data.label
                 << " [" << cur->data.updatedAt << "]\n";
            cur = cur->next;
        }
    }

    ~LocationFinder() {
        while (head) {
            LocationNode* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  5. CAPACITY SORTER  —  Array + Insertion Sort
//     (Module 3: Arrays; Module 2: Insertion Sort)
//
//  REPLACED: priority_queue / Max-Heap (not in syllabus)
//  WITH:     Array of cars sorted by empty seats (descending)
//            using Insertion Sort after every addition.
//            Insertion Sort is explicitly in Module 2.
// ─────────────────────────────────────────────────────────────

struct Car {
    int    carID;
    string driverName;
    int    totalSeats;
    int    emptySeats;
    string currentLocation;
};

class CapacitySorter {
    vector<Car> cars;

    // Insertion Sort descending by emptySeats  (Module 2)
    void insertionSort() {
        for (int i = 1; i < (int)cars.size(); i++) {
            Car key = cars[i];
            int j = i - 1;
            while (j >= 0 && cars[j].emptySeats < key.emptySeats) {
                cars[j + 1] = cars[j];
                j--;
            }
            cars[j + 1] = key;
        }
    }

public:
    void addCar(Car c) {
        cars.push_back(c);
        insertionSort();   // keep array sorted after every insert
    }

    Car getBestCar() {
        if (cars.empty()) throw runtime_error("No cars available.");
        Car best = cars[0];          // front is always max after sort
        cars.erase(cars.begin());
        cout << YELLOW << "  [MATCH] Best car: #" << best.carID
             << " (" << best.emptySeats << " empty seats)\n" << RESET;
        return best;
    }

    bool isEmpty() const { return cars.empty(); }

    void print() const {
        cout << "  Available cars (sorted by empty seats, desc):\n";
        for (int i = 0; i < (int)cars.size(); i++) {
            cout << "    " << (i + 1) << ". Car #" << cars[i].carID
                 << " | Driver: " << cars[i].driverName
                 << " | Empty: " << cars[i].emptySeats
                 << "/" << cars[i].totalSeats
                 << " | At: " << cars[i].currentLocation << "\n";
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  6. STREET NETWORK  —  Weighted Graph (Adjacency List)
//     (Module 5: Graphs — directed vs undirected, representations)
// ─────────────────────────────────────────────────────────────

struct Edge {
    int    to;
    double timeMinutes;
    double tollRupees;
};

struct GraphNode {
    string name;
};

class StreetNetwork {
public:
    vector<GraphNode>    nodes;
    vector<vector<Edge>> adj;

    int addNode(const string& name) {
        nodes.push_back({name});
        adj.push_back({});
        return (int)nodes.size() - 1;
    }

    void addRoad(int u, int v, double time, double toll,
                 bool bidirectional = true) {
        adj[u].push_back({v, time, toll});
        if (bidirectional) adj[v].push_back({u, time, toll});
    }

    int nodeCount() const { return (int)nodes.size(); }

    void print() const {
        cout << "  Street Network (" << nodes.size() << " nodes):\n";
        for (int u = 0; u < (int)nodes.size(); u++) {
            cout << "    [" << u << "] " << nodes[u].name << "\n";
            for (int i = 0; i < (int)adj[u].size(); i++)
                cout << "         -> " << nodes[adj[u][i].to].name
                     << "  (time=" << adj[u][i].timeMinutes
                     << " min, toll=Rs." << adj[u][i].tollRupees << ")\n";
        }
    }
};

// ─────────────────────────────────────────────────────────────
//  7 & 8. CARPOOL ROUTE + TOLL OPTIMIZER  —  Dijkstra
//     (Module 5: Graph Applications — Shortest path, Dijkstra's)
// ─────────────────────────────────────────────────────────────

class CarpoolRouter {
    const StreetNetwork& net;

    // Dijkstra's shortest path algorithm
    // useToll=false -> minimise time, useToll=true -> minimise toll
    void dijkstra(int src, bool useToll,
                  vector<double>& dist, vector<int>& prev) const {
        int n = net.nodeCount();
        dist.assign(n, numeric_limits<double>::infinity());
        prev.assign(n, -1);
        vector<bool> visited(n, false);
        dist[src] = 0;

        for (int iter = 0; iter < n; iter++) {
            // Pick unvisited node with smallest distance (linear scan)
            int u = -1;
            for (int i = 0; i < n; i++)
                if (!visited[i] && (u == -1 || dist[i] < dist[u]))
                    u = i;
            if (u == -1 || dist[u] == numeric_limits<double>::infinity())
                break;
            visited[u] = true;

            // Relax all neighbours
            for (int i = 0; i < (int)net.adj[u].size(); i++) {
                int    v = net.adj[u][i].to;
                double w = useToll ? net.adj[u][i].tollRupees
                                   : net.adj[u][i].timeMinutes;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    prev[v] = u;
                }
            }
        }
    }

    vector<int> buildPath(int src, int dst,
                          const vector<int>& prev) const {
        vector<int> path;
        for (int at = dst; at != -1; at = prev[at])
            path.push_back(at);
        reverse(path.begin(), path.end());
        if (path.empty() || path[0] != src) return {};
        return path;
    }

public:
    CarpoolRouter(const StreetNetwork& n) : net(n) {}

    void findMinTimePath(const vector<int>& stops) const {
        printHeader("7. CARPOOL ROUTE (Min-Time Multi-Stop)");
        double totalTime = 0;
        for (int i = 0; i + 1 < (int)stops.size(); i++) {
            int src = stops[i], dst = stops[i + 1];
            vector<double> dist;
            vector<int>    prev;
            dijkstra(src, false, dist, prev);
            vector<int> path = buildPath(src, dst, prev);
            cout << "  Leg " << (i + 1) << ": "
                 << net.nodes[src].name << " -> "
                 << net.nodes[dst].name << "\n";
            cout << "    Path: ";
            for (int j = 0; j < (int)path.size(); j++) {
                cout << net.nodes[path[j]].name;
                if (j + 1 < (int)path.size()) cout << " -> ";
            }
            cout << "\n    Time: " << dist[dst] << " min\n";
            totalTime += dist[dst];
        }
        cout << BOLD << GREEN
             << "  Total journey time: " << totalTime << " min\n"
             << RESET;
    }

    void findMinTollPath(int src, int dst) const {
        printHeader("8. TOLL OPTIMIZER (Min-Toll Path)");
        vector<double> dist;
        vector<int>    prev;
        dijkstra(src, true, dist, prev);
        vector<int> path = buildPath(src, dst, prev);
        cout << "  " << net.nodes[src].name << " -> "
             << net.nodes[dst].name << "\n";
        cout << "  Path: ";
        for (int i = 0; i < (int)path.size(); i++) {
            cout << net.nodes[path[i]].name;
            if (i + 1 < (int)path.size()) cout << " -> ";
        }
        cout << "\n";
        cout << BOLD << GREEN
             << "  Minimum toll cost: Rs." << dist[dst] << "\n"
             << RESET;
    }
};

// ─────────────────────────────────────────────────────────────
//  DESIGN JUSTIFICATION  (required by problem statement)
// ─────────────────────────────────────────────────────────────

void printJustification() {
    printHeader("DESIGN JUSTIFICATION");
    cout <<
    "  Feature           DS / Algo          Syllabus Module\n"
    "  ─────────────────────────────────────────────────────────────────────\n"
    "  User Directory    B-Tree (t=3)        Module 4: B and B+ Trees\n"
    "                    Insert O(log_t n)   Disk-optimised multi-way search\n"
    "                    Search O(log_t n)   tree. Fewer disk reads than BST.\n\n"
    "  Price Log         Stack (LIFO)        Module 3: Stacks\n"
    "                    Push/Pop O(1)       Instant surge rollback. Most\n"
    "                                        recent change reversed first.\n\n"
    "  Ride Request Line Queue (FIFO)        Module 4: Queues\n"
    "                    Enq/Deq O(1)        First request submitted is first\n"
    "                                        matched - fair ordering.\n\n"
    "  Location Finder   Singly Linked List  Module 3: Linked Lists\n"
    "                  + Linear Search O(n)  Module 2: Linear Search\n"
    "                                        Insert at head O(1). Search by\n"
    "                                        passenger ID or request token.\n\n"
    "  Capacity Sorter   Array +             Module 3: Arrays\n"
    "                    Insertion Sort      Module 2: Insertion Sort\n"
    "                    O(n^2) worst case   Cars kept sorted by empty seats.\n"
    "                                        Best car always at index 0.\n\n"
    "  Street Network    Weighted Graph      Module 5: Graphs\n"
    "                    (Adjacency List)    Intersections = nodes, roads =\n"
    "                    O(V+E) space        weighted edges (time + toll).\n\n"
    "  Carpool Route     Dijkstra's Algo     Module 5: Graph Applications\n"
    "                    O(V^2) time         Min-time multi-stop path.\n"
    "                                        Uber Pool uses this for ETA.\n\n"
    "  Toll Optimizer    Dijkstra's Algo     Module 5: Graph Applications\n"
    "                    O(V^2) time         Same algorithm, toll as weight.\n"
    "                                        Finds cheapest-toll route.\n\n"
    "  Real-world mapping (Uber Pool / Ola Share):\n"
    "  - B-Tree:          Uber/Ola store millions of profiles on disk.\n"
    "                     B-Tree height O(log_t n) means far fewer disk seeks.\n"
    "  - Stack:           Ola Share reverts surge pricing when demand drops.\n"
    "  - Queue:           Uber Pool serves ride requests in submission order.\n"
    "  - Linked List:     Driver/passenger location list updated every few sec.\n"
    "  - Insertion Sort:  Cars re-ranked by available seats after each booking.\n"
    "  - Graph+Dijkstra:  Google Maps / Uber use Dijkstra for shortest routes.\n";
}

// ─────────────────────────────────────────────────────────────
//  HELPERS
// ─────────────────────────────────────────────────────────────

void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string currentTime() {
    // Returns a simple session timestamp string
    static int tick = 0;
    int h = 8 + tick / 60;
    int m = tick % 60;
    tick += 3;
    string hh = (h < 10 ? "0" : "") + to_string(h);
    string mm = (m < 10 ? "0" : "") + to_string(m);
    return hh + ":" + mm;
}

// ─────────────────────────────────────────────────────────────
//  MAIN — Interactive Menu
// ─────────────────────────────────────────────────────────────

int main() {

    // ── System setup (runs once at startup, hidden from user) ──
    // B-Tree user database
    BTree userDB(3);
    int   nextUserID = 201;   // auto-increment from 201 upward

    // Price log (base Rs.12/km)
    PriceLog priceLog(12.0);
    priceLog.applySurge(1.5, "Evening Rush Hour", "17:30");  // default surge

    // Ride request queue
    RideRequestLine requestLine;
    int             nextReqID = 1;

    // Location finder (linked list)
    LocationFinder locFinder;

    // Capacity sorter (array + insertion sort) — pre-load some cars
    CapacitySorter sorter;
    sorter.addCar({301, "Vikas D.", 4, 4, "Andheri"});
    sorter.addCar({302, "Meera R.", 6, 6, "Dadar"});
    sorter.addCar({303, "Kiran S.", 4, 3, "Thane"});
    sorter.addCar({304, "Aryan P.", 6, 5, "Vashi"});

    // Street network (graph)
    StreetNetwork net;
    int ND_AND = net.addNode("Andheri");
    int ND_JVL = net.addNode("JVLR Junction");
    int ND_POW = net.addNode("Powai Gate");
    int ND_BKC = net.addNode("BKC");
    int ND_DAD = net.addNode("Dadar");
    int ND_SEA = net.addNode("Worli Sea Link");
    int ND_WOR = net.addNode("Worli");
    net.addRoad(ND_AND, ND_JVL,  8,  0);
    net.addRoad(ND_AND, ND_DAD, 20,  0);
    net.addRoad(ND_JVL, ND_POW,  5,  0);
    net.addRoad(ND_JVL, ND_BKC, 15, 30);
    net.addRoad(ND_POW, ND_BKC, 12,  0);
    net.addRoad(ND_BKC, ND_DAD, 10, 20);
    net.addRoad(ND_DAD, ND_SEA,  6,  0);
    net.addRoad(ND_SEA, ND_WOR,  8, 75);
    net.addRoad(ND_DAD, ND_WOR, 18,  0);

    CarpoolRouter router(net);

    // ── Welcome banner ──
    cout << BOLD << MAGENTA
         << "\n╔═══════════════════════════════════════════════════════╗\n"
         << "║       PoolRide  —  Carpool Engine                     ║\n"
         << "║       ITM Skills University  |  DSA-1 Project         ║\n"
         << "╚═══════════════════════════════════════════════════════╝\n"
         << RESET;

    int loggedInID = -1;   // -1 means no one is logged in

    // ── Main loop ──
    while (true) {

        // ── TOP-LEVEL MENU ──
        if (loggedInID == -1) {
            cout << "\n" << BOLD << "  MAIN MENU\n" << RESET
                 << "  1. Register\n"
                 << "  2. Login\n"
                 << "  3. Admin Panel\n"
                 << "  4. View Design Justification\n"
                 << "  0. Exit\n"
                 << "  Choice: ";
        } else {
            UserRecord* me = userDB.search(loggedInID);
            cout << "\n" << BOLD << "  WELCOME, "
                 << (me ? me->name : "User")
                 << "  (ID: " << loggedInID << ")\n" << RESET
                 << "  1. Book a Ride\n"
                 << "  2. View My Profile\n"
                 << "  3. Add Frequent Route\n"
                 << "  4. View Current Fare\n"
                 << "  5. Find Shortest Route (Map)\n"
                 << "  6. Find Cheapest-Toll Route\n"
                 << "  7. Logout\n"
                 << "  0. Exit\n"
                 << "  Choice: ";
        }

        int choice;
        cin >> choice;
        clearInput();

        // ── NOT LOGGED IN ──
        if (loggedInID == -1) {
            if (choice == 0) {
                cout << GREEN << "\n  Goodbye!\n" << RESET;
                break;
            }

            // ── REGISTER ──
            else if (choice == 1) {
                printHeader("REGISTER NEW USER");
                UserRecord rec;
                rec.userID = nextUserID++;

                cout << "  Enter your name    : ";
                getline(cin, rec.name);
                cout << "  Enter phone number : ";
                getline(cin, rec.phone);

                userDB.insert(rec);
                cout << GREEN
                     << "\n  Registration successful!\n"
                     << "  Your User ID is: " << BOLD << rec.userID << RESET << GREEN
                     << "  (save this to login)\n" << RESET;
            }

            // ── LOGIN ──
            else if (choice == 2) {
                printHeader("LOGIN");
                cout << "  Enter your User ID: ";
                int id; cin >> id; clearInput();
                UserRecord* rec = userDB.search(id);
                if (!rec) {
                    cout << RED << "  User ID not found. Please register first.\n" << RESET;
                } else {
                    loggedInID = id;
                    cout << GREEN << "  Login successful! Welcome, "
                         << rec->name << ".\n" << RESET;
                }
            }

            // ── ADMIN PANEL ──
            else if (choice == 3) {
                printHeader("ADMIN PANEL");
                cout << "  1. View all registered users\n"
                     << "  2. View ride request queue\n"
                     << "  3. Process next ride request\n"
                     << "  4. Apply price surge\n"
                     << "  5. Rollback last surge\n"
                     << "  6. View available cars\n"
                     << "  7. View street network\n"
                     << "  Choice: ";
                int ac; cin >> ac; clearInput();

                if (ac == 1) {
                    printHeader("ALL REGISTERED USERS");
                    userDB.printAll();
                }
                else if (ac == 2) {
                    printHeader("RIDE REQUEST QUEUE");
                    requestLine.printAll();
                }
                else if (ac == 3) {
                    printHeader("PROCESS NEXT REQUEST");
                    if (requestLine.isEmpty()) {
                        cout << YELLOW << "  No pending requests.\n" << RESET;
                    } else {
                        RideRequest r = requestLine.dequeue();
                        if (!sorter.isEmpty()) {
                            Car c = sorter.getBestCar();
                            cout << GREEN
                                 << "  Matched Passenger " << r.passengerID
                                 << " with Car #" << c.carID
                                 << " (Driver: " << c.driverName << ")\n"
                                 << "  Route: " << r.pickup
                                 << " -> " << r.dropoff << "\n"
                                 << "  Seats needed: " << r.seatsNeeded << "\n"
                                 << RESET;
                            // update remaining seats and re-add car if seats left
                            c.emptySeats -= r.seatsNeeded;
                            if (c.emptySeats > 0) sorter.addCar(c);
                        } else {
                            cout << RED << "  No cars available right now.\n" << RESET;
                        }
                    }
                }
                else if (ac == 4) {
                    printHeader("APPLY PRICE SURGE");
                    cout << "  Enter surge multiplier (e.g. 1.5): ";
                    double mult; cin >> mult; clearInput();
                    cout << "  Enter reason: ";
                    string reason; getline(cin, reason);
                    priceLog.applySurge(mult, reason, currentTime());
                }
                else if (ac == 5) {
                    printHeader("ROLLBACK SURGE");
                    priceLog.rollback();
                    priceLog.printLog();
                }
                else if (ac == 6) {
                    printHeader("AVAILABLE CARS");
                    sorter.print();
                }
                else if (ac == 7) {
                    printHeader("STREET NETWORK");
                    net.print();
                }
            }

            // ── JUSTIFICATION ──
            else if (choice == 4) {
                printJustification();
            }

        }

        // ── LOGGED IN ──
        else {
            if (choice == 0) {
                cout << GREEN << "\n  Goodbye!\n" << RESET;
                break;
            }

            // ── BOOK A RIDE ──
            else if (choice == 1) {
                printHeader("BOOK A RIDE");

                // Show available locations
                cout << "  Available locations:\n";
                for (int i = 0; i < net.nodeCount(); i++)
                    cout << "    " << (i + 1) << ". " << net.nodes[i].name << "\n";

                cout << "\n  Enter pickup location number  : ";
                int pu; cin >> pu; clearInput();
                cout << "  Enter dropoff location number : ";
                int dof; cin >> dof; clearInput();

                if (pu < 1 || pu > net.nodeCount() ||
                    dof < 1 || dof > net.nodeCount() || pu == dof) {
                    cout << RED << "  Invalid locations. Please try again.\n" << RESET;
                } else {
                    string pickup  = net.nodes[pu  - 1].name;
                    string dropoff = net.nodes[dof - 1].name;

                    cout << "  Enter seats needed (1-4)      : ";
                    int seats; cin >> seats; clearInput();
                    if (seats < 1 || seats > 4) seats = 1;

                    string token = "REQ-" + to_string(1000 + nextReqID);
                    string ts    = currentTime();

                    // Add to queue
                    requestLine.enqueue(loggedInID, pickup, dropoff, ts, seats);

                    // Update location in linked list
                    locFinder.update(loggedInID, token, 0.0, 0.0, pickup, ts);

                    // Show estimated fare (rough distance = legs * 5 km)
                    double estDist = 5.0 * seats;
                    cout << GREEN
                         << "\n  Ride booked successfully!\n"
                         << "  Request Token : " << token << "\n"
                         << "  From          : " << pickup << "\n"
                         << "  To            : " << dropoff << "\n"
                         << "  Seats         : " << seats << "\n"
                         << "  Est. Fare     : Rs."
                         << priceLog.effectivePrice(estDist) << "\n"
                         << "  Surge         : " << priceLog.currentMultiplier() << "x\n"
                         << RESET;

                    // Show fastest route
                    cout << "\n  Fastest route (Dijkstra):\n";
                    int src = pu - 1, dst = dof - 1;
                    vector<double> dist;
                    vector<int>    prev;
                    // reuse dijkstra via router by calling findMinTimePath
                    router.findMinTimePath({src, dst});

                    nextReqID++;
                }
            }

            // ── VIEW PROFILE ──
            else if (choice == 2) {
                printHeader("MY PROFILE");
                UserRecord* rec = userDB.search(loggedInID);
                if (rec) {
                    cout << "  Name   : " << rec->name   << "\n"
                         << "  Phone  : " << rec->phone  << "\n"
                         << "  UserID : " << rec->userID << "\n"
                         << "  Frequent Routes:\n";
                    if (rec->frequentRoutes.empty()) {
                        cout << "    (none saved)\n";
                    } else {
                        for (int i = 0; i < (int)rec->frequentRoutes.size(); i++)
                            cout << "    " << (i+1) << ". " << rec->frequentRoutes[i] << "\n";
                    }
                }
                // Show location if available
                LocationRecord* lr = locFinder.findByID(loggedInID);
                if (lr) cout << "  Last seen: " << lr->label << " [" << lr->updatedAt << "]\n";
            }

            // ── ADD FREQUENT ROUTE ──
            else if (choice == 3) {
                printHeader("ADD FREQUENT ROUTE");
                cout << "  Available locations:\n";
                for (int i = 0; i < net.nodeCount(); i++)
                    cout << "    " << (i+1) << ". " << net.nodes[i].name << "\n";
                cout << "  From (number): "; int f; cin >> f; clearInput();
                cout << "  To   (number): "; int t; cin >> t; clearInput();
                if (f < 1 || f > net.nodeCount() || t < 1 || t > net.nodeCount() || f == t) {
                    cout << RED << "  Invalid selection.\n" << RESET;
                } else {
                    UserRecord* rec = userDB.search(loggedInID);
                    if (rec) {
                        string route = net.nodes[f-1].name + " -> " + net.nodes[t-1].name;
                        rec->frequentRoutes.push_back(route);
                        cout << GREEN << "  Saved: " << route << "\n" << RESET;
                    }
                }
            }

            // ── VIEW CURRENT FARE ──
            else if (choice == 4) {
                printHeader("CURRENT FARE INFO");
                priceLog.printLog();
                cout << "\n  Sample fares:\n"
                     << "    3 km  -> Rs." << priceLog.effectivePrice(3)  << "\n"
                     << "    7 km  -> Rs." << priceLog.effectivePrice(7)  << "\n"
                     << "    12 km -> Rs." << priceLog.effectivePrice(12) << "\n";
            }

            // ── SHORTEST ROUTE ──
            else if (choice == 5) {
                printHeader("FIND SHORTEST ROUTE (Min-Time)");
                cout << "  Available locations:\n";
                for (int i = 0; i < net.nodeCount(); i++)
                    cout << "    " << (i+1) << ". " << net.nodes[i].name << "\n";
                cout << "  From (number): "; int f; cin >> f; clearInput();
                cout << "  To   (number): "; int t; cin >> t; clearInput();
                if (f < 1 || f > net.nodeCount() || t < 1 || t > net.nodeCount() || f == t) {
                    cout << RED << "  Invalid selection.\n" << RESET;
                } else {
                    router.findMinTimePath({f-1, t-1});
                }
            }

            // ── CHEAPEST TOLL ROUTE ──
            else if (choice == 6) {
                printHeader("FIND CHEAPEST-TOLL ROUTE");
                cout << "  Available locations:\n";
                for (int i = 0; i < net.nodeCount(); i++)
                    cout << "    " << (i+1) << ". " << net.nodes[i].name << "\n";
                cout << "  From (number): "; int f; cin >> f; clearInput();
                cout << "  To   (number): "; int t; cin >> t; clearInput();
                if (f < 1 || f > net.nodeCount() || t < 1 || t > net.nodeCount() || f == t) {
                    cout << RED << "  Invalid selection.\n" << RESET;
                } else {
                    router.findMinTollPath(f-1, t-1);
                }
            }

            // ── LOGOUT ──
            else if (choice == 7) {
                cout << YELLOW << "  Logged out.\n" << RESET;
                loggedInID = -1;
            }
        }
    }

    return 0;
}
