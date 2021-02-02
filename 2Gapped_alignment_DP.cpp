#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define s(a,b) ((a)==(b) ? s : -f)
#define MAX(a,b,c) (a > (b > c ? b : c) ? a : (b > c ? b : c))
int d[10001][10001];
string x, y;
string gx, gy;
int s, p, f;
int ngx, ngy;
void reconstruct(int i, int j);
void main()
{
	string name;
	int m, n;
	ifstream infile("input.txt");
	infile >> name >> s >> f >> p;
	infile.close();
	infile.open(name, ios::in | ios::binary);
	infile.read((char*)&m, sizeof(int));
	infile.read((char*)&n, sizeof(int));
	for (int i = 0; i < m; i++) {
		char c;
		infile.read(&c, sizeof(char));
		x.push_back(c);
	}
	for (int i = 0; i < n; i++) {
		char c;
		infile.read(&c, sizeof(char));
		y.push_back(c);
	}
	infile.close();

	//when i == 0 and j == 0
	d[0][0] = 0;
	for (int j = 1; j <= n; j++)
		d[0][j] = -p * j;
	for (int i = 1; i <= m; i++)
		d[i][0] = -p * i;
	//other cases
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			d[i][j] = MAX(d[i - 1][j - 1] + s(x[i - 1], y[j - 1]), d[i][j - 1] - p, d[i - 1][j] - p);
		}
	}
	ngx = ngy = 0;
	reconstruct(m, n);

	ofstream outfile("output.txt");
	outfile << d[m][n] << "\n" << gx.length() << "\n";
	outfile << ngx << "\n";
	for (int i = 0; i < gx.length(); i++) {
		if (gx[i] == '-')
			outfile << i + 1 << "\n";
	}
	outfile << ngy << "\n";
	for (int i = 0; i < gy.length(); i++) {
		if (gy[i] == '-')
			outfile << i + 1 << "\n";
	}
	outfile.close();
	return;
}
void reconstruct(int i, int j) {
	if (i == 0 && j == 0)
		return;
	if (i > 0 && j > 0 && d[i][j] == d[i - 1][j - 1] + s(x[i - 1], y[j - 1])) {
		reconstruct(i - 1, j - 1);
		gx.push_back(x[i - 1]);
		gy.push_back(y[j - 1]);
	}
	else if (j > 0 && d[i][j] == d[i][j - 1] - p) {
		reconstruct(i, j - 1);
		gx.push_back('-');
		gy.push_back(y[j - 1]);
		ngx++;
	}
	else {
		reconstruct(i - 1, j);
		gx.push_back(x[i - 1]);
		gy.push_back('-');
		ngy++;
	}
}