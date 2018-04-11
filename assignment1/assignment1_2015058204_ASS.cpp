#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
using namespace std;

struct Point
{
	int row;
	int col;
};

bool operator<(const Point p1, const Point p2)
{
	return p1.row < p2.row;
}

bool operator==(const Point p1, const Point p2)
{
	return ( p1.row == p2.row ) && ( p1.col == p2.col );
}

bool operator!=(const Point p1, const Point p2)
{
	return ( p1.row != p2.row ) || ( p1.col != p2.col );
}

vector<Point> ExitArr;
stack<pair<Point, Point> > st; // stack used for printing
priority_queue<pair<int, Point>, vector<pair<int, Point> >, greater<pair<int, Point> > > pq; // min-heap
Point start;
int MazeMap[501][501];
int visited[501][501];
int m, n;
int dx[] = { 0,1,0,-1 };
int dy[] = { 1,0,-1,0 };
int time, length;

// ������ �׹����� promise���� �Ǻ�. 1.border�� ������� 2. �湮�� ���ߴ��� 3. �����ִ� ������(Map�� 2�Ǵ� 4����)
bool CheckPromise(int row, int col)
{
	return ( 1 <= row && row <= m && 1 <= col && col <= n ) &&
		( !visited[row][col] ) && ( MazeMap[row][col] == 2 || MazeMap[row][col] == 4 );
}

int getHeuristic(int row, int col, vector<Point> ExitArr) // get minimum H(n) of multiple Exits 
{
	int ret = 0;
	for ( auto itr = ExitArr.begin(); itr != ExitArr.end(); itr++ )
	{
		int min_value = abs(itr->row - row) + abs(itr->col - col); // Manhatten Distance
		if ( ret == 0 || ret > min_value ) ret = min_value;
	}
	return ret;
}

int get_Astar_fn(int row, int col, vector<Point> ExitArr) // get minimum F(n) of multiple Exits in A* algorithm
{
	int ret = 0;
	for ( auto itr = ExitArr.begin(); itr != ExitArr.end(); itr++ )
	{
		int min_value = getHeuristic(row, col, ExitArr) + abs(start.row - row) + abs(start.col - col); // F(n) = H(n) + G(n)
		if ( ret == 0 || ret > min_value ) ret = min_value;
	}
	return ret;
}

bool Astar(int row, int col)
{
	int dist = get_Astar_fn(row, col, ExitArr);
	Point p = { row,col };
	pq.push({ dist, p });
	visited[row][col] = 1;
	while ( !pq.empty() )
	{
		Point p_here = pq.top().second;
		pq.pop();

		for ( int i = 0; i < 4; i++ )
		{
			int my = p_here.row + dy[i];
			int mx = p_here.col + dx[i];

			if ( CheckPromise(my, mx) )
			{
				Point p_there = { my,mx };
				int dist_there = get_Astar_fn(my, mx, ExitArr);
				
				//cout << "A*�� �̵���� : (" << p_here.row << "," << p_here.col << ")"
				//	<< " --> (" << p_there.row << "," << p_there.col << ") \n";
				pq.push({ dist_there,p_there });
				st.push({ p_here,p_there }); // Stack({�����,������})
				visited[my][mx] = 1;
				time++;
				if ( MazeMap[my][mx] == 4 )
				{
					cout << "�ⱸ �߰�!" << "\n";
					return true;
				}
			}
		}
	}
	return false;
}

//void printMap()
//{
//	for ( int i = 1; i <= m; i++ )
//	{
//		for ( int j = 1; j <= n; j++ )
//		{
//			printf("%d ", MazeMap[i][j]);
//		}
//		printf("\n");
//	}
//	printf("---\n");
//	printf("length=%d\n", length);
//	printf("time=%d\n", time);
//}

int main()
{
	char input_path[100];
	scanf("%s", input_path);

	char write_path[100];
	char read_path[100];
	strcpy(write_path, input_path);
	strcpy(read_path, input_path);
	strcat(read_path, "\\input.txt");
	strcat(write_path, "\\output.txt");

	FILE *fp = fopen(read_path, "r");
	if ( fp == NULL )
	{
		printf("No such file or directory !! \n");
		return 0;
	}
	
	fscanf(fp, "%d %d", &m, &n);
	for ( int i = 1; i <= m; i++ )
	{
		for ( int j = 1; j <= n; j++ )
		{
			fscanf(fp, "%d", &MazeMap[i][j]);
			if ( MazeMap[i][j] == 3 )
			{
				start.row = i;
				start.col = j;
			}
			else if ( MazeMap[i][j] == 4 )
			{
				ExitArr.push_back({ i,j });
			}
		}
	}

	Astar(start.row, start.col);

	// ������ �ⱸ�� ������ �ⱸ�� ����.
	Point pos;
	for ( int i = 0; i < ExitArr.size(); i++ )
	{
		int exitrow = ExitArr[i].row;
		int exitcol = ExitArr[i].col;
		if ( visited[exitrow][exitcol] == 1 )
			pos = { exitrow, exitcol };
	}

	// st(stack)�� second(������)�� pos�� ��ġ�ϴ��� check�ϸ鼭 �ִܰŸ� ��θ� 5�� �ٲ���.
	while ( !st.empty() && st.top().first != start )
	{
		if ( st.top().second == pos )
		{
			pos = st.top().first;
			st.pop();
			MazeMap[pos.row][pos.col] = 5;
			length++;
		}
		else
			st.pop();
	}

	// �Ʒ��� ��ºκ�
	FILE *w_fp = fopen(write_path, "w");
	for ( int i = 1; i <= m; i++ )
	{
		for ( int j = 1; j <= n; j++ )
		{
			fprintf(w_fp, "%d ", MazeMap[i][j]);
		}
		fprintf(w_fp, "\n");
	}
	fprintf(w_fp, "---\n");
	fprintf(w_fp, "length=%d\n", length);
	fprintf(w_fp, "time=%d\n", time);
	fclose(w_fp);

	return 0;
}
