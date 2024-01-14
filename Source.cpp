#include <iostream>
#include <queue>
#include <climits>
#include <map>
#include <set>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono> // For measuring algorithm runtimes

using namespace std;

const int MAX_VERTICES = 20; // Total number of students and teachers (assuming 10 students and 10 teachers)
const int INF = INT_MAX;
map<string, long long> algorithmRuntimes; // Stores runtimes for algorithms

struct Node
{
    int vertex, weight;
    Node *next;

    Node(int v, int w) : vertex(v), weight(w), next(nullptr) {}
};

struct Edge
{
    int src, dest, weight;
};

struct Subset
{
    int parent, rank;
};

struct Teacher
{
    string name;
    Teacher() {}
    Teacher(const string &n) : name(n) {}
};

struct Workshop
{
    string name;
    Workshop() {}
    Workshop(const string &n) : name(n) {}
};

class Graph
{
    int V;      // Number of vertices
    Node **adj; // Adjacency list
    Edge *edges;
    int edgeCount;

public:
    map<int, string> studentNames;
    Teacher *teachers;
    map<string, string> teacherStudentMap; // Map to store teacher-student relationships
    Workshop *workshops;
    map<int, string> workshopSchedule;             // Map to store workshop schedule for students
    map<int, string> workshopEnrollments;          // Map to store workshop enrollments for students
    map<int, set<int>> studentWorkshopEnrollments; // Map to store workshop enrollments for students
    Graph(int numStudents, int numTeachers) : V(numStudents + numTeachers), edgeCount(0)
    {
        adj = new Node *[V];
        edges = new Edge[50];
        for (int i = 0; i < V; ++i)
            adj[i] = nullptr;
        initializeStudentNames();
        teachers = new Teacher[numTeachers];
        initializeTeacherNames();
        workshops = new Workshop[5]; // Assuming 5 workshops
        initializeWorkshops();
    }

    ~Graph()
    {
        for (int i = 0; i < V; ++i)
        {
            Node *temp = adj[i];
            while (temp != nullptr)
            {
                Node *next = temp->next;
                delete temp;
                temp = next;
            }
        }
        delete[] adj;
        delete[] edges;
        delete[] teachers;
        delete[] workshops;
    }

    void initializeStudentNames()
    {
        studentNames[0] = "Mati";
        studentNames[1] = "Usman";
        studentNames[2] = "Bilal";
        studentNames[3] = "Tauseef";
        studentNames[4] = "Ibrahim";
        studentNames[5] = "Eman";
        studentNames[6] = "Abdurehman";
        studentNames[7] = "Abdullah";
        studentNames[8] = "Umair";
        studentNames[9] = "Cheetah";
    }

    void initializeTeacherNames()
    {
        teachers[0] = Teacher("Sir_Khwaja");
        teachers[1] = Teacher("Sir_Hassan");
        teachers[2] = Teacher("Sir_Atif");
        teachers[3] = Teacher("Maam_Behjat");
        teachers[4] = Teacher("Sir_Zaheer");
    }

    void initializeWorkshops()
    {
        workshops[0] = Workshop("Unity");
        workshops[1] = Workshop("Hackathon");
        workshops[2] = Workshop("CodeJack");
        workshops[3] = Workshop("Speed_Programming");
        workshops[4] = Workshop("BlockChain");
    }

    void printGraph()
    {
        for (int v = 0; v < V; ++v)
        {
            if (v < 10)
            {
                cout << "Student " << studentNames[v] << " has friends: \n";
            }
            else
            {
                cout << "Teacher " << teachers[v - 10].name << " has connections: \n";
                if (teacherStudentMap.find(teachers[v - 10].name) != teacherStudentMap.end())
                {
                    cout << "\tTeaches: " << teacherStudentMap[teachers[v - 10].name] << endl;
                }
            }

            Node *crawl = adj[v];
            while (crawl)
            {
                if (crawl->vertex < 10)
                    cout << "\t" << studentNames[crawl->vertex] << " (Strength: " << crawl->weight << ")\n";
                else
                    cout << "\t" << teachers[crawl->vertex - 10].name << " (Strength: " << crawl->weight << ")\n";
                crawl = crawl->next;
            }
            cout << endl;
        }
    }

    void addEdge(int u, int v, int w)
    {
        Node *node = new Node(v, w);
        node->next = adj[u];
        adj[u] = node;

        node = new Node(u, w);
        node->next = adj[v];
        adj[v] = node;

        edges[edgeCount++] = {u, v, w};
    }

