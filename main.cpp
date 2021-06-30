//Вариант без печатолок с map и double
#include <iostream>
#include <map>
#include <vector>
#include <deque>
#include <limits>
#include <time.h>
#include <fstream>

using namespace std;

const int _infinum = numeric_limits<int>::max();

template <typename T>
void push (int num, int* e, T& edge, map < pair <int, int>, double>& graph){
    e[edge->first.second] += num;
    e[edge->first.first] -= num;
    graph[{edge->first.second, edge->first.first}] += num; //Вычисляем остаточную пропускную способность противоположного ребра
    graph[{edge->first.first, edge->first.second}] -= num; //Вычисляем остаточную пропускную способность ребра
}

int* BFS (int* d, int n, map < pair <int, int>, double>& graph){
    d[n-1] = 0;
    deque<int> dec;
    dec.push_back(n-1);
    while (!dec.empty()){
        int i = dec.front();
        dec.pop_front();
        for (auto edge = graph.begin(); edge != graph.end(); ++edge){
            if (edge->first.second == i){
                if (d[edge->first.first] == _infinum){
                    d[edge->first.first] = d[edge->first.second] + 1;
                    dec.push_back(edge->first.first);
                }
            }
        }
    }
    return d;
}

int* GlobalRelabeling (int n, int* h, map < pair <int, int>, double>& graph){
    int* d1 = new int[n];
    fill(d1, d1 + n, _infinum);
    int* d2 = new int[n];
    fill(d2, d2 + n, _infinum);
    d1 = BFS (d1, n, graph); //Ищем расстояние от всех вершин до n при помощи обхода графа в ширину по ребрам в обратном направлении
    d2 = BFS (d2, 1, graph); //Ищем расстояние от всех вершин до 0 при помощи обхода графа по его обратным ребрам в обратном направлении
    for (int i = 0; i < n; ++i){
        if (d1[i] == _infinum){
            h[i] = d2[i];
        }
        else
            h[i] = d1[i];
    }
    return h;
}

int maxFlow (int n, map < pair <int, int>, double> graph) {
    deque<int> dec; //Очередь из вершин, в которых есть избыток

    int *h = new int[n]; //Массив высот
    int *e = new int[n]; //Массив избытков

    // Инициализация
    h = GlobalRelabeling (n, h, graph);
    for (int i = 0; i < n; ++i) {
        e[i] = 0;
    }

    // Пускаем максимальный поток из истока
    auto edge = graph.begin();
    while (edge != graph.end()) {
        if (edge->first.first == 0) {
            e[edge->first.second] += edge->second;
            e[edge->first.first] -= edge->second;
            graph[{edge->first.second, edge->first.first}] += edge->second;
            if (edge->first.second != n-1)
                dec.push_back(edge->first.second);
            graph.erase(edge++);
        } else
            ++edge;
    }
    h[0] = n;

    while (!dec.empty()) { //Пока в очереди есть вершины
        int i = dec.front(); //Получаем первый элемент очереди
        dec.pop_front(); //Удаляем этот первый элемент
        int minH = _infinum;
        while (e[i] > 0) { //Пока в текущей вершине есть избыток
            auto edge = graph.begin();
            while (edge != graph.end()) {
                if (edge->first.first == i) { //Находим ребро, выходящее из текущей вершины
                    if (h[i] - 1 == h[edge->first.second]) { //Если текущая вершина на 1 выше той, в которую ведет ребро, то делаем push:
                        if (edge->first.second != n-1 && edge->first.second != 0) { //Если вершина, в которую проталкиваем, не является истоком/стоком и не находится в очереди, то добавляем ее туда
                            bool signDec = true;
                            for (int k = 0; k < dec.size(); ++k) {
                                if (dec[k] == edge->first.second)
                                    signDec = false;
                            }
                            if (signDec)
                                dec.push_back(edge->first.second);
                        }
                        if (e[i] >= edge->second) { //Если избыток в вершине больше, чем остаточная пропускная способность ребра
                            push(edge->second, e, edge, graph);//то проталкиваем количество единиц, равное остаточной пропускной способности
                            graph.erase(edge++); //Удаляем текущее ребро, поскольку его пропускная способность стала равна 0
                        } else {//Если избыток в вершине меньше, чем остаточная пропускная способность ребра
                            push(e[i], e, edge, graph);//то проталкиваем количество единиц, равное избытку
                            edge++;
                            break; //Переходим к следующей вершине, поскольку избыток в текущей стал равен 0
                        }

                    } else { //Среди тех вершин, которые выше, либо на том же уровне, что текущая, ищем ту, высота которой минимальна (этот показатель необходим для операции relabel)
                        if (minH > h[edge->first.second]) {
                            minH = h[edge->first.second];
                        }
                        edge++;
                    }
                } else {
                    if (edge->first.first > i)//Если текущее ребро выходит из вершины с номером большим i, то все исходящие из i ребра уже были рассмотрены (в силу самосортируемости map)
                        break;
                    edge++;
                }
            }
            if (e[i] > 0) { //Если после обхода всех ребер в вершине остался избыток, то
                h[i] = minH + 1; // relabel
                dec.push_back(i); //Добавляем эту вершину в конец очереди
                break; //Переходим к рассмотрению новой вершины
            }
        }
    }
    return e[n-1];
}

int main() {
    srand( time(0) );
    ifstream dataFile;
    dataFile.open("/home/nata/1.SPBU_homework/6_Теория конечных графов и ее приложения/PreflowPushingAlg(FIFO+GR)/Tests/test_rd06.txt");
    if (!dataFile.is_open())
        cout << "File is't open!" << endl;
    else{
        int n, m;
        int v1, v2;
        double c;
        map < pair <int, int>, double> graph;
        dataFile >> n;
        dataFile >> m;
        for (int i = 0; i < m; ++i){
            dataFile >> v1;
            dataFile >> v2;
            dataFile >> c;
            graph.insert ( pair <pair<int,int>, double>({v1-1,v2-1}, c) );
        }

        int mF;
        clock_t start, end;
        start = clock();
        mF = maxFlow (n, graph);
        end = clock();
        double t = (double)(end - start) / CLOCKS_PER_SEC;
        cout << "maxFlow = " << mF << endl;
        cout << "TIME: " << t << endl;
    }
    return 0;
}