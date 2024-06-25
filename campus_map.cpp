// campus_map.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*	
    1.设计一个所在学校的建筑和道路平面图，所含建筑不少于10个，并建立全局坐标，按照全局右手定则确定角度；
	2.存储各建筑点的信息，包括位置坐标（二维）和简要介绍（例如：G栋，理学楼）；
	3.提供图中任意建筑点的相关信息查询，即输入关键字可输出相关信息（例如：输入G栋；给出结果：G栋，理学楼，坐标（2, 3））；
	4.提供图中多个建筑点的最佳访问路线查选，即求途经n（任意）个景点的最短路径，并给出所经过的点的方位和长度。
        （给定开始和结束景点，并一共包含n个景点，从A栋到G栋，途径A - B - C - D - F - G，在B点位于A点30°方向并距离A点5）；
	5.提供任意建筑点问路查询，即查询某个建筑到其他任意一个建筑点的最短路径，并按照路径长度从小到大的顺序排列（不能使用迪杰斯特拉算法）。
*/
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct building//建筑信息结构体
{
    string name;//建筑名称
    int x;//坐标
    int y;
    string information;//建筑信息
};

const int BUILDING_NUMBER=16;

//结构数组存储所有建筑信息
building arr[BUILDING_NUMBER] = 
{
  //序号，名称，坐标，信息
    {"校门",0,0,"平山一路大门"},
    {"一食堂",70,30,"一食堂"},
    {"三食堂",210,-100,"三食堂"},
    {"A",170,200,"研究生院"},
    {"B",250,250,"社科楼"},
    {"C",140,300,"机电楼"},
    {"D",0,300,"材料楼"},
    {"E",-80,300,"水木楼"},
    {"F",70,300,"学术楼"},
    {"G",300,150,"理学楼"},
    {"H",0,530,"主楼"},
    {"J",460,40,"活动中心"},
    {"K",460,120,"实训楼"},
    {"L",460,200,"信息楼"},
    {"T",420,-70,"本科教学楼"},
    {"LY",270,-100,"荔园789栋"},

};