    void dijkstra(int src);
    int find(Subset subsets[], int i);
    void Union(Subset subsets[], int x, int y);
    void kruskalMST();
    void BFS(int startVertex, bool showTeacherConnections);
    void scheduleWorkshop(int studentIndex, int workshopIndex);
    void printWorkshopSchedule();
    void searchByName(const string &name);
    void searchStudentsInWorkshop(int workshopIndex); // Added for workshop search
    void DFSStudentsInWorkshop(int v, int workshopIndex, bool visited[]);
    void exportGraphData();
};

int Graph::find(Subset subsets[], int i)
{
    if (subsets[i].parent != i)
    {
        subsets[i].parent = find(subsets, subsets[i].parent);
    }
    return subsets[i].parent;
}

void Graph::exportGraphData()
{
    ofstream outFile("graph_data.json");
    outFile << "{" << endl;
    outFile << "\"nodes\": [" << endl;
    for (int i = 0; i < V; ++i)
    {
        outFile << "{ \"id\": " << i;
        outFile << ", \"label\": \"" << ((i < 10) ? studentNames[i] : teachers[i - 10].name) << "\"}";
        if (i < V - 1)
            outFile << ",";
        outFile << endl;
    }
    outFile << "]," << endl;

    outFile << "\"edges\": [" << endl;
    for (int i = 0; i < edgeCount; ++i)
    {
        outFile << "{ \"from\": " << edges[i].src << ", \"to\": " << edges[i].dest << ", \"weight\": " << edges[i].weight << "}";
        if (i < edgeCount - 1)
            outFile << ",";
        outFile << endl;
    }
    outFile << "]" << endl;
    outFile << "}" << endl;
    outFile.close();
}

