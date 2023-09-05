//20230905_GRAPH practice
#define DEBUG
#define MAXN 1e9
#include <bits/stdc++.h>
#define PRIM 1
#define KRUSAL 2
#define DIJKSTRA 3
#define BELLMAN 4
#define FLOYD 5
#define SOLLIN 6


using namespace std;
class Node {
    public : 
        int index;
        int parent; /* -1 for root */
        int key;
        Node () {
            index = 0; 
            parent = -1;
            key = MAXN;
        }
        Node (int i , int p = -1, int k = MAXN) {
            index = i;
            parent = p;
            key = k;
        }
};
//if 不提供某個參數(i,p,k)的值，則將使用預設值，例如，如果只提供了i的值，而未提供p和k，則p將被設為-1，k將被設為MAXN。

class disjoint_set {           //不相交集合數據結構的基本功能, 包括初始化、查找和合併操作
    public :
        struct n {
            int parent;
            int rank;
        };
        int total;
        struct n *sset;

        disjoint_set (int a) {
            total = a;
            sset = new struct n [total];
            for (int i = 0; i < total; ++i) {
                sset[i].parent = i;
                sset[i].rank = 0;
            }
        }
        int find (int u) {       //查找操作
            if (u != sset[u].parent) 
                sset[u].parent = find(sset[u].parent);
            return sset[u].parent;
        }
        void _union (int u, int v) { //合併操作,根據元素所屬集合的rank（深度）來優化
            int u_set = find(u);
            int v_set = find(v);
            if (sset[u_set].rank < sset[v_set].rank) {
                sset[u_set].parent = v_set;
            } else if (sset[u_set].rank > sset[v_set].rank ) {
                sset[v_set].parent = u_set;
            } else {
                sset[u_set].parent = v_set;
                sset[v_set].rank++;
            }
        }
};
/*如果u_set的深度< v_set，則將u_set的根節點的parent設為v_set的根節點，or v_set的深度< u_set，則將v_set的
根節點的parent設為u_set的根節點，如果它們的深度相等，則選擇一個作為新的根節點，並將另一個的深度增加一。*/

class edge {
    public :
        int weight;
        int v1; //兩個頂點v1和v2
        int v2;

        edge (int w, int a1, int a2) { 
            weight = w;
            v1 = a1;
            v2 = a2;
        }        //edge類別的構造函數（constructor）,用w a1 a2來初始化edge物件的權重（weight）、v1和v2成員變數。
        edge () {
            weight = MAXN;
            v1 = -1;
            v2 = -1;
        }       //當不提供任何參數時，將調用這個函數，它將weight初始化為MAXN，v1和v2初始化為-1
        bool operator<(const edge& rhs) {
            return this->weight < rhs.weight;
        }     //排序, 比較邊的權重
};
//edge表示圖的邊，它可以存儲邊的權重以及相關的頂點信息

class Graph {
    private :
        int **graph;     //指向二維整數陣列的指針，用於表示圖的鄰接矩陣
        int vertex;
        vector<bool> visited; //visited的布林向量，用於跟蹤圖中的頂點是否已被訪問過
        Node *node;
    public :
        Graph (int total) {
            vertex = total;  //表示圖中的總頂點數量
            
            //動態分配了一個vertex x vertex大小的二維整數陣列，這個陣列用於表示圖的鄰接矩陣
            graph = new int* [vertex];  
            for (int i = 0; i < vertex; ++i) {
                graph[i] = (int *)new int* [vertex];
            }
            // initial not connected
            for (int i = 0; i < vertex; ++i) {
                for (int j = 0; j < vertex; ++j) {
                    if (i == j) graph[i][j] = 0; //處理對角線元素，值設為0，表示頂點到自身的距離是0。
                    else graph[i][j] = MAXN; //非對角線元素，值初始化為 MAXN
                }
            }
            node = new Node [vertex]; //存儲有關每個頂點的其他信息
            visited.resize(vertex, false); /*將 visited 向量的大小調整為 vertex，並將所有元素
            初始化為 false, 表示一開始所有的頂點都未被訪問過*/
        }
        void build () {
            int total_path;
            cin >> total_path;
            for (int i = 0; i < total_path; ++i) {
                int from, to; //起始頂點（from）和終止頂點（to）。
                cin >> from >> to ;
                cin >> graph[from][to];
                graph[to][from] = graph[from][to]; //確保在無向圖中兩個相鄰的頂點之間的邊具有相同的權重。
            }
        }
        void dfs () { //比較兩種不同的實現方式，並輸出它們的結果
            cout << "recursive : ";
            dfs_recursive(0);
            cout << endl;
            /* --------------------------*/
            fill(visited.begin(), visited.end(), false);
            cout << "nonrecursive : ";
            dfs_nonrecursive(); //使用堆疊（stack）而不是遞迴來實現深度優先搜索。
            cout << endl;
        }
        void dfs_recursive(int index) {
            visited[index] = true; //表示當前節點已被訪問過 
            cout << (char)(index+'A') << "-> ";
            for (int i = 0; i < vertex; ++i) {
                if (graph[index][i] != MAXN && visited[i] == false) {
                    dfs_recursive(i);
                }
            }
        }
        void dfs_nonrecursive() {
            // O(n^2)
            stack<int > buffer;
            buffer.push(0);
            while (!buffer.empty()) {
                int top = buffer.top();
                buffer.pop();
                if (!visited[top]) {
                    // visit top
                    cout << (char)(top+'A') << "-> ";
                    visited[top] = true;
                    // push the adjecent node into the stack
                    for (int i = vertex-1; i >= 0; --i) {
                        if (graph[top][i] != MAXN && visited[i] == false) {
                            buffer.push(i);
                        }
                    }
                }
            }
        }

