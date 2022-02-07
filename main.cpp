#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <climits>
class str;
class node;
class edge;
class graph;
class str{
private:
    char* content;
public:
    str(const char* other = nullptr){
        if(other==nullptr)
            content= nullptr;
        else {
            content = new char[strlen(other) + 1];
            strcpy(content, other);
        }
    }
    str(const str& other){
        if(other.content == nullptr){
            delete[] content;
            content= nullptr;
        }
        else{
            delete[] content;
            content = new char[strlen(other.content)+1];
            strcpy(content,other.content);
        }
    }
    void rep(const char* other){
        delete[] content;
        content = new char[strlen(other)+1];
        strcpy(content,other);
    }
    char* rep() const{
        return content;
    }
    bool operator> (const str& a){
        if( strcmp(content, a.rep()) == -1)
            return false;
        else
            return true;
    }
    bool operator< (const str& a){
        if( strcmp(content, a.rep()) == -1)
            return true;
        else
            return false;
    }
};
class node{
public:
    str _name;
    std::vector<edge*> _node_edges;
public:
    explicit node(const char* content){
        _name.rep(content);
    }
    void add_edge(edge* content){
        _node_edges.push_back(content);
    }
};
class edge{
public:
    node* _start;
    node* _end;
    int _dist;
public:
    edge(node* start, node* end, int dist){
        _start=start;
        _end=end;
        _dist=dist;
    }
};
class graph{
public:
    int i{}, j{};
    int matrix[100][100]{};
    std::vector<edge *> _edges;
    std::vector<node *> _existing_nodes_starting;
    std::vector<node *> _existing_nodes_ending;
public:
    void print() {
        int length = _edges.size();
        for (i = 0; i < length; i++) {
            std::cout << "Node no. " << i + 1 << std::endl;
            std::cout << "Starting point: " << _edges[i]->_start->_name.rep() << " Ending Point: "
                      << _edges[i]->_end->_name.rep() << " Distance: " << _edges[i]->_dist << std::endl;
        }
    }
    void add_edge(str start, str end, int dist){
        int pos_start = search_point(start, _existing_nodes_starting);
        int pos_end = search_point(end, _existing_nodes_ending);
        if (pos_start != -1){
            if (pos_end == -1){
                node *ending_point = new node(end.rep());
                edge *new_edge = new edge(_existing_nodes_starting[pos_start], ending_point, dist);
                _existing_nodes_starting[pos_start]->add_edge(new_edge);
                _edges.push_back(new_edge);
                _existing_nodes_ending.push_back(ending_point);
            }
            else {
                edge *new_edge = new edge(_existing_nodes_starting[pos_start], _existing_nodes_ending[pos_end], dist);
                _existing_nodes_starting[pos_start]->add_edge(new_edge);
                _edges.push_back(new_edge);
            }
        }
        else {
            if (pos_end == -1){
                node *starting_point = new node(start.rep());
                node *ending_point = new node(end.rep());
                edge *new_edge = new edge(starting_point, ending_point, dist);
                starting_point->add_edge(new_edge);
                _edges.push_back(new_edge);
                _existing_nodes_starting.push_back(starting_point);
                _existing_nodes_ending.push_back(ending_point);
            }
            else{
                node *starting_point = new node(start.rep());
                edge *new_edge = new edge(starting_point, _existing_nodes_ending[pos_end], dist);
                starting_point->add_edge(new_edge);
                _edges.push_back(new_edge);
                _existing_nodes_starting.push_back(starting_point);
            }
        }
    }
    void dijkstra(str src_1, str dest_1){
        create_matrix();
        fill_matrix();
        int src=search_point(src_1,_existing_nodes_starting);
        int dest=search_point(dest_1,_existing_nodes_starting);
        int V = _existing_nodes_starting.size();
        int dist[V];
        bool sptSet[V];
        int parent[V];
        for (i = 0; i < V; i++) {
            parent[src] = -1;
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++){
            int u = minDistance(dist, sptSet);
            sptSet[u] = true;
            for (int v = 0; v < V; v++)
                if (!sptSet[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]){
                    parent[v] = u;
                    dist[v] = dist[u] + matrix[u][v];
                }
        }
        printSolution(dist, parent,src,dest);
    }
    void printStations() const{
        std::cout << "Stations: " << std::endl;
        show_list(_existing_nodes_starting);
    }
    void dijkstra_world(str src_1){
        create_matrix();
        fill_matrix();
        int src = search_point(src_1,_existing_nodes_starting);
        int V = _existing_nodes_starting.size();
        int dist[V];
        bool sptSet[V];
        int parent[V];
        for (i = 0; i < V; i++) {
            parent[src] = -1;
            dist[i] = INT_MAX;
            sptSet[i] = false;
        }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++) {
            int u = minDistance_world(dist, sptSet);
            sptSet[u] = true;
            for (int v = 0; v < V; v++)
                if (!sptSet[v] && matrix[u][v] && dist[u] + matrix[u][v] < dist[v]) {
                    parent[v] = u;
                    dist[v] = dist[u] + matrix[u][v];
                }
        }
        printSolution_world(dist, parent,src);
    }
