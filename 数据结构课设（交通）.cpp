#include"数据结构课设（交通）.h";

//默认构造函数
MatGraph::MatGraph() : n(0) {
    for (int i = 0; i < MAXV; i++) {
        cities[i] = { 0, "", 0 };
        for (int j = 0; j < MAXV; j++) {
            priceMatrix[i][j] = (i == j) ? 0 : INF;
            timeMatrix[i][j] = (i == j) ? 0 : INF;
            edgeInfo[i][j] = { "", "", 0, 0 };
        }
    }
}

//添加城市
void MatGraph::addCity(int no, const string& name, int transferTime) {
    if (no >= 0 && no < MAXV && cities[no].name.empty()) {
        cities[no] = { no, name, transferTime };
        if (no >= n) n = no + 1;
        cout << name << " 添加成功！" << endl;
    }
    else {
        cout << "编号无效或已存在！" << endl;
    }
}

//删除城市
void MatGraph::deleteCity(int no) {
    if (no >= 0 && no < MAXV && !cities[no].name.empty()) {
        cities[no] = { 0, "", 0 };
        // 清除相关边信息
        for (int i = 0; i < MAXV; i++) {
            priceMatrix[i][no] = INF;
            timeMatrix[i][no] = INF;
            priceMatrix[no][i] = INF;
            timeMatrix[no][i] = INF;
            edgeInfo[i][no] = { "", "", 0, 0 };
            edgeInfo[no][i] = { "", "", 0, 0 };
        }
        cout << "删除成功！" << endl;
    }
    else {
        cout << "编号不存在！" << endl;
    }
}

//添加列车（更新邻接矩阵）
void MatGraph::addTrain(int start, int end, const string& departure, const string& arrival, int price) {
    if (!isValidTime(departure) || !isValidTime(arrival)) {
        cout << "无效的时间格式！请使用HH:MM格式（例如：08:30）。" << endl;
        return;
    }

    if (timeDifference(departure, arrival) <= 0) {
        cout << "到达时间必须晚于出发时间！" << endl;
        return;
    }

    if (start >= 0 && start < MAXV && end >= 0 && end < MAXV &&
        !cities[start].name.empty() && !cities[end].name.empty()) {

        int duration = timeDifference(departure, arrival);

        //更新时间矩阵（选择最快到达的列车）
        if (duration < timeMatrix[start][end]) {
            timeMatrix[start][end] = duration;
            edgeInfo[start][end] = { departure, arrival, price, duration };
        }

        //更新票价矩阵（选择最便宜的列车）
        if (price < priceMatrix[start][end]) {
            priceMatrix[start][end] = price;
            if (duration >= timeMatrix[start][end]) {
                edgeInfo[start][end].price = price;
            }
        }

        cout << "列车信息更新成功！" << endl;
    }
    else {
        cout << "操作失败，不存在！" << endl;
    }
}


// 查询列车
void MatGraph::queryTrains(int start, int end)  {
    if (start >= 0 && start < MAXV && end >= 0 && end < MAXV &&
        !cities[start].name.empty() && !cities[end].name.empty()) {

        if (timeMatrix[start][end] == INF) {
            cout << "没有从 " << cities[start].name << " 到 " << cities[end].name << " 的列车！" << endl;
        }
        else {
            const Edge& edge = edgeInfo[start][end];
            cout << "从 " << cities[start].name << " 到 " << cities[end].name << " 的最优列车：" << endl;
            cout << "出发时间: " << edge.departureTime
                << " 到达时间: " << edge.arrivalTime
                << " 时长: " << edge.duration << "分钟"
                << " 票价: " << priceMatrix[start][end] << endl;
        }
    }
    else {
        cout << "城市编号不存在！" << endl;
    }
}

// 从文件读取数据
void MatGraph::readFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        return;
    }

    // 重置数据
    n = 0;
    for (int i = 0; i < MAXV; i++) {
        cities[i] = { 0, "", 0 };
        for (int j = 0; j < MAXV; j++) {
            priceMatrix[i][j] = (i == j) ? 0 : INF;
            timeMatrix[i][j] = (i == j) ? 0 : INF;
            edgeInfo[i][j] = { "", "", 0, 0 };
        }
    }

    file >> n;

    // 读取城市信息
    for (int i = 0; i < n; i++) {
        int no, transferTime;
        string name;
        file >> no >> name >> transferTime;
        cities[no] = { no, name, transferTime };
    }

    // 读取列车信息
    int trainCount;
    file >> trainCount;
    for (int i = 0; i < trainCount; i++) {
        int start, end, price;
        string departure, arrival;
        file >> start >> end >> departure >> arrival >> price;

        int duration = timeDifference(departure, arrival);

        if (duration < timeMatrix[start][end]) {
            timeMatrix[start][end] = duration;
            edgeInfo[start][end] = { departure, arrival, price, duration };
        }

        if (price < priceMatrix[start][end]) {
            priceMatrix[start][end] = price;
            if (duration >= timeMatrix[start][end]) {
                edgeInfo[start][end].price = price;
            }
        }
    }

    file.close();
    cout << "数据加载成功！" << endl;
}

