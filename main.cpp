#include<bits/stdc++.h>
using namespace std;

typedef map<pair<int, int>, vector<vector<pair<int, int>>>> region;

ifstream fin;
pair<int,int> tankStartPosition;
int gasVolume;
int rows, columns;
vector<vector<int>> maps;
vector<pair<int,int>> stars;
map<pair<int, int>, vector<pair<int, int>>> pathsBetween;
vector<pair<int,int>> gasStations;
region regions;

struct Node {
    Node* pre;
    pair<int,int> cur;
    int cost;
    int g;
    Node(Node* pre, pair<int,int> cur, int g, int cost) {
        this->pre = pre;
        this->cur = cur;
        this->g = g;
        this->cost = cost;
    }
};

void readFile(string filename) {
    fin.open(filename);
    fin >> tankStartPosition.first >> tankStartPosition.second;
    fin >> gasVolume;
    fin >> rows >> columns;
    maps.resize(rows, vector<int>(columns, 0));
    int row = 0;
    while(!fin.eof()) {
        for(int i = 0; i < columns; ++i) {
            fin>>maps[row][i];
        }
        ++row;
    }
    fin.close();
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < columns; ++j) {
            if(maps[i][j] == 2) {
                gasStations.emplace_back(make_pair(i,j));
            }else if(maps[i][j] == 3) {
                stars.emplace_back(make_pair(i,j));
            }
        }
    }
}

int manhattanDistance(const pair<int,int>& p1,const pair<int,int>& p2) {
    return abs(p1.first-p2.first) + abs(p1.second-p2.second);
}

class BinaryHeap {
public:
    BinaryHeap(int capacity) {
        currentSize = 0;
        array.resize(capacity);
    }
    void insert(Node* x){
        if(currentSize == array.size() -1){
            array.resize(array.size() * 2);
        }

        int hole = ++currentSize;

        //store the inserted item in array[0]
        array[0] = x;

        //in the case that x is a new minimum, x = array[hole/2] which is array[0] and then terminate the loop
        for( ; x->cost < array[hole/2]->cost ; hole /= 2){
            array[hole] = array[hole/2];
        }
        array[hole] = array[0];
    }
    Node* top() {
        return array[1];
    }
    void pop() {
        array[1] = array[currentSize];
        --currentSize;
        if(currentSize > 0)
            percolateDown(1);
    }
    bool isEmpty() {
        return currentSize = 0;
    }
    void decreaseKey(int index, int amount, int g, Node* pre){
        if(1<=index && index<=currentSize && amount < array[index]->cost){
            array[index]->cost = amount;
            array[index]->g = g;
            array[index]->pre = pre;
            percolateUp(index);
        }
    }
    int findIndex(pair<int,int> p) {
        for(int i = 1; i <= currentSize; ++i) {
            if(array[i]->cur == p) {
                return i;
            }
        }
        return -1;
    }
private:
    int currentSize;
    vector<Node*> array;
        void percolateDown(int hole){
        int child;
        Node* tmp = array[hole];

        for( ; hole*2 <= currentSize; hole = child){
            child = hole * 2;
            //compare two children of the hole and test whether the right child is existed
            if(child != currentSize && array[child+1]->cost < array[child]->cost){
                ++child;
            }
            //compare child with the hole
            if(array[child] < tmp){
                array[hole] = array[child];
            }else{
                break;
            }
        }
        array[hole] = tmp;
    }
    void percolateUp(int hole){
        int parent;
        Node* tmp = array[hole];

        for( ; hole/2 >= 1; hole = parent){
            parent = hole/2;
            //compare a hole with its parent
            if(array[parent]->cost > array[hole]->cost){
                array[hole] = array[parent];
            }else{
                break;
            }
        }
        array[hole] = tmp;
    }
};

