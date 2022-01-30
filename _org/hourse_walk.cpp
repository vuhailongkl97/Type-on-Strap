#include <iostream>
#include <map>
#include <vector>
#include <unistd.h>
#include <iomanip>
using namespace std;


int arr[8][8] = {0};
map<int, int> step = {{1,2}, {-1, 2}, {-2,1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}, {2, 1}};
int X[8] = { -2,-2,-1,-1, 1, 1, 2, 2};
int Y[8] = { -1, 1,-2, 2,-2, 2,-1, 1};
void print_array(int a[8][8])
{
    for(int i = 0; i < 8 ; i++)
    {
        for (int j = 0; j < 8 ; j++)
        {
            cout << setw(4)<< a[i][j] << " ";
        }
        cout << "\n";
    }
}
int max_s = 0;
int dem = 0;
void walk( int x, int y)
{
    dem++;
    arr[x][y] = dem;

    if(dem > max_s)
        max_s = dem;

    if(dem >=64)
    {
        print_array(arr);
    }
    for(int i = 0; i < 8; i++)
    {
        int nx = x + X[i];
        int ny = y + Y[i];
        if(( nx >= 0 && ny >= 0 && nx < 8 && ny <8 && arr[nx][ny] == 0 ))
        {
            walk(nx, ny);
        }
    }
    arr[x][y] = 0;
    dem--;
}

int main()
{

    walk(0,0);
    return 0;
}