//各点路径长度
const int MAX_NUMBER = 9999;//定义一个很长的路径代表不通,直接用INT_MAX会导致后续计算溢出
 int weight[BUILDING_NUMBER][BUILDING_NUMBER] = {
    {MAX_NUMBER,   80	,       230,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {80,	        MAX_NUMBER,	190,	    260,	    MAX_NUMBER,	340,	    MAX_NUMBER,	310,	    270,	    320,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {230,	        190,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	50},
    {MAX_NUMBER,	260,	    MAX_NUMBER,	MAX_NUMBER,	80,	        90,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	140,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	80,	        MAX_NUMBER,	130,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	70,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	340,	    MAX_NUMBER,	90,	        130,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	70,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	80,	        70,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	310,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	80,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	270,	    MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	70,	        70,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	320,        MAX_NUMBER,	140,	    70,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	170,	    MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	70,	        MAX_NUMBER,	MAX_NUMBER,	170,	    MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER, MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	70,	        MAX_NUMBER,	80,	        MAX_NUMBER,	90,	        MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	80,	        MAX_NUMBER,	80,	        MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER, MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	170,	    MAX_NUMBER,	MAX_NUMBER,	80,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER},
    {MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER, MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	170,	    90,	        MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	180},
    {MAX_NUMBER,	MAX_NUMBER,	50,     	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	MAX_NUMBER,	180,	    MAX_NUMBER},

};

int path_matrix[BUILDING_NUMBER][BUILDING_NUMBER];//最短路径矩阵
int short_path_table[BUILDING_NUMBER][BUILDING_NUMBER];//路径长度矩阵

void campus_maps();//地图
double angle(int front, int behind);//计算两点方向
int find_sequence(string name);//找到存储建筑所在数组序号

void short_path_floyd(int weight[][BUILDING_NUMBER], int path_matrix[][BUILDING_NUMBER], int short_path_table[][BUILDING_NUMBER]);//计算所有最短路径
void path_print(int start, int end, int path_arr[]);//打印路径
void direction_print(int end,int path_arr[]);//打印方向
void length_sort_all(int sequence,int end_arr[], int path_matrix[][BUILDING_NUMBER], int short_path_table[][BUILDING_NUMBER]);//按路径长度排序
//-----------------------------------------
bool visited[BUILDING_NUMBER];
void dfs(int start,int stop,int node_number);//dfs递归搜索路径
int stack[MAX_NUMBER], m = 1;///存储路径,m为栈指针
int path_length = 0;//路径长度
int path_amount = 0;//路径数量
struct all_path
{
    int path_arr[BUILDING_NUMBER];//储存途径信息
    int node_amount;//途径点数量
    int path_total_length;//本条路径长度
};
all_path path_vector[MAX_NUMBER];//结构数组，储存每条路径信息
void dfs_sort_print(all_path path_vector[], int node_number, int path_amount, int fit_path_number);//将DFS搜索后的结果排序并输出
//--------------------------------------------------
int main()
{
    campus_maps();//校园地图全貌
    short_path_floyd(weight, path_matrix, short_path_table);//求取所有最短路径
    char mode;
    int a=1;//循环标志
    while (a==1)
    {
        cout << "输入1进入查询模式，2进入问路模式，3进入路径规划模式，q退出：" << endl;

        cin >> mode;

        while (mode != '1' && mode != '2' && mode != '3' && mode != 'q' && mode != 'Q')
        {
            cout << "输入有误，请重新输入！\n输入1进入查询模式，2进入问路模式，3进入路径规划模式，q退出：" << endl;
            cin >> mode;
        }
        //-----------------------------------------------
        while (mode == '1')//查询模式
        {
            string find_name;
            cout << "你想要查：";
            cin >> find_name;
            int sequence = find_sequence(find_name);
            while (sequence == -1)
            {
                cout << "无此信息，请重新输入：";
                cin >> find_name;
                sequence = find_sequence(find_name);
            }
            cout << "所查信息为：" << arr[sequence].name << "  简介：" << arr[sequence].information << "  坐标：（ " << arr[sequence].x << " , " << arr[sequence].y << " ） " << endl;
            cout << endl; 
            break;
        };
        //------------------------------------------------
        while (mode == '2')//问路模式,查询指定点到所有点的最短路径
        {
            string start_name;
            cout << "输入起点：";
            cin >> start_name;
            int sequence = find_sequence(start_name);
            while (sequence == -1)
            {
                cout << "无此信息，请重新输入：";
                cin >> start_name;
                sequence = find_sequence(start_name);
            }

            int end_arr[BUILDING_NUMBER - 1];//定义一个终点数组
            int j = 0;
            for (int i = 0; i < BUILDING_NUMBER; i++)
            {
                //int j = 0;
                if (i != sequence)
                {
                    end_arr[j] = i;
                    j++;
                }

            }
            length_sort_all(sequence, end_arr, path_matrix, short_path_table);//对给定起点到所有终点按路径长度进行排序
            cout << "从这里到其他所有地方的最短路径为：" << endl << endl;
            for (int i = 0; i < BUILDING_NUMBER - 1; i++)
            {
                int path_arr[BUILDING_NUMBER - 1];
                path_print(sequence, end_arr[i], path_arr);
                cout << endl;
            }
            cout << endl;
            break;
        }
        //-------------------------------------------------
        while (mode == '3')//路径规划模式
        {
            cout << "输入起点和终点：";
            
            string front, behind;
            cin >> front >> behind;
            int front_sequence = find_sequence(front);
            int behind_sequence = find_sequence(behind);
            while (front_sequence == -1 || behind_sequence == -1)
            {
                cout << "无此信息，请重新输入：";
                cin >> front >> behind;
                front_sequence = find_sequence(front);
                behind_sequence = find_sequence(behind);
            }

            cout << "输入途径景点数量：";
            int node_number;
            cin >> node_number;
            stack[0] = front_sequence;//将起点入栈
            for (int i = 0; i < BUILDING_NUMBER; i++)//初始化所有未访问
                visited[i] = false;
            visited[front_sequence]=true;//初始化起点为访问
            dfs(front_sequence, behind_sequence, node_number);//求解
            int fit_path_number=0;//符合要求的路径数目
            
            dfs_sort_print(path_vector, node_number, path_amount, fit_path_number);
            cout << endl; 
            path_amount = 0;//路径数重置为0
            break;
            
        };
        while (mode == 'q' || mode == 'Q')//退出
        {
            cout << "谢谢，再见！\n";
            a = 0;
            cout << endl;
            break;
        };

    }
}
void dfs_sort_print(all_path path_vector[], int node_number, int path_amount, int fit_path_number )
{
    //vector<all_path> path[path_amount];
    for (int i = 0; i < path_amount; i++)//统计符合所要求结点数量的路径
    {
        if (path_vector[i].node_amount == node_number)
        {
            path_vector[fit_path_number] = path_vector[i];
            fit_path_number++;
        }
    }
    if (fit_path_number == 0)
        cout << "没有符合要求的路径" << endl;
    else//排序并输出
    {
        for (int i = 0; i < fit_path_number; i++)//排序
        {
            for (int j = i; j < fit_path_number; j++)
                if (path_vector[i].path_total_length > path_vector[j].path_total_length)
                    swap(path_vector[i], path_vector[j]);
            
        }
        for (int i = 0; i < fit_path_number; i++)
        {
            for (int j = 0; j < path_vector[i].node_amount; j++)
                cout <<left<<setw(12)<< arr[path_vector[i].path_arr[j]].information << "  ";
            cout << "路径长度为  " << path_vector[i].path_total_length;
            cout << endl;
        }
        cout << endl;
        cout << "推荐路径：" << endl;
        for (int i = 0; i < path_vector[0].node_amount-1; i++)
            cout << arr[path_vector[0].path_arr[i]].information << "--->";
        cout << arr[path_vector[0].path_arr[path_vector[0].node_amount- 1] ].information << endl;
        cout << "路径长度为  " << path_vector[0].path_total_length<<endl;
        cout << "请根据以下导航走" << endl;
        direction_print(path_vector[0].path_arr[path_vector[0].node_amount - 1], path_vector[0].path_arr);

    }
}
//------------------------------------------
void campus_maps()//打印地图
{
    cout << " _______________________________________________________________________________________________________" << endl;
    cout << "|-------------------------------哈尔滨工业大学（深圳）平面图--------------------------------------------|" << endl;
    cout << "|                                                                                                       |" << endl;
    cout << "|     *(E栋）-----*（D栋）------*(F栋）------*---------\\                                                |" << endl;
    cout << "|     |           |           /            /  （C栋）   \\------*（B栋）                                 |" << endl;
    cout << "|     |            \\        /___________/                \\     /     \\        /-----------*（L栋）      |" << endl;
    cout << "|     |               \\   /                               \\   /        ----*(G栋）        |             |" << endl;
    cout << "|     |_________________\\                           （A栋) *______________/               *（K栋）      |" << endl;
    cout << "|                        \\ ________________________________\\____________/                 |             |" << endl;
    cout << "|                         \\                                                               *（J栋）      |" << endl;
    cout << "|                      ____*_____________                                                 |             |" << endl;
    cout << "|                    /   (一食堂)        \\                                                /\\            |" << endl;
    cout << "|                  /                      \\                                              /  \\           |" << endl;
    cout << "|                 * _______________________\\                                            /     *（H栋）  |" << endl;
    cout << "|               (校门)                      \\                                        __*(T)___/         |" << endl;
    cout << "|                                            \\   （荔园789栋）                      /                   |" << endl;
    cout << "|                                             *_______*____________________________/                    |" << endl;
    cout << "|                                          （三食堂）                                                   |" << endl;
    cout << "|                                                                                                       |" << endl;
    cout << "|                                                                                                       |" << endl;
    cout << "|                                                                                                       |" << endl;
    cout << "|_______________________________________________________________________________________________________|" << endl;
    cout << endl;
    cout << endl;
    cout << " __________________________________________________"<<endl;
    cout << "|                                                  |" << endl;
    cout << "|     欢迎使用哈尔滨工业大学（深圳）地图向导       |" << endl;
    cout << "|                                                  |" << endl;
    cout << "|            请根据提示进行相关操作                |" << endl;
    cout << "|                                                  |" << endl;
    cout << "|               已收录的信息如下                   |" << endl;
    cout << "| 校门 一食堂  三食堂 A B C D E F G H J K L T LY   |" << endl;
    cout << "|__________________________________________________|" << endl;
    cout << endl;
    cout << endl;
}
//按路径长度排序
void length_sort_all(int sequence, int end_arr[], int path_matrix[][BUILDING_NUMBER], int short_path_table[][BUILDING_NUMBER])
{
    for (int i = 0; i < BUILDING_NUMBER - 1; i++)
    {
        for (int j = i + 1; j < BUILDING_NUMBER - 1; j++)
        {
            if (short_path_table[sequence][end_arr[i]] > short_path_table[sequence][end_arr[j]])
                swap(end_arr[i], end_arr[j]);
                
        }
    }
}
//--------------------------------------------------------------------------
//Floyd算法求最短路径
void short_path_floyd(int weight[][BUILDING_NUMBER], int path_matrix[][BUILDING_NUMBER], int short_path_table[][BUILDING_NUMBER])
{
    int v, w, k;
    for (v = 0; v < BUILDING_NUMBER; v++)//初始化  path_matrix  和  short_path_table
    {
        for (w = 0; w < BUILDING_NUMBER; w++)
        {
            short_path_table[v][w] = weight[v][w];
            path_matrix[v][w] = w;
        }
    }
    for (k = 0; k < BUILDING_NUMBER; k++)
    {
        for (v = 0; v < BUILDING_NUMBER; v++)
        {
            for (w = 0; w < BUILDING_NUMBER; w++)
            {
                if (short_path_table[v][w] > short_path_table[v][k] + short_path_table[k][w])
                {
                    short_path_table[v][w] = short_path_table[v][k] + short_path_table[k][w];
                    path_matrix[v][w] = path_matrix[v][k];
                }
            }
        }
    }
    //打印两个矩阵
    /*
    for (int i = 0; i < BUILDING_NUMBER; i++)
    {
        for (int j = 0; j < BUILDING_NUMBER; j++)
        {
            cout << weight[i][j] << "    ";
        }
        cout << endl;
    }
    for (int i = 0; i < BUILDING_NUMBER; i++)
    {
        for (int j = 0; j < BUILDING_NUMBER; j++)
        {
            cout << short_path_table[i][j]<<"    ";
        }
        cout << endl;
    }
    for (int i = 0; i < BUILDING_NUMBER; i++)
    {
        for (int j = 0; j < BUILDING_NUMBER; j++)
        {
            cout << path_matrix[i][j]<<"    ";
        }
        cout << endl;
    }
    */
}
//----------------------------------------------------------------------
void path_print(int start, int end,int path_arr[]) //打印最短路径
{

    cout << arr[start].information << " ---> ";//打印起点
    int i = 0;
    path_arr[i] = start;
    int k = path_matrix[start][end];
    while (k != end)
    {
        i++;
        path_arr[i] = k;
        cout << arr[k].information << " ---> ";
        k = path_matrix[k][end];
    }
    i++;
    path_arr[i] = k;
    cout << arr[end].information << endl;
    cout << "路径总长为：" << short_path_table[start][end] << endl;
    
}
//----------------------------------------------------------------------
void direction_print(int end,int path_arr[])//打印路径方位
{
    cout << endl;
    int i = 0;
    while (path_arr[i+1] != end)
    {
        
        int k = path_arr[i];
        int j = path_arr[i + 1];
        double direction = angle(k, j);
        cout <<  arr[j].information <<" 在 " << " " << arr[k].information <<"  "<<direction<<"°方向上，和" << " " << arr[k].information <<"相距"<<weight[k][j]<<" 米\n";
        i++;
    }
    int k = path_arr[i];
    int j = path_arr[i + 1];
    double direction = angle(k, j);
    cout << arr[j].information << " 在 "  << " " << arr[k].information << "  " << direction << "°方向上，和" <<  " " << arr[k].information << "相距" << weight[k][j] << " 米\n";

}

//-------------------------------------------------------------------------
double angle(int front, int behind)//求方位角
{
    double x1 = arr[front].x;
    double y1 = arr[front].y;
    double x2 = arr[behind].x;
    double y2 = arr[behind].y;
    
    double angle_rad=atan( (y1 - y2)/ (x1 - x2));//计算两点间的方向（弧度表示）
    angle_rad = angle_rad * 180 / acos(-1);//返回角度表示
    if (x2 <= x1 && y2 > y1)
        return angle_rad + 180;
    else
        if (x2 <= x1 && y2 < y1)
            return angle_rad - 180;
        else
            if (x2 < x1 && y2 == y1)
                return angle_rad + 180;
            else
                if (x2 > x1&& y2 == y1)
                    return 0;
                else
                    return angle_rad;

        
}
//------------------------------------------------------
int find_sequence(string name)
{
    int i = 0;
   
    for (i; i < BUILDING_NUMBER; i++)
    {
        if (arr[i].name == name)
        {
            
            return i;
            break; 
            
        };
        
    };
    return -1;//没找到返回-1
}
//------------------------------------------------
void dfs(int start,int stop,int node_number)//深度优先搜索路径
{
    int i, j;
    for (i = 0; i < BUILDING_NUMBER; i++)
    {
        if (weight[start][i] != MAX_NUMBER&& m<node_number&&visited[i]==false)
        {
            if (i == stop)//如果深搜到了终点，就输出刚才经过的路径
            {
                path_length += weight[start][i];

                for (j = 0; j < m; j++)
                {
                    //printf("%-5d", stack[j]);
                    path_vector[path_amount].path_arr[j] = stack[j];//存路径

                }
                //printf("%-5d\n", stop);
                path_vector[path_amount].path_arr[m] = stop;//存终点
                path_vector[path_amount].node_amount = m + 1;//存途径点的数量
                //cout << "此条路径长度为" << path_length << endl;
                path_vector[path_amount].path_total_length = path_length;//存长度
                path_amount++;//每到一次终点，路径数目+1
                path_length -= weight[start][i];
            }
            else///如果该点不是终点
            {
                visited[i] = true;
                path_length += weight[start][i];
                int length = weight[i][start];//暂存路长信息
                weight[start][i] = MAX_NUMBER;
                weight[i][start] = MAX_NUMBER;
                stack[m] = i;//将该点存起来
                m++;
                dfs(i,stop,node_number);//接着深搜
                visited[i] = false;
                weight[start][i] = length;
                weight[i][start] = length;
                path_length -= weight[start][i];
                m--;
            }
        }
    }
}