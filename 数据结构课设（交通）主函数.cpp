#include"数据结构课设（交通）.h";

int main() {
    MatGraph graph;
    graph.readFromFile("trains.txt");

    int choice;
    do {
        cout << "======列车查询系统======" << endl;
        cout << "1.添加城市" << endl;
        cout << "2.删除城市" << endl;
        cout << "3.添加列车" << endl;
        cout << "4.查询列车" << endl;
        cout << "5.查询最优路径" << endl;
        cout << "6.保存数据到文件" << endl;
        cout << "0.退出" << endl;
        cout << "请输入选择: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int no, transferTime;
            string name;
            cout << "请输入城市编号: ";
            cin >> no;
            cout << "请输入城市名称: ";
            cin >> name;
            cout << "请输入中转时间(分钟): ";
            cin >> transferTime;
            graph.addCity(no, name, transferTime);
            break;
        }
        case 2: {
            int no;
            cout << "请输入要删除的城市编号: ";
            cin >> no;
            graph.deleteCity(no);
            break;
        }
        case 3: {
            int start, end, price;
            string departure, arrival; 
            cout << "请输入起始站编号:  ";
            cin >> start;
            cout << "请输入终点站编号:  ";
            cin >> end;
            cout << "请输入出发时间(HH:MM格式):  ";
            cin >> departure;
            cout << "请输入到达时间(HH:MM格式):  ";
            cin >> arrival;
            cout << "请输入票价: ";
            cin >> price;
            graph.addTrain(start, end, departure, arrival, price);
            break;
        }
        case 4: {
            int start, end;
            cout << "请输入起始站编号:  ";
            cin >> start;
            cout << "请输入终点站编号:  ";
            cin >> end;
            graph.queryTrains(start, end);
            break;
        }
        case 5: {
            int start, end, strategy;
            cout << "请输入起始站编号:  ";
            cin >> start;
            cout << "请输入终点站编号:  ";
            cin >> end;
            cout << "1.最快到达 ";
            cout << "2.最省钱到达 ";
            cout << "请输入您的咨询方案: ";
            cin >> strategy;
            graph.findOptimalPath(start, end, strategy);
            break;
        }
        case 6: {
            graph.saveToFile("trains.txt");
            break;
        }
        case 0:
            cout << "退出！" << endl;
            break;
        default:
            cout << "无效！" << endl;
        }
    } while (choice != 0);

    return 0;
}