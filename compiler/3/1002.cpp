#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

typedef vector<vector<bool> > Matrix;
void Warshall(Matrix& R);
void Transpose(Matrix& R);
void To_Star(Matrix& R);
Matrix mul(const Matrix& A, const Matrix& B);
void display(const Matrix& R, const char* tips, const vector<char>& header);
void showRelation(const Matrix& E, const Matrix& L, const Matrix& G, const char* tips, const vector<char>& header);

int main() {
	char Start, ch, left;
	int n, id;
	vector<char> all;
	map<char, size_t> char2idx;
	string right;

	cin >> Start >> n;
	for (int i = 0; i < n; ++i) {
		cin >> ch;
		all.push_back(ch);
		char2idx[ch] = all.size() - 1;
	}
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> ch;
		all.push_back(ch);
		char2idx[ch] = all.size() - 1;
	}

	cin >> n;
	int N = all.size();
	Matrix FIRST(N, vector<bool>(N)), LAST(N, vector<bool>(N));
	Matrix EQUAL(N, vector<bool>(N));
	for (int i = 0; i < n; ++i) {
		cin >> id >> left >> right;
		FIRST[char2idx[left]][char2idx[right[0]]] = true;
		LAST[char2idx[left]][char2idx[right[right.size()-1]]] = true;
		for (int j = 0; j < right.size()-1; ++j)
			EQUAL[char2idx[right[j]]][char2idx[right[j+1]]] = true;
	}

	Warshall(FIRST);
	Matrix LEQUAL = mul(EQUAL, FIRST);

	Warshall(LAST);
	Transpose(LAST);
	To_Star(FIRST);
	Matrix GEQUAL = mul(LAST, EQUAL);
	GEQUAL = mul(GEQUAL, FIRST);

	display(EQUAL, "EQUAL=", all);
	display(LEQUAL, "LEQUAL=", all);
	display(GEQUAL, "GEQUAL=", all);
	showRelation(EQUAL, LEQUAL, GEQUAL, "M[R]=", all);

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

void Transpose(Matrix& R) {
	for (int i = 0; i < R.size(); ++i)
		for (int j = 0; j < i; ++j) {
			bool tmp = R[i][j];
			R[i][j] = R[j][i];
			R[j][i] = tmp;
		}
}

void To_Star(Matrix& R) {
	for (int i = 0; i < R.size(); ++i)
		R[i][i] = true;
}

Matrix mul(const Matrix& A, const Matrix& B) {
	int row = A.size(), col = B[0].size();
	Matrix res(row, vector<bool>(col));
	for (int i = 0; i < row; ++i)
		for (int j = 0; j < col; ++j) {
			int tmp = 0;
			for (int k = 0; k < A[i].size(); ++k)
				if (A[i][k] && B[k][j])
					++tmp;
			res[i][j] = (tmp % 2) != 0;
		}

	return res;
}

void display(const Matrix& R, const char* tips, const vector<char>& header) {
	cout << tips << endl << setw(3) << ' ';
	for (int i = 0; i < header.size(); ++i)
		cout << setw(3) << header[i];
	cout << endl;

	int N = R.size();
	for (int i = 0; i < N; ++i) {
		cout << setw(3) << header[i];
		for (int j = 0; j < N; ++j)
			cout << setw(3) << R[i][j];
		cout << endl;
	}
}

void showRelation(const Matrix& E, const Matrix& L, const Matrix& G, const char* tips, const vector<char>& header) {
	cout << tips << endl << setw(3) << ' ';
	for (int i = 0; i < header.size(); ++i)
		cout << setw(3) << header[i];
	cout << endl;

	int N = E.size();
	for (int i = 0; i < N; ++i) {
		cout << setw(3) << header[i];
		for (int j = 0; j < N; ++j) {
			cout << setw(3);
			if (E[i][j])
				cout << '=';
			else if (L[i][j])
				cout << '<';
			else if (G[i][j])
				cout << '>';
			else
				cout << ' ';
		}
		cout << endl;
	}
}