        void bfs () {
            cout << "bfs " << endl;
            vector<bool> done; 
            done.resize(vertex, false);
            
            queue<int > q;
            q.push(0);
            while (!q.empty()) {
                int top = q.front();
                q.pop();
                if (!done[top]) { 
                    cout << (char)(top+'A') << "-> ";
                    done[top] = true;
                    for (int i = 0; i < vertex; ++i) {
                        if (graph[top][i] != MAXN && done[i] == false) {
                            q.push(i);
                        }
                    }
                }
            }
            cout << endl;
        }
        void spanning_tree (int method) {
            if (method == PRIM) prim();
            else if (method == KRUSAL) krusal();
            else if (method == SOLLIN) sollin();
        }
        void prim () {
            // initial
            vector<bool > gone(vertex);
            fill(gone.begin(), gone.end(), false);
            for (int i = 0; i < vertex; ++i) {
                node[i].index = i;
                node[i].key = MAXN;
                node[i].parent = -1;
            }
            node[vertex-1].key = 0;
            
            // prim
           for (int i = 0; i < vertex; ++i) {
                // find the min key node to deal 
                int min = MAXN;
                int min_index = -1;
                for (int j = 0; j < vertex; ++j) {
                    if (!gone[j] && min > node[j].key ) {
                        min = node[j].key;
                        min_index = j;
                    }
                }
                gone[min_index] = true;
                // update the near node's key
                for (int j = 0; j < vertex; ++j) {
                    if (graph[min_index][j] != MAXN &&
                        !gone[j] &&
                        node[j].key > graph[min_index][j]){
                        node[j].key = graph[min_index][j];
                        node[j].parent = min_index;
                    }
                }
           }

            // print result
            for (int i = 0; i < vertex; ++i) {
                cout << (char)(node[i].parent + 'A') << " --- " << (char)(node[i].index + 'A') << endl;
            }
            

        }
        void krusal() {
            vector<edge > sorted;
            for (int i = 0; i < vertex; ++i) {
                for (int j = i+1; j < vertex; ++j) {
                    if (graph[i][j] !=  MAXN) { 
                        sorted.push_back(edge(graph[i][j], i, j)); 
                    }
                }
            }
            sort(sorted.begin(), sorted.end());
            disjoint_set ans(vertex);
            vector<edge > ans_edge;

            // for each edge
            for (int i = 0; i < vertex; ++i) {
                // if the edge node not cause a cycle
                if (ans.find(sorted[i].v1) != ans.find(sorted[i].v2)) {
                    ans_edge.push_back(sorted[i]);
                    ans._union(sorted[i].v1, sorted[i].v2);
                }
            }
            for (int i = 0; i < ans_edge.size(); ++i) {
                cout << (char)(ans_edge[i].v1 + 'A') << " --- " << (char)(ans_edge[i].v2+'A') << endl;
            }
        }

        void sollin() {
            vector<edge > cheap(vertex);
            disjoint_set ans(vertex);
            int num_component = vertex;
            vector<edge > ans_edge;

            while (num_component > 1) {
                // min cost
                for (int i = 0; i < vertex; ++i) {
                    cheap[i] = edge(MAXN, i, i); // no cheap
                }
                // update cheap
                for (int i = 0; i < vertex; ++i) {
                    for (int j = 0; j < vertex; ++j) {
                        if (graph[i][j] != MAXN) {
                            int set_i = ans.find(i);
                            int set_j = ans.find(j);
                            if (set_i == set_j) continue;
                            
                            if (cheap[set_i].weight == MAXN || cheap[set_i].weight > graph[i][j]) 
                                cheap[set_i] = edge(graph[i][j], i, j);
                            if (cheap[set_j].weight == MAXN || cheap[set_j].weight > graph[i][j]) 
                                cheap[set_j] = edge(graph[i][j], i, j);
                        }
                    }
                }

                for (int i = 0; i < vertex; ++i) {
                    if (cheap[i].weight != MAXN) {
                        // TODO : if no this will error
                        // but dont know why
                        int set1 = ans.find(cheap[i].v1);
                        int set2 = ans.find(cheap[i].v2);
                        if (set1 == set2) continue;
                        ans_edge.push_back(cheap[i]);
                        ans._union(cheap[i].v1, cheap[i].v2);
                        num_component--;
                    }
                }
            
            }

            for (int i = 0; i < ans_edge.size(); ++i) {
                cout << (char)(ans_edge[i].v1 + 'A') << " --- " << (char)(ans_edge[i].v2+'A') << endl;
            }
        }