bool isValid(int x, int y, set<pair<int,int>>& visited) {
    return x >= 0 && x < rows && y >= 0 && y < columns && !visited.count(make_pair(x,y)) && maps[x][y] != 0;
}
vector<pair<int,int>> aStarSearch(pair<int,int>& start, pair<int,int>& end) {
    Node* node = new Node(nullptr, start, 0, manhattanDistance(start, end));
    BinaryHeap heap(1000);
    set<pair<int,int>> visited;
    heap.insert(node);
    while(heap.top()->cur != end) {
        Node* curNode = heap.top();
        visited.insert(curNode->cur);
        heap.pop();
        if (isValid(curNode->cur.first+1, curNode->cur.second, visited)) {
            pair<int,int> p = make_pair(curNode->cur.first+1, curNode->cur.second);
            int index = heap.findIndex(p);
            if(index == -1) {
                heap.insert(new Node(curNode, p, curNode->g+1, manhattanDistance(p, end)+ curNode->g+1));
            }else {
                heap.decreaseKey(index, manhattanDistance(p, end)+curNode->g+1, curNode->g+1, curNode);
            }
        }
        if (isValid(curNode->cur.first, curNode->cur.second+1, visited)) {
            pair<int,int> p = make_pair(curNode->cur.first, curNode->cur.second+1);
            int index = heap.findIndex(p);
            if(index == -1) {
                heap.insert(new Node(curNode, p, curNode->g+1, manhattanDistance(p, end)+ curNode->g+1));
            }else {
                heap.decreaseKey(index, manhattanDistance(p, end)+curNode->g+1, curNode->g+1, curNode);
            }
        }
        if (isValid(curNode->cur.first-1, curNode->cur.second, visited)) {
            pair<int,int> p = make_pair(curNode->cur.first-1, curNode->cur.second);
            int index = heap.findIndex(p);
            if(index == -1) {
                heap.insert(new Node(curNode, p, curNode->g+1, manhattanDistance(p, end)+ curNode->g+1));
            }else {
                heap.decreaseKey(index, manhattanDistance(p, end)+curNode->g+1, curNode->g+1, curNode);
            }
        }
        if (isValid(curNode->cur.first, curNode->cur.second-1, visited)) {
            pair<int,int> p = make_pair(curNode->cur.first, curNode->cur.second-1);
            int index = heap.findIndex(p);
            if(index == -1) {
                heap.insert(new Node(curNode, p, curNode->g+1, manhattanDistance(p, end)+curNode->g+1));
            }else {
                heap.decreaseKey(index, manhattanDistance(p, end)+curNode->g+1, curNode->g+1, curNode);
            }
        }
    }
    vector<pair<int,int>> res;
    node = heap.top();
    while(node->cur != start) {
        res.insert(res.begin(), node->cur);
        node = node->pre;
    }
    return res.size() <= gasVolume ? res : vector<pair<int,int>>{};
}
void findRegion(pair<int,int>& station) {
    vector<pair<int,int>> potentialStars;
    for(pair<int,int>& star: stars) {
        if(manhattanDistance(station, star) <= gasVolume) {
            potentialStars.emplace_back(star);
        }
    }
    if(potentialStars.size() == 1) {
        if(manhattanDistance(station, potentialStars[0]) <= gasVolume) {
            vector<pair<int,int>> path = aStarSearch(station, potentialStars[0]);
            if(path.size() > 0) {
                regions[station].emplace_back(path);
            }
        }
    }else {
        for(pair<int,int>& star: potentialStars) {
            if(manhattanDistance(station, star) <= gasVolume/2) {
                vector<pair<int,int>> path = aStarSearch(station, star);
                if(path.size() > 0) {
                    regions[station].emplace_back(path);
                }
            }
        }
    }
}

class SuperRegion {
public:
    vector<region::iterator> regions;
    void insert(region::iterator it) {
        regions.emplace_back(it);
    }
    int getSize() {
        int size = 0;
        for(int i = 0; i < regions.size(); ++i) {
            size += regions[i]->second.size();
        }
        return size;
    }
};

int main() {
    readFile("test.txt");
    for(pair<int,int>& station: gasStations) {
        findRegion(station);
    }
    vector<SuperRegion> superRegions;
    int idx = 0;
    for(auto it1 = regions.begin(); it1 != regions.end(); ++it1) {
        for(auto it2 = next(it1); it2 != regions.end(); ++it2) {
            if(manhattanDistance(it1->first, it2->first) <= gasVolume) {
                if(idx == superRegions.size()) {
                    SuperRegion spRegion;
                    spRegion.insert(it2);
                    superRegions.emplace_back(spRegion);
                }else {
                    superRegions[idx].insert(it2);
                }
            }
        }
        if(idx == superRegions.size()) {
            SuperRegion spRegion;
            spRegion.insert(it1);
            superRegions.emplace_back(spRegion);
        }else {
            superRegions[idx].insert(it1);
        }
    }
    SuperRegion maxRegion = superRegions[0];
    for(int i = 1; i < superRegions.size(); ++i) {
        if(maxRegion.getSize() < superRegions[i].getSize()) {
            maxRegion = superRegions[i];
        }
    }
    return 0;
}