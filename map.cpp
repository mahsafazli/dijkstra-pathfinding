/*
Dijkstra's Algorithm — Navigation & Pathfinding
Author: Mahsa Fazli
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <limits>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

const double INF = numeric_limits<double>::infinity();

// struct Vertex stores one vertex.
// For input-graph.txt format, label will be the number as text (for example "0").
// For canada.txt format, label will be a name such as "s", "a", or "t".
struct Vertex
{
    string label;
    double x;
    double y;
};

// Graph class that supports both input file formats(input-graph.txt and canada.txt)
class Graph
{
private:
    bool directed;
    bool coordinateFormat;

    int numVertices;
    int numEdges;

    vector<Vertex> vertices;
    map<string, int> labelToIndex;

    // Adjacency Matrix representation.
    vector<vector<double>> adjMatrix;

    // Adjacency List representation.
    vector<vector<pair<int, double>>> adjList;

    // This function removes leading and trailing spaces from a string.
    string trim(const string& s) const
    {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == string::npos)
        {
            return "";
        }

        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    // This function checks whether a line starts with a number.
    bool startsWithDigit(const string& s) const
    {
        if (s.empty())
        {
            return false;
        }

        return isdigit(static_cast<unsigned char>(s[0]));
    }

    // This function computes Euclidean distance for coordinate-based input.
    double calculateDistance(int u, int v) const
    {
        double dx = vertices[u].x - vertices[v].x;
        double dy = vertices[u].y - vertices[v].y;
        return sqrt(dx * dx + dy * dy);
    }

    // This function prints the shortest path using the parent array.
    void printPath(const vector<int>& parent, int destination) const
    {
        vector<int> path;
        int current = destination;

        while (current != -1)
        {
            path.push_back(current);
            current = parent[current];
        }

        reverse(path.begin(), path.end());

        cout << "Route: ";
        for (int i = 0; i < path.size(); i++)
        {
            cout << vertices[path[i]].label;
            if (i != path.size() - 1)
            {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    // This function initializes adjacency matrix and adjacency list.
    void initializeGraphStructures()
    {
        adjMatrix.assign(numVertices, vector<double>(numVertices, INF));
        adjList.assign(numVertices, vector<pair<int, double>>());

        for (int i = 0; i < numVertices; i++)
        {
            adjMatrix[i][i] = 0.0;
        }
    }

    // This function reads the input-graph.txt style format.
    bool loadCoordinateFormat(ifstream& fin)
    {
        coordinateFormat = true;
        directed = false;

        fin >> numVertices >> numEdges;

        if (numVertices <= 0 || numEdges < 0)
        {
            cout << "Error: Invalid graph size.\n";
            return false;
        }

        vertices.resize(numVertices);
        initializeGraphStructures();

        // To read vertices: id x y
        for (int i = 0; i < numVertices; i++)
        {
            int id;
            double x, y;
            fin >> id >> x >> y;

            if (id < 0 || id >= numVertices)
            {
                cout << "Error: Invalid vertex id in file.\n";
                return false;
            }

            vertices[id] = { to_string(id), x, y };
            labelToIndex[to_string(id)] = id;
        }

        // To read edges: u v
        for (int i = 0; i < numEdges; i++)
        {
            int u, v;
            fin >> u >> v;

            if (u < 0 || u >= numVertices || v < 0 || v >= numVertices)
            {
                cout << "Error: Invalid edge in file.\n";
                return false;
            }

            double weight = calculateDistance(u, v);

            // Undirected graph for this format.
            adjMatrix[u][v] = weight;
            adjMatrix[v][u] = weight;

            adjList[u].push_back({ v, weight });
            adjList[v].push_back({ u, weight });
        }

        return true;
    }

    // This function reads the canada.txt format.
    bool loadTextFormat(ifstream& fin)
    {
        coordinateFormat = false;
        directed = false;
        numVertices = 0;
        numEdges = 0;

        vector<tuple<string, string, double>> edges;
        string line;

        while (getline(fin, line))
        {
            line = trim(line);

            if (line.empty())
            {
                continue;
            }

            istringstream iss(line);
            string keyword;
            iss >> keyword;

            if (keyword == "directed")
            {
                directed = true;
            }
            else if (keyword == "undirected")
            {
                directed = false;
            }
            else if (keyword == "vertex")
            {
                string name;
                iss >> name;

                if (labelToIndex.find(name) == labelToIndex.end())
                {
                    labelToIndex[name] = numVertices;
                    vertices.push_back({ name, 0.0, 0.0 });
                    numVertices++;
                }
            }
            else if (keyword == "edge")
            {
                string from, to;
                double weight;
                iss >> from >> to >> weight;

                edges.push_back({ from, to, weight });
                numEdges++;
            }
            else if (keyword == "dijkstra")
            {
                // This line is ignored because the program asks the user interactively.
                continue;
            }
        }

        if (numVertices <= 0)
        {
            cout << "Error: No vertices found in file.\n";
            return false;
        }

        initializeGraphStructures();

        for (int i = 0; i < edges.size(); i++)
        {
            string from = get<0>(edges[i]);
            string to = get<1>(edges[i]);
            double weight = get<2>(edges[i]);

            if (labelToIndex.find(from) == labelToIndex.end() ||
                labelToIndex.find(to) == labelToIndex.end())
            {
                cout << "Error: Edge references an undefined vertex.\n";
                return false;
            }

            int u = labelToIndex[from];
            int v = labelToIndex[to];

            adjMatrix[u][v] = weight;
            adjList[u].push_back({ v, weight });

            if (!directed)
            {
                adjMatrix[v][u] = weight;
                adjList[v].push_back({ u, weight });
            }
        }

        return true;
    }

public:
    Graph()
    {
        directed = false;
        coordinateFormat = false;
        numVertices = 0;
        numEdges = 0;
    }

    // This function detects the format and loads the graph.
    bool loadFromFile(const string& filename)
    {
        ifstream fin(filename);

        if (!fin)
        {
            cout << "Error: Could not open file.\n";
            return false;
        }

        string firstLine;
        getline(fin, firstLine);
        firstLine = trim(firstLine);

        if (firstLine.empty())
        {
            cout << "Error: File is empty.\n";
            return false;
        }

        fin.close();

        ifstream fin2(filename);
        if (!fin2)
        {
            cout << "Error: Could not reopen file.\n";
            return false;
        }

        if (startsWithDigit(firstLine))
        {
            return loadCoordinateFormat(fin2);
        }
        else
        {
            return loadTextFormat(fin2);
        }
    }

    // This function displays the list of vertices.
    void showVertices() const
    {
        cout << "\nAvailable vertices: { ";
        for (int i = 0; i < vertices.size(); i++)
        {
            cout << vertices[i].label;
            if (i != vertices.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << " }\n";
    }

    // This function validates whether the entered label exists.
    bool isValidVertexLabel(const string& label) const
    {
        return labelToIndex.find(label) != labelToIndex.end();
    }

    // Dijkstra using Adjacency Matrix.
    double dijkstraMatrix(const string& sourceLabel, const string& destinationLabel) const
    {
        int source = labelToIndex.at(sourceLabel);
        int destination = labelToIndex.at(destinationLabel);

        vector<double> dist(numVertices, INF);
        vector<bool> visited(numVertices, false);
        vector<int> parent(numVertices, -1);

        dist[source] = 0.0;

        for (int count = 0; count < numVertices - 1; count++)
        {
            int u = -1;
            double minDistance = INF;

            for (int i = 0; i < numVertices; i++)
            {
                if (!visited[i] && dist[i] < minDistance)
                {
                    minDistance = dist[i];
                    u = i;
                }
            }

            if (u == -1)
            {
                break;
            }

            visited[u] = true;

            for (int v = 0; v < numVertices; v++)
            {
                if (!visited[v] &&
                    adjMatrix[u][v] != INF &&
                    dist[u] != INF &&
                    dist[u] + adjMatrix[u][v] < dist[v])
                {
                    dist[v] = dist[u] + adjMatrix[u][v];
                    parent[v] = u;
                }
            }
        }

        cout << "\n--- Adjacency Matrix Result ---\n";

        if (dist[destination] == INF)
        {
            cout << "No path found.\n";
            return INF;
        }

        cout << "Shortest distance: " << dist[destination] << endl;
        printPath(parent, destination);

        return dist[destination];
    }

    // Dijkstra using Adjacency List and priority queue.
    double dijkstraList(const string& sourceLabel, const string& destinationLabel) const
    {
        int source = labelToIndex.at(sourceLabel);
        int destination = labelToIndex.at(destinationLabel);

        vector<double> dist(numVertices, INF);
        vector<int> parent(numVertices, -1);

        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        dist[source] = 0.0;
        pq.push({ 0.0, source });

        while (!pq.empty())
        {
            double currentDistance = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (currentDistance > dist[u])
            {
                continue;
            }

            for (int i = 0; i < adjList[u].size(); i++)
            {
                int v = adjList[u][i].first;
                double weight = adjList[u][i].second;

                if (dist[u] != INF && dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                    pq.push({ dist[v], v });
                }
            }
        }

        cout << "\n--- Adjacency List Result ---\n";

        if (dist[destination] == INF)
        {
            cout << "No path found.\n";
            return INF;
        }

        cout << "Shortest distance: " << dist[destination] << endl;
        printPath(parent, destination);

        return dist[destination];
    }
};

int main()
{
    Graph g;
    string filename;

    cout << "Enter input file name: ";
    cin >> filename;

    if (!g.loadFromFile(filename))
    {
        return 1;
    }

    g.showVertices();

    long long totalMatrixTime = 0;
    long long totalListTime = 0;

    while (true)
    {
        string source;
        string destination;

        cout << "\nEnter source vertex (or quit): ";
        cin >> source;

        if (source == "quit")
        {
            break;
        }

        cout << "Enter destination vertex: ";
        cin >> destination;

        if (!g.isValidVertexLabel(source) || !g.isValidVertexLabel(destination))
        {
            cout << "Invalid vertex. Please choose from the displayed list.\n";
            continue;
        }

        auto startMatrix = chrono::high_resolution_clock::now();
        double matrixDistance = g.dijkstraMatrix(source, destination);
        auto endMatrix = chrono::high_resolution_clock::now();

        auto matrixDuration =
            chrono::duration_cast<chrono::microseconds>(endMatrix - startMatrix).count();
        totalMatrixTime += matrixDuration;

        auto startList = chrono::high_resolution_clock::now();
        double listDistance = g.dijkstraList(source, destination);
        auto endList = chrono::high_resolution_clock::now();

        auto listDuration =
            chrono::duration_cast<chrono::microseconds>(endList - startList).count();
        totalListTime += listDuration;

        cout << "\n---------------------------------------------\n";
        cout << "Time using adjacency matrix: " << matrixDuration << " microseconds\n";
        cout << "Time using adjacency list:   " << listDuration << " microseconds\n";
        cout << "---------------------------------------------\n";

        if (matrixDistance != INF && listDistance != INF &&
            fabs(matrixDistance - listDistance) > 0.000001)
        {
            cout << "Warning: The two methods produced different distances.\n";
        }
    }

    cout << "\n---------- Final Performance Report ----------\n";
    cout << "Total matrix time: " << totalMatrixTime << " microseconds\n";
    cout << "Total list time:   " << totalListTime << " microseconds\n";

    if (totalMatrixTime < totalListTime)
    {
        cout << "Adjacency Matrix was faster.\n";
    }
    else if (totalListTime < totalMatrixTime)
    {
        cout << "Adjacency List was faster.\n";
    }
    else
    {
        cout << "Both took the same time.\n";
    }

    return 0;
}