void Graph::Union(Subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
    {
        subsets[xroot].parent = yroot;
    }
    else if (subsets[xroot].rank > subsets[yroot].rank)
    {
        subsets[yroot].parent = xroot;
    }
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

void Graph::kruskalMST()
{

    auto start = chrono::high_resolution_clock::now();
    Edge result[MAX_VERTICES]; // Store result
    int e = 0;                 // Index variable for result

    // Step 1: Sort all the edges in non-decreasing order of their weight
    sort(edges, edges + edgeCount, [](Edge a, Edge b)
         { return a.weight < b.weight; });

    Subset *subsets = new Subset[V];
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    int i = 0; // Index used to pick next edge
    while (e < V - 1 && i < edgeCount)
    {
        Edge next_edge = edges[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }

    // Print the result
    for (i = 0; i < e; ++i)
    {
        cout << studentNames[result[i].src] << " -- " << studentNames[result[i].dest] << " == " << result[i].weight << endl;
    }

    delete[] subsets;

    auto end = chrono::high_resolution_clock::now();
    algorithmRuntimes["Kruskal"] += chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

void Graph::dijkstra(int src)
{
    auto start = chrono::high_resolution_clock::now();

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(V, INF);

    pq.push(make_pair(0, src));
    dist[src] = 0;

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        Node *node = adj[u];
        while (node != nullptr)
        {
            int v = node->vertex;
            int weight = node->weight;

            if (dist[v] > dist[u] + weight)
            {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
            node = node->next;
        }
    }

    if (src < 10)
        cout << "Shortest path distances from " << studentNames[src] << ":\n";
    else
        cout << "Shortest path distances from " << teachers[src - 10].name << ":\n";

    for (int i = 0; i < V; ++i)
    {
        if (i < 10)
            cout << "To " << studentNames[i] << ":\t" << dist[i] << " units\n";
        else
            cout << "To " << teachers[i - 10].name << ":\t" << dist[i] << " units\n";
    }

    auto end = chrono::high_resolution_clock::now();
    algorithmRuntimes["Dijkstra"] += chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

void Graph::BFS(int startVertex, bool showTeacherConnections)
{
    auto start = chrono::high_resolution_clock::now();

    // Create an array to keep track of visited students and teachers
    bool *visited = new bool[V];
    for (int i = 0; i < V; ++i)
    {
        visited[i] = false;
    }

    // Create a queue for BFS
    queue<int> bfsQueue;

    // Mark the current student or teacher as visited and enqueue them
    visited[startVertex] = true;
    bfsQueue.push(startVertex);

    if (startVertex < 10)
        cout << "Friends of " << studentNames[startVertex] << ":\n";
    else
    {
        if (showTeacherConnections)
            cout << "Connections of " << teachers[startVertex - 10].name << ":\n";
        else
            cout << "Friends of " << teachers[startVertex - 10].name << ":\n";
    }

    while (!bfsQueue.empty())
    {
        int currentVertex = bfsQueue.front();
        bfsQueue.pop();

        Node *node = adj[currentVertex];
        while (node != nullptr)
        {
            int neighbor = node->vertex;

            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                bfsQueue.push(neighbor);

                if (neighbor < 10)
                {
                    cout << " - " << studentNames[neighbor] << endl;
                    // Check if the current teacher teaches this student
                    if (!showTeacherConnections && currentVertex >= 10)
                    {
                        string teacherName = teachers[currentVertex - 10].name;
                        teacherStudentMap[teacherName] += studentNames[neighbor] + " ";
                    }
                }
                else
                {
                    if (showTeacherConnections)
                        cout << " - " << teachers[neighbor - 10].name << endl;
                }
            }

            node = node->next;
        }
    }

    delete[] visited;

    auto end = chrono::high_resolution_clock::now();
    algorithmRuntimes["BFS"] += chrono::duration_cast<chrono::milliseconds>(end - start).count();
}

void Graph::scheduleWorkshop(int studentIndex, int workshopIndex)
{
    if (workshopIndex >= 0 && workshopIndex < 5) // Assuming 5 workshops
    {
        string studentName = studentNames[studentIndex];
        string workshopName = workshops[workshopIndex].name;
        workshopSchedule[studentIndex] = workshopName;
        workshopEnrollments[studentIndex] = workshopName;

        // Update student workshop enrollments
        studentWorkshopEnrollments[studentIndex].insert(workshopIndex);

        cout << "Scheduled " << studentName << " for " << workshopName << endl;
    }
    else
    {
        cout << "Invalid workshop index. Please select a valid workshop." << endl;
    }
}

void Graph::printWorkshopSchedule()
{
    cout << "Workshop Schedule:\n";
    for (const auto &pair : workshopSchedule)
    {
        cout << studentNames[pair.first] << " is scheduled for " << pair.second << endl;
    }
}

void Graph::searchByName(const string &name)
{
    int startIndex = -1;
    for (const auto &pair : studentNames)
    {
        if (pair.second == name)
        {
            startIndex = pair.first;
            break;
        }
    }

    if (startIndex == -1)
    {
        for (int i = 0; i < 10; ++i) // Assuming 10 teachers
        {
            if (teachers[i].name == name)
            {
                startIndex = i + 10; // Offset to represent teachers
                break;
            }
        }
    }

    if (startIndex != -1)
    {
        // When searching for a student, show friends only
        // When searching for a teacher, show connections (including student friends)
        BFS(startIndex, (startIndex < 10) ? false : true);
    }
    else
    {
        cout << "Name not found. Please enter a valid name." << endl;
    }
}
void Graph::DFSStudentsInWorkshop(int v, int workshopIndex, bool visited[])
{
    static auto start = chrono::high_resolution_clock::now();
    static bool firstCall = true; // To capture the start time on first call

    visited[v] = true;

    // Check if the current student is enrolled in the desired workshop
    if (studentWorkshopEnrollments.find(v) != studentWorkshopEnrollments.end())
    {
        const set<int> &enrolledWorkshops = studentWorkshopEnrollments[v];
        if (enrolledWorkshops.count(workshopIndex) > 0)
        {
            cout << "Student " << studentNames[v] << " is enrolled in workshop " << workshops[workshopIndex].name << endl;
        }
    }

    Node *node = adj[v];
    while (node != nullptr)
    {
        int neighbor = node->vertex;
        if (!visited[neighbor])
        {
            DFSStudentsInWorkshop(neighbor, workshopIndex, visited);
        }
        node = node->next;
    }

    if (firstCall)
    {
        firstCall = false;
        auto end = chrono::high_resolution_clock::now();
        algorithmRuntimes["DFS"] += chrono::duration_cast<chrono::milliseconds>(end - start).count();
    }
}

void Graph::searchStudentsInWorkshop(int workshopIndex)
{
    if (workshopIndex >= 0 && workshopIndex < 5) // Assuming 5 workshops
    {
        string workshopName = workshops[workshopIndex].name;
        cout << "Students enrolled in workshop '" << workshopName << "':\n";

        // Create an array to keep track of visited students and teachers
        bool *visited = new bool[V];
        for (int i = 0; i < V; ++i)
        {
            visited[i] = false;
        }

        for (int v = 0; v < V; ++v)
        {
            if (!visited[v] && v < 10) // Only start DFS from students
            {
                DFSStudentsInWorkshop(v, workshopIndex, visited);
            }
        }

        delete[] visited;
    }
    else
    {
        cout << "Invalid workshop index. Please select a valid workshop." << endl;
    }
}

int main()
{
    Graph g(10, 5); // 10 students and 5 teachers

    g.addEdge(0, 1, 1); // Mati and Usman
    g.addEdge(0, 2, 4); // Mati and Bilal
    g.addEdge(1, 3, 2); // Usman and Tauseef
    g.addEdge(2, 3, 5); // Bilal and Tauseef
    g.addEdge(3, 4, 1); // Tauseef and Ibrahim
    g.addEdge(4, 5, 3); // Ibrahim and Eman
    // g.addEdge(5, 6, 2);  // Eman and Abdurehman
    g.addEdge(6, 7, 4);  // Abdurehman and Abdullah
    g.addEdge(7, 8, 1);  // Abdullah and Ushna
    g.addEdge(8, 9, 2);  // Ushna and Anum
    g.addEdge(0, 10, 2); // Mati (student) and Teacher1 (teacher)
    g.addEdge(5, 11, 1); // Eman (student) and Teacher2 (teacher)
    g.addEdge(1, 12, 3); // Usman (student) and Teacher3 (teacher)
    g.addEdge(3, 13, 2); // Tauseef (student) and Teacher4 (teacher)
    g.addEdge(8, 14, 4); // Ushna (student) and Teacher5 (teacher)
    // add edges for teachers
    g.addEdge(6, 11, 1); // Abdurehman and Teacher2
    g.addEdge(4, 0, 2);  // Ibrahim and Mati
    g.addEdge(10, 9, 3); // Teacher1 and Anum
    bool exitProgram = false;
    g.printGraph();

    while (!exitProgram)
    {
        int choice;
        cout << "Menu:\n";
        cout << "1. Shortest Distance\n";
        cout << "2. MST (Minimum Spanning Tree)\n";
        cout << "3. Search by Name\n";
        cout << "4. Schedule Workshop\n";
        cout << "5. Print Workshop Schedule\n";
        cout << "6. Search Students in Workshop\n";
        cout << "7. Export Graph Data\n";
        cout << "8. Print Algorithm Runtimes\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            string startPerson;
            cout << "Enter the starting student's or teacher's name for Dijkstra's algorithm: ";
            cin >> startPerson;

            int startIndex = -1;
            for (const auto &pair : g.studentNames)
            {
                if (pair.second == startPerson)
                {
                    startIndex = pair.first;
                    break;
                }
            }

            if (startIndex == -1)
            {
                for (int i = 0; i < 10; ++i) // Assuming 10 teachers
                {
                    if (g.teachers[i].name == startPerson)
                    {
                        startIndex = i + 10; // Offset to represent teachers
                        break;
                    }
                }
            }

            if (startIndex != -1)
            {
                g.dijkstra(startIndex);
            }
            else
            {
                cout << "Invalid name. Please enter a valid name." << endl;
            }
            break;
        }
        case 2:
            g.kruskalMST();
            break;
        case 3:
        {
            string name;
            cout << "Enter the name of the student or teacher to find friends or connections: ";
            cin >> name;
            g.searchByName(name);
            break;
        }
        case 4:
        {
            int studentIndex, workshopIndex;
            cout << "Enter the student index (0-9): ";
            cin >> studentIndex;
            cout << "Select a workshop (0-4): " << endl;
            for (int i = 0; i < 5; ++i)
            {
                cout << i << ". " << g.workshops[i].name << endl;
            }
            cin >> workshopIndex;
            g.scheduleWorkshop(studentIndex, workshopIndex);
            break;
        }
        case 5:
            g.printWorkshopSchedule();
            break;
        case 6:
        {
            int workshopIndex;
            cout << "Select a workshop (0-4): " << endl;
            for (int i = 0; i < 5; ++i)
            {
                cout << i << ". " << g.workshops[i].name << endl;
            }
            cin >> workshopIndex;
            g.searchStudentsInWorkshop(workshopIndex);
            break;
        }
        case 7: 
            g.exportGraphData();
            cout << "Graph data exported. Run the Python visualization script." << endl;
            break;
        case 8: 
            cout << "Algorithm Runtimes:\n";
            for (const auto &entry : algorithmRuntimes)
            {
                cout << entry.first << ": " << entry.second << " ms\n";
            }
            break;
        case 9:
            exitProgram = true;
            break;
        default:
            cout << "Invalid choice. Please select a valid option." << endl;
            break;
        }
    }

    return 0;
}