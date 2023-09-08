#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

namespace Color
{
    const string Reset = "\033[0m";
    const string Red = "\033[31m";
    const string Green = "\033[32m";
    const string Yellow = "\033[33m";
    const string Cyan = "\033[36m";
}

class Queue
{
    class Queuenode
    {
    public:
        int vertex;
        Queuenode *next;
        Queuenode(int v)
        {
            vertex = v;
            next = NULL;
        }
    };

    Queuenode *front;
    Queuenode *rear;

public:
    Queue()
    {
        front = NULL;
        rear = NULL;
    }

    void insert(int v)
    {
        Queuenode *vertex = new Queuenode(v);
        if (front == NULL)
        {
            front = vertex;
            rear = vertex;
            return;
        }
        rear->next = vertex;
        rear = vertex;
    }

    int pop()
    {
        if (front == NULL)
        {
            cout << "Queue is Empty!!\n";
            return -1;
        }
        int vertex = front->vertex;
        front = front->next;
        if (front == NULL)
        {
            rear = NULL;
        }
        return vertex;
    }

    int peek()
    {
        if (front == NULL)
        {
            cout << "Queue is Empty!!\n";
            return -1;
        }
        return front->vertex;
    }

    bool isEmpty()
    {
        return front == NULL;
    }
};

void printPath(int *parent, int dest)
{
    if (dest == -1)
    {
        return;
    }
    printPath(parent, parent[dest]);
    cout << dest << " -> ";
}

int min(int a, int b)
{
    return a > b ? b : a;
}

class Graph
{
private:
    int number_of_vertices;
    int **adj;

