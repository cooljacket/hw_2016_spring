/*
如何输出不可达非终结符、不可达产生式，我觉得也得做一个规定。
比如按给定的非终结符的顺序输出（我就是按这个顺序来做的）。
但是样例里只有一个，一个就无所谓顺序了。
*/
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <set>
using namespace std;

typedef map<char, vector<string> > MAPS;

struct NT
{
	char data;
	bool reachable;
};

void strip(string& str) {
	int left = 0, right = str.size() - 1;
	while (left <= right && str[left] == ' ')
		++left;
	while (right >= left && str[right] == ' ')
		--right;
	str = str.substr(left, right-left+1);
}

int main() {
	char Start, left, ch;
	string production;
	int count;
	MAPS left_to_rights;

	cin >> Start;
	cin >> count;
	vector<NT> NTs(count);
	map<char, int> NT_to_idx;

	for (int i = 0; i < count; ++i) {
		cin >> NTs[i].data;
		NTs[i].reachable = NTs[i].data == Start;
		NT_to_idx[NTs[i].data] = i;
	}

	cin >> count;
	for (int i = 0; i < count; ++i)
		cin >> ch;

	cin >> count;
	for (int i = 0; i < count; ++i) {
		cin >> left;
		getline(cin, production);
		strip(production);

		// 自产生式
		if (production.size() == 1 && left == production[0]) {
			cout << "delete self production:" << left << "::=" << production << endl;
			continue;
		}

		MAPS::iterator it = left_to_rights.find(left);
		if (it != left_to_rights.end())
			it->second.push_back(production);
		else
			left_to_rights[left] = vector<string>(1, production);
	}

	// 使用bfs判断各非终结符是否可达
	queue<char> q;
	q.push(Start);
	while (!q.empty()) {
		char top = q.front();
		q.pop();

		if (left_to_rights.find(top) == left_to_rights.end())
			continue;

		const vector<string>& rights = left_to_rights[top];
		// 对每个产生式，检查其中每个(非终结)符号
		for (int i = 0; i < rights.size(); ++i) {
			for (int j = 0; j < rights[i].size(); ++j) {
				char key = rights[i][j];
				map<char, int>::iterator it = NT_to_idx.find(key);
				if (it != NT_to_idx.end() && !NTs[it->second].reachable) {
					NTs[it->second].reachable = true;
					q.push(key);
				}
			}
		}
	}

	// 找出并输出不可达非终结符
	vector<char> unReachableNTs;
	for (int i = 0; i < NTs.size(); ++i)
		if (!NTs[i].reachable)
			unReachableNTs.push_back(NTs[i].data);
	for (int i = 0; i < unReachableNTs.size(); ++i)
		cout << "unreached Vn:" << unReachableNTs[i] << endl;

	// 输出不可达产生式（由不可达非终结符直接决定
	for (int i = 0; i < unReachableNTs.size(); ++i) {
		vector<string>& rights = left_to_rights[unReachableNTs[i]];
		for (int j = 0; j < rights.size(); ++j)
			cout << "delete production:" << unReachableNTs[i] << "::=" << rights[j] << endl;
	}

	// 输出删除掉的不可达非终结符
	for (int i = 0; i < unReachableNTs.size(); ++i)
		cout << "delete VN:" << unReachableNTs[i] << endl;


	// 最后按照输入给定的非终结符的顺序，输出可达、非自产生的产生式
	cout << "G[" << Start << "]:\n";
	for (int i = 0; i < NTs.size(); ++i) {
		if (!NTs[i].reachable)
			continue;

		cout << NTs[i].data << "::=";
		vector<string>& rights = left_to_rights[NTs[i].data];
		for (int i = 0; i < rights.size(); ++i) {
			if (i > 0)
				cout << " | ";
			cout << rights[i];
		}
		cout << endl;
	}

	return 0;
}