private:
    static void show_list(std::vector<node *> a) {
        int length = a.size();
        for (int i = 0; i < length; i++)
            std::cout << a[i]->_name.rep() << " ";
        std::cout << std::endl;
    }
    int search_point(str &name, std::vector<node *> other){
        int length = other.size();
        int pos = -1;
        for (i = 0; i < length; i++)
            if (strcmp(name.rep(), other[i]->_name.rep()) == 0)
                pos = i;
        return pos;
    }
    void create_matrix() {
        int length = _existing_nodes_starting.size();
        for (i = 0; i < length; i++)
            for (j = 0; j < length; j++)
                matrix[i][j] = 0;
    }
    void fill_matrix(){
        int edge_length = _edges.size();
        int k;
        for (k = 0; k < edge_length; k++) {
            str temp = _edges[k]->_start->_name.rep();
            str temp2 = _edges[k]->_end->_name.rep();
            int x = search_point(temp, _existing_nodes_starting);
            int y = search_point(temp2, _existing_nodes_starting);
            if (x != -1 && y != -1)
                matrix[x][y] = _edges[k]->_dist;
        }
    }
    int minDistance(const int dist[], const bool sptSet[]) const{
        int V = _existing_nodes_starting.size();
        int min = INT_MAX, min_index;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && dist[v] <= min)
                min = dist[v], min_index = v;
        return min_index;
    }
    void printPath(int parent[], int p) {
        if (parent[p] == -1)
            return;
        printPath(parent, parent[p]);
        std::cout<<_existing_nodes_starting[p]->_name.rep()<<" ";
    }
    void printSolution(int dist[], int parent[],int src,int dest) {
        std::cout<<"Stations"<<std::setw(21)<<"Distance"<<std::setw(13)<<"Path";
        std::cout<<std::endl<<_existing_nodes_starting[src]->_name.rep()<<" -> "<<_existing_nodes_starting[dest]->_name.rep()<<std::setw(16)<<dist[dest]<<std::setw(15)<<_existing_nodes_starting[src]->_name.rep()<<" ";
        printPath(parent,dest);
    }
    int minDistance_world(const int dist[], const bool sptSet[]) const {
        int V = _existing_nodes_starting.size();
        int min = INT_MAX, min_index;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && dist[v] <= min)
                min = dist[v], min_index = v;
        return min_index;
    }
    void printPath_world(int parent[], int p) {
        if (parent[p] == -1)
            return;
        printPath_world(parent, parent[p]);
        std::cout<<_existing_nodes_starting[p]->_name.rep()<<" ";
    }
    void printSolution_world(int dist[], int parent[],int src) {
        int V = _existing_nodes_starting.size();
        std::cout<<"Vertex"<<std::setw(21)<<"Distance"<<std::setw(13)<<"Path";
        for (i = 0; i < V; i++) {
            if(strcmp(_existing_nodes_starting[i]->_name.rep(),"B")==0){
                std::cout<<std::endl<<_existing_nodes_starting[src]->_name.rep()<<" -> "<<_existing_nodes_starting[i]->_name.rep()<<std::setw(16)<<dist[i]<<std::setw(15)<<_existing_nodes_starting[src]->_name.rep()<<" ";
                printPath_world(parent, i);
            }
            else{
                std::cout<<std::endl<<_existing_nodes_starting[src]->_name.rep()<<" -> "<<_existing_nodes_starting[i]->_name.rep()<<std::setw(15)<<dist[i]<<std::setw(15)<<_existing_nodes_starting[src]->_name.rep()<<" ";
                printPath_world(parent, i);
            }
        }
    }
};