// 保存数据到文件
void MatGraph::saveToFile(const string& filename){
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件 " << filename << endl;
        return;
    }

    file << n << endl;

    // 保存城市信息
    for (int i = 0; i < MAXV; i++) {
        if (!cities[i].name.empty()) {
            file << cities[i].no << " "
                << cities[i].name << " "
                << cities[i].transferTime << endl;
        }
    }

    // 计算并保存列车总数
    int trainCount = 0;
    for (int i = 0; i < MAXV; i++) {
        for (int j = 0; j < MAXV; j++) {
            if (timeMatrix[i][j] != INF && i != j) {
                trainCount++;
            }
        }
    }
    file << trainCount << endl;

    // 保存列车信息（以时:分形式保存）
    for (int i = 0; i < MAXV; i++) {
        for (int j = 0; j < MAXV; j++) {
            if (timeMatrix[i][j] != INF && i != j) {
                const Edge& edge = edgeInfo[i][j];
                file << i << " " << j << " "
                    << edge.departureTime << " "
                    << edge.arrivalTime << " "
                    << priceMatrix[i][j] << endl;
            }
        }
    }

    file.close();
    cout << "数据保存成功！" << endl;
}

// 最优路径查询
void MatGraph::findOptimalPath(int start, int end, int strategy){
    if (start < 0 || start >= MAXV || end < 0 || end >= MAXV ||
        cities[start].name.empty() || cities[end].name.empty()) {
        cout << "编号不存在！" << endl;
        return;
    }

    if (start == end) {
        cout << "起点和终点相同！" << endl;
        return;
    }

    if (strategy == 1) {
        findFastestPath(start, end);
    }
    else if (strategy == 2) {
        findCheapestPath(start, end);
    }
    else {
        cout << "无效！" << endl;
    }
}

//验证时间格式是否正确
bool isValidTime(const string& timeStr) {
    int hours, minutes;
    char colon;
    stringstream ss(timeStr);
    if (ss >> hours >> colon >> minutes && colon == ':') {
        return hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60;
    }
    return false;
}

//计算两个时间之间的分钟差
int timeDifference(const string& startTime, const string& endTime) {
    int startHours, startMinutes, endHours, endMinutes;
    char colon;

    stringstream ss1(startTime);
    ss1 >> startHours >> colon >> startMinutes;

    stringstream ss2(endTime);
    ss2 >> endHours >> colon >> endMinutes;

    return (endHours * 60 + endMinutes) - (startHours * 60 + startMinutes);
}


//查找最快路径
void MatGraph::findFastestPath(int start, int end){
    vector<int> dist(MAXV, INF);
    vector<int> parent(MAXV, -1);
    vector<bool> visited(MAXV, false);

    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        int minDist = INF;
        int u = -1;

        //找到未访问节点中距离最小的节点
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        //更新所有邻接节点的距离
        for (int v = 0; v < n; v++) {
            if (!visited[v] && timeMatrix[u][v] != INF) {
                //考虑中转时间
                int newDist = dist[u] + timeMatrix[u][v] + cities[v].transferTime;
                if (v == end) newDist -= cities[v].transferTime; // 终点不需要中转时间

                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    parent[v] = u;
                }
            }
        }
    }

    if (dist[end] == INF) {
        cout << "无法从 " << cities[start].name << " 到达 " << cities[end].name << endl;
        return;
    }

    //构建路径
    vector<int> path;
    int current = end;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    //输出结果
    cout << "最快路径（总时间: " << dist[end] << "分钟）：" << endl;
    for (size_t i = 0; i < path.size() - 1; i++) {
        int u = path[i];
        int v = path[i + 1];

        cout << "从 " << cities[u].name << " 到 " << cities[v].name
            << " 乘坐列车"
            << " 出发时间: " << edgeInfo[u][v].departureTime
            << " 到达时间: " << edgeInfo[u][v].arrivalTime
            << " 票价: " << priceMatrix[u][v] << endl;

        if (i < path.size() - 2) {
            cout << "在 " << cities[v].name << " 中转等待 "
                << cities[v].transferTime << " 分钟" << endl;
        }
    }
}

//查找最省钱路径
void MatGraph::findCheapestPath(int start, int end){
    vector<int> dist(MAXV, INF);
    vector<int> parent(MAXV, -1);
    vector<bool> visited(MAXV, false);

    dist[start] = 0;

    for (int i = 0; i < n - 1; i++) {
        int minDist = INF;
        int u = -1;

        //找到未访问节点中距离最小的节点
        for (int j = 0; j < n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;

        visited[u] = true;

        //更新所有邻接节点的距离
        for (int v = 0; v < n; v++) {
            if (!visited[v] && priceMatrix[u][v] != INF) {
                int newDist = dist[u] + priceMatrix[u][v];
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    parent[v] = u;
                }
            }
        }
    }

    if (dist[end] == INF) {
        cout << "无法从 " << cities[start].name << " 到达 " << cities[end].name << endl;
        return;
    }

    //构建路径
    vector<int> path;
    int current = end;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    //输出结果
    cout << "最省钱路径（总票价: " << dist[end] << "）：" << endl;
    for (size_t i = 0; i < path.size() - 1; i++) {
        int u = path[i];
        int v = path[i + 1];

        cout << "从 " << cities[u].name << " 到 " << cities[v].name
            << " 乘坐列车"
            << " 出发时间: " << edgeInfo[u][v].departureTime
            << " 到达时间: " << edgeInfo[u][v].arrivalTime
            << " 票价: " << priceMatrix[u][v] << endl;

        if (i < path.size() - 2) {
            cout << "在 " << cities[v].name << " 中转等待 "
                << cities[v].transferTime << " 分钟" << endl;
        }
    }
}