    bool hasPath(int src, int sink, int *parent)
    {

        for (int i = 0; i < number_of_vertices; i++)
        {
            parent[i] = -1;
        }

        bool visited[number_of_vertices];
        for (int i = 0; i < number_of_vertices; i++)
        {
            visited[i] = 0;
        }

        Queue *q = new Queue();

        q->insert(src);
        visited[src] = 1;

        while (!q->isEmpty())
        {
            int u = q->pop();
            for (int i = 0; i < number_of_vertices; i++)
            {
                if (adj[u][i] > 0 and !visited[i])
                {
                    parent[i] = u;
                    q->insert(i);
                    visited[i] = 1;
                    if (sink == i)
                    {
                        delete q;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void isnetworkValid(int ans[])
    {
        for (int i = 0; i < 5; i++)
        {
            ans[i] = 0;
        }

        int src[number_of_vertices];
        int sink[number_of_vertices];

        int number_of_sources = 0;
        int number_of_sinks = 0;

        for (int i = 0; i < number_of_vertices; i++)
        {
            src[i] = 1;
            sink[i] = 1;
        }

        for (int i = 0; i < number_of_vertices; i++)
        {
            for (int j = 0; j < number_of_vertices; j++)
            {
                if (adj[i][j] > 0)
                {
                    src[j] = 0;
                    sink[i] = 0;
                }
            }
        }

        for (int i = 0; i < number_of_vertices; i++)
        {
            if (src[i] == 1)
            {
                number_of_sources++;
                ans[1] = i;
            }
        }

        for (int i = 0; i < number_of_vertices; i++)
        {
            if (sink[i] == 1)
            {
                number_of_sinks++;
                ans[2] = i;
            }
        }

        ans[0] = number_of_sinks == 1 and number_of_sources == 1;
        ans[3] = number_of_sources;
        ans[4] = number_of_sinks;
    }

    void print()
    {
        for (int i = 0; i < number_of_vertices; i++)
        {
            for (int j = 0; j < number_of_vertices; j++)
            {
                printf("%-2d ", adj[i][j]);
            }
            cout << "\n";
        }
    }

public:
    Graph(int n)
    {
        number_of_vertices = n;
        adj = new int *[number_of_vertices];
        for (int i = 0; i < n; i++)
        {
            adj[i] = new int[number_of_vertices];
            for (int j = 0; j < n; j++)
            {
                adj[i][j] = 0;
            }
        }
    }

    Graph(int n, int **graph)
    {
        number_of_vertices = n;
        adj = graph;
    }

    void addEdge(int src, int dest, int wt)
    {
        adj[src][dest] = wt;
    }

    int maxFlow()
    {
        int valid[5] = {0};
        isnetworkValid(valid);

        if (!valid[0])
        {
            cout << "Network is not valid\n";
            if (valid[1] > 1)
            {
                cout << "There are " << valid[3] << " sources in the given graph!!\n";
            }
            if (valid[2] > 1)
            {
                cout << "There are " << valid[4] << " sinks in the given graph!!\n";
            }
            return -1;
        }

        int src = valid[1];
        int sink = valid[2];

        cout << "\n\nSource : " << src << endl;
        cout << "Sink : " << sink << "\n\n";

        int temp[number_of_vertices][number_of_vertices];
        int parent[number_of_vertices];

        for (int i = 0; i < number_of_vertices; i++)
        {
            for (int j = 0; j < number_of_vertices; j++)
            {
                temp[i][j] = adj[i][j];
            }
        }

        int ans = 0;

        while (hasPath(src, sink, parent))
        {
            cout << "--------------------Residual Graph------------------\n";
            print();

            cout << "\n\nAugmented path chosen : ";
            printPath(parent, sink);
            cout << "x\n";

            int flow_of_one_path = INT_MAX;
            for (int v = sink; v != src; v = parent[v])
            {
                int u = parent[v];
                flow_of_one_path = min(flow_of_one_path, adj[u][v]);
            }

            for (int v = sink; v != src; v = parent[v])
            {
                int u = parent[v];
                adj[u][v] -= flow_of_one_path;
                adj[v][u] += flow_of_one_path;
            }

            ans += flow_of_one_path;

            cout << "Flow added in this augmenting path : " << flow_of_one_path << "\n\n";
        }

        cout << "---------------------Final residual graph---------------\n";
        for (int i = 0; i < number_of_vertices; i++)
        {
            for (int j = 0; j < number_of_vertices; j++)
            {
                if (temp[i][j] - adj[i][j] >= 0)
                {
                    if (adj[i][j] == 0) // Change this condition to highlight specific values
                        cout << Color::Cyan;
                    printf("%-2d ", adj[i][j]);
                    cout << Color::Reset;
                }
                else
                {
                    printf("%-2d ", 0);
                }
            }
            cout << endl;
        }

        cout << "---------------------Final flow network---------------\n";
        for (int i = 0; i < number_of_vertices; i++)
        {
            for (int j = 0; j < number_of_vertices; j++)
            {
                if (temp[i][j] - adj[i][j] >= 0)
                {
                    if (temp[i][j] - adj[i][j] == 0) // Change this condition to highlight specific values
                        cout << Color::Green;
                    printf("%-2d ", temp[i][j] - adj[i][j]);
                    cout << Color::Reset;
                }
                else
                {
                    printf("%-2d ", 0);
                }
            }
            cout << endl;
        }

        return ans;
    }
};

int main(int argc, char const *argv[])
{
    bool flag = false;
    int **graph;
    Graph *g;
    int max_flow = 0;
    std::ofstream outputFile("output.txt");
    while (true)
    {
        cout << endl;
        cout << endl;
        cout << "1.Enter graph using adjacency list\t\t2.Enter graph using adjacency matrix\n"
                "3.Find Max Flow of graph\t\t\t4.Exit\n";
        int option;
        cin >> option;
        switch (option)
        {
        case 1:
            flag = true;
            cout << endl;
            cout << "Enter number of vertices and edges : ";
            int n, e;
            cin >> n >> e;
            g = new Graph(n);
            cout << "----------------------Enter edges of graph in {source,destination,weight} format--------------------\n";
            for (int i = 0; i < e; i++)
            {
                int u, v, wt;
                cin >> u >> v >> wt;
                g->addEdge(u, v, wt);
            }
            break;
        case 2:
            flag = true;
            int no;
            cout << "Enter number of vertices : ";
            cin >> no;
            graph = new int *[no];
            for (int i = 0; i < no; i++)
            {
                graph[i] = new int[no];
                for (int j = 0; j < no; j++)
                {
                    cin >> graph[i][j];
                }
            }
            g = new Graph(no, graph);
            break;
        case 3:
            if (flag)
            {
                // Redirect the standard output to the file stream
                std::streambuf* coutBuffer = std::cout.rdbuf();
                std::cout.rdbuf(outputFile.rdbuf());

                max_flow = g->maxFlow();
                cout << Color::Yellow << "***************************************************************************************************************" << endl;
                cout << "\t\t\t\t\tMax Flow(Ford Fulkerson with BFS) : " << max_flow << endl;
                cout << "***************************************************************************************************************" << Color::Reset << endl;

                // Restore the standard output
                std::cout.rdbuf(coutBuffer);

                cout << "Output has been saved to 'output.txt'." << endl;
            }
            else
            {
                cout << Color::Red << "Please input a graph first!!" << Color::Reset << endl;
            }
            break;
        default:
            exit(0);
            break;
        }
    }

    return 0;
}