        void single_source_shortest_path (int method, int start) {
            if (method == DIJKSTRA) dijkstra(start);
            else if (method == BELLMAN) bellman(start);
            else if (method == FLOYD) floyd(start);
        }
        void dijkstra (int start) {
            vector<int > cost(vertex);
            fill(cost.begin(), cost.end(), MAXN);
            vector<bool > gone(vertex);
            fill(gone.begin(), gone.end(), false);
            cost[start] = 0;

            for (int i = 0; i < vertex;++i) {
                // find the minmum cost of unjudge node
                int min = MAXN + 10;
                int min_index = -1;
                for (int j = 0; j < vertex; ++j) {
                    if (cost[j] < min && !gone[j]) {
                        min = cost[j];
                        min_index = j;
                    }
                }
                // mark visited
                gone[min_index] = true;
                // adjecent node of min
                for (int j = 0; j < vertex; ++j) {
                    if (graph[min_index][j] != MAXN &&
                        !gone[j] &&
                        min+graph[min_index][j] < cost[j]) {
                        cost[j] = min + graph[min_index][j];
                    }
                }
            }

            // print 
            cout << "from " << (char)(start+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != start) {
                    cout << (char)(start+'A') << "--> " << (char)(i+'A') << " is " << cost[i] << endl;
                }
            }
        }
        void bellman (int start) {
            vector<int > pi(vertex);
            fill(pi.begin(), pi.end(), -1);
            vector<int > cost(vertex);
            fill(cost.begin(), cost.end(), MAXN);
            cost[start] = 0;

            for (int i = 0; i < vertex-1; ++i) {
                // for every node 
                for (int n = 0; n < vertex; ++n) {
                    // for its adjecent node
                    for (int adjecent = 0; adjecent < vertex; ++adjecent) {
                        if (graph[n][adjecent] != MAXN && 
                            cost[n] + graph[n][adjecent] < cost[adjecent] ) {
                            pi[adjecent] = n;
                            cost[adjecent] = cost[n] + graph[n][adjecent];
                        }
                    }
                }
            }

            // print
            cout << "from " << (char)(start+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != start) {
                    cout << (char)(start+'A') << "--> " << (char)(i+'A') << " is " << cost[i] << endl;
                }
            }
        }
        void floyd (int source) {
            int cost[vertex][vertex];
            //copy map
            for (int i = 0; i < vertex;++i) {
                for(int j = 0; j < vertex; ++j) {
                    cost[i][j] = graph[i][j];
                }
            }
            for (int middle = 0; middle < vertex; ++middle) {
                for (int start = 0; start < vertex; ++start) {
                    for (int end = 0; end < vertex; ++end ) {
                        if (cost[start][middle] + cost[middle][end] < cost[start][end] ) {
                            cost[start][end] = cost[start][middle] + cost[middle][end] ;
                        }
                    }
                }
            }
            // print
            cout << "from " << (char)(source+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != source) {
                    cout << (char)(source+'A') << "--> " << (char)(i+'A') << " is " << cost[source][i] << endl;
                }
            }
        }

};

int main () 
{
    #ifdef DEBUG
    freopen ("in.in", "r", stdin);
    #endif 
    int total;
    cin >> total;
    Graph t(total);

    t.build();
    cout << "---------dfs---------\n";
    t.dfs();
    cout << "---------bfs---------\n";
    t.bfs();
    cout << "-------------spanning tree-------------\n";
    cout << "prim \n";
    t.spanning_tree(PRIM); 
    cout << "KRUSAL \n";
    t.spanning_tree(KRUSAL); 
    cout << "SOLLIN \n";
    t.spanning_tree(SOLLIN); 
    
    cout << "------------single_source_shortest_path-------------\n";
    cout << "dijkstra \n";
    t.single_source_shortest_path(DIJKSTRA, 0);
    cout << "bellman \n";
    t.single_source_shortest_path(BELLMAN, 0);
    cout << "floyd \n";
    t.single_source_shortest_path(FLOYD, 0);
    

    return 0;
}

/*
testcase :
8
10
0 1 1
0 2 2
1 3 3
1 4 6
2 5 5
2 6 4
3 7 7
4 7 3
5 7 2
6 7 1
 */
