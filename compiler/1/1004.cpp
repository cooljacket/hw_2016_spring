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

bool checkUseful(set<char>& useful, const MAPS& left_to_rights) {
	bool haveNew = false;
	for (MAPS::const_iterator it = left_to_rights.begin(); it != left_to_rights.end(); ++it) {
		if (useful.find(it->first) != useful.end())
			continue;

		const vector<string>& rights = it->second;
		for (int i = 0; i < rights.size(); ++i) {
			bool allOk = true;
			for (int j = 0; j < rights[i].size(); ++j)
				if (useful.find(rights[i][j]) == useful.end()) {
					allOk = false;
					break;
				}
			if (allOk) {
				useful.insert(it->first);
				haveNew = true;
				break;
			}
		}
	}

	return haveNew;
}

void eraseFromList(vector<string>& vs, const string& key) {
	for (int i = 0; i < vs.size(); ++i)
		if (vs[i] == key) {
			vs.erase(vs.begin() + i);
			break;
		}
}


int main() {
	char Start, left, ch;
	string production;
	int count;
	MAPS left_to_rights;
	vector<pair<char, string> > NT_to_productions;

	cin >> Start;
	cin >> count;
	vector<NT> NTs(count);
	map<char, int> NT_to_idx;
	set<char> useful, noUseAndDel;
	for (int i = 0; i < count; ++i) {
		cin >> NTs[i].data;
		NT_to_idx[NTs[i].data] = i;
	}

	cin >> count;
	for (int i = 0; i < count; ++i) {
		cin >> ch;
		useful.insert(ch);
	}

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

		NT_to_productions.push_back(make_pair(left, production));
	}

	// 由于删除掉一些非终结符、相关的产生式之后，可能会导致其它符号情况的变化
	// 所以应该将其放在一个while循环里
	while (true) {
		// 初始化可达情况
		for (int i = 0; i < NTs.size(); ++i)
			NTs[i].reachable = false;
		NTs[NT_to_idx[Start]].reachable = true;

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

		// 找出所有不可达非终结符
		vector<char> unReachableNTs;
		for (int i = 0; i < NTs.size(); ++i)
			if (!NTs[i].reachable && noUseAndDel.find(NTs[i].data) == noUseAndDel.end())
				unReachableNTs.push_back(NTs[i].data);

		if (unReachableNTs.empty())
			break;

		for (int i = 0; i < unReachableNTs.size(); ++i) {
			cout << "unreached Vn:" << unReachableNTs[i] << endl;
			cout << "delete VN:" << unReachableNTs[i] << endl;

			// 输出不可达产生式（由不可达非终结符直接决定）
			vector<string>& rights = left_to_rights[unReachableNTs[i]];
			for (int j = 0; j < rights.size(); ++j) {
				cout << "delete production:" << unReachableNTs[i] << "::=" << rights[j] << endl;

				// 同时也要从总的里面删掉
				for (int k = 0; k < NT_to_productions.size(); ++k) {
					if (NT_to_productions[k].first == unReachableNTs[i] && NT_to_productions[k].second == rights[j])
						NT_to_productions.erase(NT_to_productions.begin() + k);
				}
			}

			// 及时删除掉不可达非终结符的所有相关的产生式
			left_to_rights.erase(unReachableNTs[i]);
			noUseAndDel.insert(unReachableNTs[i]);
		}

		// 产生所有“有用”的符号
		while (checkUseful(useful, left_to_rights));

		for (int i = NTs.size()-1; i >= 0; --i) {
			if (!NTs[i].reachable)
				continue;

			char key = NTs[i].data;
			// 不在可用符号的集合里，即为无用的符号（即不可实例化的符号）
			if (useful.find(key) == useful.end() && noUseAndDel.find(key) == noUseAndDel.end()) {
				noUseAndDel.insert(key);
				cout << "unusefulNv:" << key << endl;
				cout << "delete VN:" << key << endl;
				// 输出不可达产生式（由不可达非终结符直接决定）
				// 在所有产生式里面找，从后往前，这样删除的话，也不会影响后续的迭代
				vector<string> delete_productions;
				for (int j = NT_to_productions.size()-1; j >= 0; --j) {
					char left = NT_to_productions[j].first;
					string production = NT_to_productions[j].second;
					string wholeProduction = string(1, left) + "::=" + production;

					if (left == key) {
						delete_productions.push_back(wholeProduction);
						NT_to_productions.erase(NT_to_productions.begin() + j);
						vector<string>& rights = left_to_rights[left];
						eraseFromList(rights, production);
						continue;
					}

					size_t pos = production.find(key);
					if (pos != string::npos) {
						delete_productions.push_back(wholeProduction);
						NT_to_productions.erase(NT_to_productions.begin() + j);
						vector<string>& rights = left_to_rights[left];
						eraseFromList(rights, production);
					}
				}

				for (int i = delete_productions.size()-1; i >= 0; --i)
					cout << "delete production:" << delete_productions[i] << endl;
			}
		}
	}


	// 最后按照输入给定的非终结符的顺序，输出可达、非自产生的产生式
	cout << "G[" << Start << "]:\n";
	for (int i = 0; i < NTs.size(); ++i) {
		if (!NTs[i].reachable)
			continue;

		vector<string>& rights = left_to_rights[NTs[i].data];
		if (rights.empty())
			continue;
		cout << NTs[i].data << "::=";
		for (int i = 0; i < rights.size(); ++i) {
			if (i > 0)
				cout << " | ";
			cout << rights[i];
		}
		cout << endl;
	}

	return 0;
}