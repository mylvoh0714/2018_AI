#include <iostream>
#include <string>
#include <vector>
#include <stack>
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

const int depth_interval = 1;
vector<Point> ExitArr;
stack<pair<Point, Point>> st; // stack used for printing
Point start;
int MazeMap[501][501];
int visited[501][501];
int m, n;
int dx[] = { 0,1,0,-1 };
int dy[] = { 1,0,-1,0 };
int time, length;

// 인접한 네방향이 promise한지 판별. 1.border를 벗어났는지 2. 방문을 안했는지 3. 갈수있는 길인지(Map이 2또는 4인지)
bool CheckPromise(int row, int col)
{
	return ( 1 <= row && row <= m && 1 <= col && col <= n ) &&
		( !visited[row][col] ) && ( MazeMap[row][col] == 2 || MazeMap[row][col] == 4 );
}

bool dfs(int row, int col,int index)
{
	visited[row][col] = 1;
	Point p = { row,col };

	if ( index <= 0 )
	{
		return false;
	}

	for ( int i = 0; i < 4; i++ )
	{
		int my = row + dy[i];
		int mx = col + dx[i];
		
		// Check Promise of four-direction 
		if ( CheckPromise(my, mx) )
		{
			Point p_there = { my,mx };
			//cout << "IDS의 이동경로 : (" << p.row << "," << p.col << ")"
			//	<< " --> (" << p_there.row << "," << p_there.col << ") \n";
			st.push({ p,p_there });
			visited[my][mx] = 1;
			time++;
			if ( MazeMap[my][mx] == 4 )
			{
				//cout << "출구 발견!" << "\n";
				return true;
			}
			if ( dfs(my, mx, index - 1) == true )
				return true;
		}
	}
	return false;
}

bool IDDFS(int row, int col, int max_depth) // depth를 점차 늘리면서 dfs를 실행.
{
	for ( int i = 0	; i <= max_depth; i += depth_interval )
	{
		//cout << "=====================================================" << endl;
		//cout << "현재 max_depth : " << i << endl;
		if ( dfs(row, col, i) == true )
			return true;
		memset(visited, 0, sizeof(visited));
		while ( !st.empty() ) st.pop();
	}
	return false;
}

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
			if ( MazeMap[i][j] == 3 ) // set start point
			{
				start.row = i;
				start.col = j;
			}
			else if ( MazeMap[i][j] == 4 ) // set exit point
			{
				ExitArr.push_back({ i,j });
			}
		}
	}

	IDDFS(start.row, start.col, 1000);

	// 여러개 출구중 도착한 출구를 선택.
	Point pos;
	for ( int i = 0; i < ExitArr.size(); i++ )
	{
		int exitrow = ExitArr[i].row;
		int exitcol = ExitArr[i].col;
		if ( visited[exitrow][exitcol] == 1 )
			pos = { exitrow, exitcol };
	}

	// st의 second(도착점)이 pos와 일치하는지 check하면서 최단거리 경로를 5로 바꿔줌.
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

	// 아래는 출력부분
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
