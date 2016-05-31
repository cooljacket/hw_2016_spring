#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;


/*// 非终结符
struct {
	int Nv;
	string VN[10];
} Vns = {3, "E", "T", "F"};

// 终结符
struct {
	int Nt;
	string VT[10];
} Vts = {7, "+", "-", "*", "/", "(", ")", "i"};

// 不知道是啥
struct {
	int Np;
	string PL[20], PR[20];
} ps = {0};*/

const string Start = "E";

void strip(string& str) {
	int left = 0, right = str.size() - 1;
	while (left <= right && str[left] == ' ')
		++left;
	while (right >= left && str[right] == ' ')
		--right;
	str = str.substr(left, right-left+1);
}

int main() {
	// get input
	string left, right;
	int count;
	map<string, int> left_to_idx;
	vector<string> lefts;
	vector<vector<string> > rights;

	cin >> count;
	for (int i = 0; i < count; ++i) {
		cin >> left;
		getline(cin, right);
		strip(left);
		strip(right);
		map<string, int>::iterator it = left_to_idx.find(left);
		if (it != left_to_idx.end())
			rights[it->second].push_back(right);
		else {
			rights.push_back(vector<string>(1, right));
			lefts.push_back(left);
			left_to_idx[left] = rights.size() - 1;
		}
	}

	cout << "G[" << Start << "]:\n";
	for (int i = 0; i < lefts.size(); ++i) {
		cout << lefts[i] << "::=";
		for (int j = 0; j < rights[i].size(); ++j) {
			if (j > 0)
				cout << " | ";
			cout << rights[i][j];
		}
		cout << endl;
	}

	return 0;
}