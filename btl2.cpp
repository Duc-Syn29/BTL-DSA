#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// cau truc hang doi
class Queue
{
private:
    vector<int> elements;

public:
    void enqueue(int value)
    {
        elements.push_back(value);
    }

    void dequeue()
    {
        if (!isEmpty())
        {
            elements.erase(elements.begin());
        }
    }

    int front()
    {
        return elements.front();
    }

    bool isEmpty()
    {
        return elements.empty();
    }
};

// lop bieu dien do thi vo huong co trong so la ma tran
class Graph
{
private:
    vector<vector<int>> adjacencyMatrix;
    int vertices;

public:
    Graph(int n) : vertices(n)
    {
        adjacencyMatrix.resize(n, vector<int>(n, 0));
    }
    int getVertices()
    {
        return vertices;
    }

    void loadGraphFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cerr << "Khong the mo file!\n";
            return;
        }
        file >> vertices;
        adjacencyMatrix.resize(vertices, vector<int>(vertices));
        for (int i = 0; i < vertices; i++)
        {
            for (int j = 0; j < vertices; j++)
            {
                file >> adjacencyMatrix[i][j];
            }
        }
        file.close();
    }

    void saveGraphToFile(const string &filename)
    {
        ofstream file(filename);
        if (!file)
        {
            cerr << "Khong the mo file!\n";
            return;
        }
        file << vertices << "\n";
        for (int i = 0; i < vertices; i++)
        {
            for (int j = 0; j < vertices; j++)
            {
                file << adjacencyMatrix[i][j] << " ";
            }
            file << "\n";
        }
        file.close();
    }

    void BFS(int start, int end)
    {
        vector<int> distance(vertices, INT_MAX);
        vector<int> parent(vertices, -1);
        queue<int> q;

        distance[start] = 0;
        q.push(start);

        while (!q.empty())
        {
            int current = q.front();
            q.pop();

            for (int i = 0; i < vertices; i++)
            {
                if (adjacencyMatrix[current][i] != 0 && distance[i] == INT_MAX)
                {
                    distance[i] = distance[current] + 1;
                    parent[i] = current;
                    q.push(i);

                    if (i == end)
                    {
                        break;
                    }
                }
            }
        }

        if (distance[end] == INT_MAX)
        {
            cout << "Khong co duong di tu " << start + 1 << " den " << end + 1 << "\n";
        }
        else
        {
            cout << "duong di ngan nhat tu " << start + 1 << " den " << end + 1 << ": ";
            vector<int> path;
            for (int v = end; v != -1; v = parent[v])
            {
                path.push_back(v);
            }
            for (int i = path.size() - 1; i >= 0; i--)
            {
                cout << path[i] + 1 << (i != 0 ? " -> " : "\n");
            }
        }
    }
};

int main()
{
    Graph g(0);
    string inputFile = "graph.txt";
    string outputFile = "output_graph.txt";

    g.loadGraphFromFile(inputFile);
    g.saveGraphToFile(outputFile);

    int start, end;
    cout << "Nhap dinh bat dau: ";
    cin >> start;
    while (start > g.getVertices() || start <= 0)
    {
        cout << "Nhap lai: ";
        cin >> start;
    }
    cout << "Nhap dinh ket thuc: ";
    cin >> end;
    while (end > g.getVertices())
    {
        cout << "Nhap lai: ";
        cin >> end;
    }

    g.BFS(start - 1, end - 1);

    return 0;
}