void menu_display(){
    std::cout << "1. Take a look at the stations. " << std::endl;
    std::cout << "2. Find a route. " << std::endl;
    std::cout << "3. See all routes from a station." << std::endl;
    std::cout << "4. Check all the routes." << std::endl;
    std::cout << "9. Exit." << std::endl;
}
void main_menu(graph& T){
    int choice = 0;
    menu_display();
    while (choice != 9) {
        char src_1[50];
        char src_2[50];
        char dest_1[50];
        std::cin >> choice;
        switch (choice) {
            case 1:
                T.printStations();
                menu_display();
                break;
            case 2:
                std::cout<<"Please enter the source station:";
                std::cin>>src_1;
                std::cout<<"Please enter the destination station:";
                std::cin>>dest_1;
                T.dijkstra(src_1,dest_1);
                std::cout<<std::endl;
                menu_display();
                break;
            case 3:
                std::cout<<"Please enter the source station:";
                std::cin>>src_2;
                T.dijkstra_world(src_2);
                std::cout<<std::endl;
                menu_display();
                break;
            case 4:
                T.print();
                menu_display();
                break;
            case 9:
                std::cout<<"Thank you for choosing AGL Calatori";
                break;
            default:
                std::cout<<"Invalid choice";
                std::cout<<std::endl;
                menu_display();
                break;
        }
    }
}
void add_nodes(graph& T){
    //CT ->
    T.add_edge("CT","TL",125);
    T.add_edge("CT","IL",139);
    //TL ->
    T.add_edge("TL","GL",83);
    T.add_edge("TL","CT",125);
    //GL ->
    T.add_edge("GL","TL",83);
    T.add_edge("GL","VS",161);
    //VS ->
    T.add_edge("VS","GL",161);
    T.add_edge("VS","IS",66);
    //IS ->
    T.add_edge("IS","SV",145);
    T.add_edge("IS","VS",66);
    //SV ->
    T.add_edge("SV","IS",145);
    T.add_edge("SV","MM",338);
    //MM ->
    T.add_edge("MM","SV",338);
    T.add_edge("MM","CJ",148);
    // CJ ->
    T.add_edge("CJ","MM",148);
    T.add_edge("CJ","BH",152);
    //BH ->
    T.add_edge("BH","CJ",152);
    T.add_edge("BH","HD",191);
    //HD ->
    T.add_edge("HD","TM",149);
    T.add_edge("HD","MH",235);
    T.add_edge("HD","VL",228);
    T.add_edge("HD","SB",128);
    T.add_edge("HD","BH",191);
    //TM ->
    T.add_edge("TM","HD",149);
    //MH ->
    T.add_edge("MH","HD",235);
    //VL ->
    T.add_edge("VL","HD",228);
    T.add_edge("VL","SB",97);
    //SB ->
    T.add_edge("SB","HD",128);
    T.add_edge("SB","VL",97);
    T.add_edge("SB","BV",142);
    //BV ->
    T.add_edge("BV","SB",142);
    T.add_edge("BV","PH",108);
    //PH ->
    T.add_edge("PH","BV",108);
    T.add_edge("PH","B",60);
    T.add_edge("PH","IL",125);
    //B ->
    T.add_edge("B","IL",126);
    T.add_edge("B","PH",60);
    //IL ->
    T.add_edge("IL","B",126);
    T.add_edge("IL","CT",139);
    T.add_edge("IL","PH",125);
}

int main() {
    graph T;
    add_nodes(T);
    main_menu(T);
    return 0;
}
