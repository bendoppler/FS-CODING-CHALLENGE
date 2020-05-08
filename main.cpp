#include<bits/stdc++.h>
using namespace std;

ifstream fin;
pair<int,int> tankStartPosition;
int gasVolume;
int rows, columns;
vector<vector<int>> maps;
map<pair<int, int>, vector<pair<int, int>>> pathsBetween;

void readFile(string filename) {
    fin.open(filename);
    fin >> tankStartPosition.first >> tankStartPosition.second;
    fin >> gasVolume;
    fin >> rows >> columns;
    maps.resize(rows, vector<int>(columns, 0));
    int row = 0;
    while(!fin.eof()) {
        for(int i = 0; i < n; ++i) {
            fin>>maps[row][i];
        }
        ++row;
    }
    fin.close();
}

// void dfs(Point p, vector<Point> paths, int len, set<Point>& mark) {
//     if(mark.count(p)) {
//         return;
//     }
//     if(maps[p.x][p.y] == 1) {
//         mark.insert(p);
//         if(p.x < m-1) {
//             Point next(p.x+1, p.y);
//             dfs(next, paths, len+1, mark);
//         }
//         if(p.x > 0) {
//             Point next(p.x-1, p.y);
//             dfs(next, paths, len+1, mark);
//         }
//         if(p.y < n-1) {
//             Point next(p.x, p.y+1);
//         }
//         if(p.y > 0) {
//             Point next(p.x, p.y-1);
//         }
//     }
// }

int main() {
    readFile("test.txt");

    return 0;
}