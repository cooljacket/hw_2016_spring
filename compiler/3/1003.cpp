#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
using namespace std;

enum Relation
{
	Equal, Little, Large, Error
};

typedef vector<vector<bool> > Matrix;
typedef vector<vector<Relation> > Table;
void Warshall(Matrix& R);
void Transpose(Matrix& R);
void To_Star(Matrix& R);
Matrix mul(const Matrix& A, const Matrix& B);
Table genRelation(const Matrix& E, const Matrix& L, const Matrix& G);
void outputStack(const vector<char>& stack);
void parse(const Table& table, const string& production);


char Start, ch, left_ch;
int n, id;
vector<char> all;
map<char, size_t> char2idx;
string p_right;
map<string, char> r_productions;


int main() {
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
		cin >> id >> left_ch >> p_right;
		r_productions[p_right] = left_ch;
		FIRST[char2idx[left_ch]][char2idx[p_right[0]]] = true;
		LAST[char2idx[left_ch]][char2idx[*(p_right.rbegin())]] = true;
		for (int j = 0; j < p_right.size()-1; ++j)
			EQUAL[char2idx[p_right[j]]][char2idx[p_right[j+1]]] = true;
	}

	cin >> n;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			cin >> id;
	string production;
	while (production.empty())
		getline(cin,production);

	Warshall(FIRST);
	Matrix LEQUAL = mul(EQUAL, FIRST);

	Warshall(LAST);
	Transpose(LAST);
	To_Star(FIRST);
	Matrix GEQUAL = mul(LAST, EQUAL);
	GEQUAL = mul(GEQUAL, FIRST);

	Table table = genRelation(EQUAL, LEQUAL, GEQUAL);
	parse(table, production);

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

Table genRelation(const Matrix& E, const Matrix& L, const Matrix& G) {
	int N = E.size();
	Table table(N, vector<Relation>(N, Error));
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (E[i][j])
				table[i][j] = Equal;
			else if (L[i][j])
				table[i][j] = Little;
			else if (G[i][j])
				table[i][j] = Large;
		}
	}

	return table;
}

void outputStack(const vector<char>& stack) {
	for (int i = 0; i < stack.size(); ++i)
		cout << stack[i];
}

void parse(const Table& table, const string& production) {
	vector<char> stack;
	int idx = 0;

	while (true) {
		cout << '#';
		outputStack(stack);
		cout << " & " << production.substr(idx) << endl;
		if (stack.empty()) {
			stack.push_back(production[idx++]);
			continue;
		}
		if (idx >= production.size()-1 && stack.size() == 1 && stack[0] == Start)
			break;

		const size_t i = char2idx[*(stack.rbegin())], j = (idx < production.size()-1) ? char2idx[production[idx]] : 0;
		if (idx >= production.size()-1 || table[i][j] == Large) {
			int key = stack.size() - 1;
			stack.pop_back();
			string right(1, stack[key]);
			while (key >= 1 && table[char2idx[stack[key-1]]][char2idx[stack[key]]] == Equal) {
				right.push_back(stack[key-1]);
				--key;
				stack.pop_back();
			}
			right = string(right.rbegin(), right.rend());
			stack.push_back(r_productions[right]);
		} else {
			stack.push_back(production[idx++]);
		}
	}
}