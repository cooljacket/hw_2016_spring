/*
我觉得输出的产生式，右边应该去重
比如样例输出有这么一行：
P::=G | G
所以数据结构应该改用map<string, set<string> >...
*/
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

typedef map<string, vector<string> > MAPS;

void strip(string& str) {
	int left = 0, right = str.size() - 1;
	while (left <= right && str[left] == ' ')
		++left;
	while (right >= left && str[right] == ' ')
		--right;
	str = str.substr(left, right-left+1);
}

int main() {
	string Start, left, right, VT;
	int count;
	MAPS left_to_rights;

	cin >> Start;
	cin >> count;
	vector<string> NTs(count);
	for (int i = 0; i < count; ++i)
		cin >> NTs[i];
	cin >> count;
	for (int i = 0; i < count; ++i)
		cin >> VT;

	cin >> count;
	for (int i = 0; i < count; ++i) {
		cin >> left;
		getline(cin, right);
		strip(left);
		strip(right);

		// 自产生式
		if (left == right) {
			cout << "delete self production:" << left << "::=" << right << endl;
			continue;
		}

		MAPS::iterator it = left_to_rights.find(left);
		if (it != left_to_rights.end())
			it->second.push_back(right);
		else
			left_to_rights[left] = vector<string>(1, right);
	}

	cout << "G[" << Start << "]:\n";
	for (int i = 0; i < NTs.size(); ++i) {
		cout << NTs[i] << "::=";
		vector<string>& rights = left_to_rights[NTs[i]];
		for (int i = 0; i < rights.size(); ++i) {
			if (i > 0)
				cout << " | ";
			cout << rights[i];
		}
		cout << endl;
	}

	return 0;
}