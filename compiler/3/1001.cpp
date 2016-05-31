#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

typedef vector<vector<bool> > Matrix;
void Warshall(Matrix& R);
void display(const Matrix& R, const char* tips, const vector<char>& header);

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
	for (int i = 0; i < n; ++i) {
		cin >> id >> left >> right;
		FIRST[char2idx[left]][char2idx[right[0]]] = true;
		LAST[char2idx[left]][char2idx[right[right.size()-1]]] = true;
	}

	display(FIRST, "FIRST=", all);
	display(LAST, "LAST=", all);
	Warshall(FIRST);
	Warshall(LAST);
	display(FIRST, "FIRST+=", all);
	display(LAST, "LAST+=", all);
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