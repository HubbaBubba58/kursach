#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <list>
using namespace std;
ofstream fout("cppstudio.txt");

// Класс, представляющий неориентированный граф
class Graph // непоределенный граф
{
    bool** G; // матрица смежности(для вида)
    int Vertex;    // Количество вершин
    bool* visited; //посещенные вершины
    list<int>* adj;   // Динамический массив списков смежности
public:
    // Конструктор и деструктор
    Graph(int Vertex) 
    { 
        this->Vertex = Vertex; 
        adj = new list<int>[Vertex]; 
        visited = new bool[Vertex];
        memset(visited, false, Vertex);
        G = new bool* [Vertex];
        for (int i = 0; i < Vertex; i++) {
            G[i] = new bool[Vertex];
            for (int j = 0; j < Vertex; j++)
                G[i][j] = 0;
        }
    }
    ~Graph() { 
        for (int i = 0; i < Vertex; i++)
            delete[] G[i];
        delete[] G;
        delete[] adj; 
        delete[]visited; 
    }

    //вывод на экран матрицы
    void toString() {
        for (int i = 0; i < Vertex; i++) {
            cout << i << " : ";
            fout << i << " : ";
            for (int j = 0; j < Vertex; j++) {
                cout << G[i][j] << " ";
                fout << G[i][j] << " ";
            }
            cout << endl;
            fout << endl;
        }
    }

    // функция для добавления
    void addEdge(int u, int v) { 
        G[u][v] = G[v][u] = true;

        adj[u].push_back(v); 
        adj[v].push_back(u); 
    }
    //удаление ребёр
    void rmvEdge(int u, int Vertex);

    // Способы печати эйлерова тура
    void print();
    void printEuler(int s);

    // Эта функция возвращает количество вершин, доступных из вершины. Это делает DFS
    int DFS(int Vertex);

    //элеров граф
    bool isValidNextEdge(int u, int Vertex);

    //проверка на эйлеровость
    bool proverka();
};

/* Основная функция, которая выводит эйлерову тропу. Сначала он находит
вершину нечетной степени (если таковая имеется), а затем вызывает printEulerUtil()
 чтобы напечатать путь */
void Graph::print()
{
    // Найти вершину с нечетной степенью
    int u = 0; // временная переменная
    for (size_t i = 0; i < Vertex; i++)
        if (adj[i].size() & 1) {
            u = i; break;
        }
    printEuler(u);
    cout << endl;
    fout << endl;
}

// Вывести тур Эйлера, начинающийся с вершины u
void Graph::printEuler(int u)
{
    // Повторяется для всех вершин, смежных с этой вершиной
    for (auto i = adj[u].begin(); i != adj[u].end(); ++i)
    {
        int Vertex = *i;

        // Если u-вершина ребра не удалена и это допустимое следующее ребро
        if (Vertex != -1 && isValidNextEdge(u, Vertex))
        {
            cout << u << "->" << Vertex << "  ";
            fout << u << "->" << Vertex << "  ";
            rmvEdge(u, Vertex);
            printEuler(Vertex);
        }
    }
}

// Функция для проверки, может ли ребро u-вершина рассматриваться как следующее ребро
// Эйлер Зазывается
bool Graph::isValidNextEdge(int u, int Vertex)
{
    // U-вершина ребра допустима в одном из следующих двух случаев:

     // 1) Если вершина является единственной смежной вершиной u
    int count = 0;  // Для сохранения количества смежных вершин
    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i)
        if (*i != -1)
            count++;
    if (count == 1)
        return true;

    // 2) Если существует несколько смежных, то u-вершина не является мостом
     // Выполните следующие действия, чтобы проверить, является ли u-вершина мостом

     // 2.а) количество вершин, достижимых из u
    visited[Vertex];
    int count1 = DFS(u);

    // 2.b) Удалите ребро (u, v) и после удаления ребра подсчитайте вершины, доступные из u
    rmvEdge(u, Vertex);
    memset(visited, false, Vertex);
    int count2 = DFS(u);

    // 2.c) Добавьте ребро обратно к графику
    addEdge(u, Vertex);

    // 2.d) Если count1 больше, то ребро (u, v) является мостом
    return (count1 > count2) ? false : true;
}

bool Graph::proverka()
{
    int sum = 0;
    for (int i = 0; i < Vertex; i++) {
        sum = 0;
        for (int j = 0; j < Vertex; j++) {
            if (G[i][j] == 1) sum++;
        }
        if (sum % 2) return false; // если степень вершины(хоть одной) нечетная, то граф не считается элеровым
    }
    return true;
}

// Эта функция удаляет u-вершину ребра из графика. Он удаляет ребро, заменяя значение смежной вершины на -1.
void Graph::rmvEdge(int u, int Vertex)
{
    // Найдите вершину в списке смежности u и замените ее на -1
    list<int>::iterator iv = find(adj[u].begin(), adj[u].end(), Vertex);
    *iv = -1;

    // Найдите u в списке смежности вершины и замените его на -1
    list<int>::iterator iu = find(adj[Vertex].begin(), adj[Vertex].end(), u);
    *iu = -1;
}

// Функция на основе DFS для подсчета достижимых вершин из вершины
int Graph::DFS(int start)
{
    // Отметить текущий узел как посещенный
    visited[start] = true;
    int count = 1;

    // Повторяется для всех вершин, смежных с этой вершиной
    for (auto i = adj[start].begin(); i != adj[start].end(); ++i)
        if (*i != -1 && !visited[*i])
            count += DFS(*i);

    return count;
}

int main()
{
    setlocale(0, "");
    srand(time(0));
    int z;
    string s;
    while (true) {
        system("cls");
        cout << "Введите кол-во вершин графа: "; cin >> z;
        Graph c(z);
        cout << endl
            << "|------------------------|" << endl
            << "| 1)Случайное заполнение |" << endl
            << "|------------------------|" << endl
            << "| 2)Заполнить самому     |" << endl
            << "|------------------------|" << endl
            << "| 3)Выход                |" << endl
            << "|------------------------|" << endl;
        cout << "       Введите: ";
        cin >> s;
        string q1, q2, q3;
        switch (atoi(s.c_str()))
        {
        case 1:
            system("cls");

            for (int i = 0; i < z; i++)
            {
                for (int j = i + 1; j < z; j++)
                {
                    if (bool(rand() % 2)) {
                        c.addEdge(i, j);
                    }
                }
            }
            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << "-----------------" << endl << "Матрица смежности: \n";
            c.toString();
            cout << endl << "-----------------" << endl << "Ответ: \n";
            fout << endl << "-----------------" << endl << "Ответ: \n";
            if (c.proverka())
                c.print();
            else
                cout << "Граф является не элеровым" << endl;

            system("pause");
            break;
        case 2:
            system("cls");
            while (1) {
                system("cls");
                cout << "Для выхода введите *" << endl;
                cout << "Введите 1-вершину: "; cin >> q1;
                if (q1 == "*") break;
                cout << "Введите 2-вершину: "; cin >> q2;
                if (q1 == "*") break;
                c.addEdge(atoi(q1.c_str()), atoi(q2.c_str()));
                cout << endl;
                c.toString();
                system("pause");
            }
            system("cls");
            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << "-----------------" << endl << "Матрица смежности: \n";
            c.toString();
            cout << endl << "-----------------" << endl << "Ответ: \n";
            fout << endl << "-----------------" << endl << "Ответ: \n";
            if (c.proverka())
                c.print();
            else
                cout << "Граф является не элеровым"<<endl;

            system("pause");
            break;
        case 3:
            exit(0);
            break;
        }
        system("cls");
    }

    fout.close();
    return 0;
}