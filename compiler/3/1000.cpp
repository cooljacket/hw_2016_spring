#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

typedef vector<vector<bool> > Matrix;
void Warshall(Matrix& R);
void display(const Matrix& R);

int main() {
	int size, tmp;
	cin >> size;
	Matrix R(size, vector<bool>(size));
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j) {
			cin >> tmp;
			R[i][j] = tmp == 1;
		}

	Warshall(R);
	display(R);

	return 0;
}


void Warshall(Matrix& R) {
	int N = R.size();
	Matrix R_clouse = Matrix(N, vector<bool>(N));

	for (int k = 0; k < N; ++k) {
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				R_clouse[i][j] = R[i][j] || (R[i][k] && R[k][j]);
		R = R_clouse;
	}
}

void display(const Matrix& R) {
	int N = R.size();
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j)
			cout << setw(3) << R[i][j];
		cout << endl;
	}
}