/*
问题来源：http://soj.sysu.edu.cn/show_problem.php?pid=1005&cid=2207
输入字母表，NDFA状态集，映射集；用造表法算法。
输出DFA状态集，映射集；

还未解决：空移环路的寻找和消除、消除空移、简化DFA
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;

typedef vector<int> STATE;
typedef pair<int, char> KEY;
typedef map<KEY, STATE> MAPS;
const char Empty = 'k';

struct T
{
	int from, to;
	char x;
};

bool checkInVector(const vector<int>& data, const vector<int>& toCheck) {
	for (int i = 0; i < toCheck.size(); ++i)
		for (int j = 0; j < data.size(); ++j)
			if (toCheck[i] == data[j])
				return true;
	return false;
}

void setToVector(const set<int>& s, vector<int>& v) {
	v = vector<int>(s.size());
	int i = 0;
	for (set<int>::const_iterator it = s.begin(); it != s.end(); ++it, ++i)
		v[i] = *it;
}

void display(const vector<int>& v) {
	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << ' ';
	cout << endl;
}

int main() {
	int n, x, y;
	char ch;
	bool start, end;
	vector<int> Q0, F;
	MAPS NDFA;

	cin >> n;
	vector<char> Sigmas(n);
	for (int i = 0; i < n; ++i)
		cin >> Sigmas[i];

	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> x >> start >> end;
		if (start)
			Q0.push_back(x);
		if (end)
			F.push_back(x);
	}

	// 一切状态都排序
	sort(Q0.begin(), Q0.end());
	sort(F.begin(), F.end());

	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> x >> y >> ch;
		NDFA[make_pair(x, ch)].push_back(y);
	}

	vector<STATE> states;
	vector<T> transaction_inOrder;
	map<STATE, int> state_to_idx;
	queue<int> q;
	q.push(0);
	state_to_idx[Q0] = 0;
	states.push_back(Q0);

	while (!q.empty()) {
		int from_idx = q.front();
		STATE now = states[from_idx];
		q.pop();

		for (int i = 0; i < Sigmas.size(); ++i) {
			set<int> to_set;
			for (int j = 0; j < now.size(); ++j) {
				KEY key = make_pair(now[j], Sigmas[i]);
				MAPS::iterator it = NDFA.find(key); 
				if (it != NDFA.end()) {
					to_set.insert(it->second.begin(), it->second.end());
					vector<int> tmp(it->second.begin(), it->second.end());
				}
			}

			if (!to_set.empty()) {
				int to_idx;
				vector<int> to;
				setToVector(to_set, to);
				sort(to.begin(), to.end());	// remember to sort
				map<STATE, int>::iterator it = state_to_idx.find(to);
				if (it == state_to_idx.end()) {
					states.push_back(to);
					to_idx = states.size() - 1;
					q.push(to_idx);
					state_to_idx[to] = to_idx;
				} else
					to_idx = it->second;

				transaction_inOrder.push_back((T) {from_idx, to_idx, Sigmas[i]});
			}
		}
	}


	cout << "Determine State:" << endl;
	for (int i = 0; i < states.size(); ++i) {
		cout << i << ' ';
		cout << (checkInVector(Q0, states[i]) ? 1 : 0) << ' ';
		cout << (checkInVector(F, states[i]) ? 1 : 0) << " [";
		for (int j = 0; j < states[i].size(); ++j)
			cout << states[i][j];
		cout << ']' << endl;
	}

	cout << "Determine Mapping:" << endl;
	for (int i = 0; i < transaction_inOrder.size(); ++i) {
		const T& t = transaction_inOrder[i];
		cout << t.from << ' ' << t.to << ' ' << t.x << " (" << i << endl;
	}

	return 0;
}