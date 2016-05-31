#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
using namespace std;

enum STATE
{
	Forward, Error, Receive, Reduction
};

void outputStack(const vector<char>& stack);
void parse(const string& production, const vector<pair<char, string> >& productions, const vector<vector<pair<STATE, int> > >& Action, 
	const vector<vector<int> >& GOTO, map<char, size_t>& char2idx);


int main() {
	char Start, ch, left;
	int n, id, cnt = 0;
	vector<char> all;
	vector<pair<char, string> > productions;
	map<char, size_t> char2idx;
	string right;

	cin >> Start >> n;
	for (int i = 0; i < n; ++i) {
		cin >> ch;
		all.push_back(ch);
		char2idx[ch] = cnt++;
	}
	cin >> n;
	cnt = 0;
	for (int i = 0; i < n; ++i) {
		cin >> ch;
		all.push_back(ch);
		char2idx[ch] = cnt++;
	}

	cin >> n;
	int N = all.size();
	for (int i = 0; i < n; ++i) {
		cin >> id >> left >> right;
		productions.push_back(make_pair(left, right));
	}

	int s_num, a_col, g_col;
	cin >> s_num >> a_col >> g_col;
	vector<vector<pair<STATE, int> > > Action(s_num, vector<pair<STATE, int> >(a_col));
	vector<vector<int> > GOTO(s_num, vector<int>(g_col));
	for (int i = 0; i < s_num; ++i) {
		cin >> id;
		for (int j = 0; j < a_col; ++j) {
			cin >> ch >> Action[i][j].second;
			switch (ch) {
				case 'k':
					Action[i][j].first = Error;
				break;
				case 's':
					Action[i][j].first = Forward;
				break;
				case 'r':
					Action[i][j].first = Reduction;
				break;
				case 'A':
					Action[i][j].first = Receive;
				break;
				default: break;
			}
		}
		for (int j = 0; j < g_col; ++j)
			cin >> GOTO[i][j];
	}

	string production;
	while (production.empty())
		getline(cin, production);

	parse(production, productions, Action, GOTO, char2idx);

	return 0;
}

void outputStack(const vector<char>& stack) {
	for (int i = 0; i < stack.size(); ++i) {
		if (i % 2 == 0)
			cout << (char)stack[i];
		else
			cout << (int)(stack[i] - '0');
	}
}

void parse(const string& production, const vector<pair<char, string> >& productions, const vector<vector<pair<STATE, int> > >& Action, 
	const vector<vector<int> >& GOTO, map<char, size_t>& char2idx) {
	vector<char> stack;
	stack.push_back('#');
	stack.push_back('0');
	int idx = 0;

	while (true) {
		int top = stack.size() - 1;
		pair<STATE, int> now = Action[stack[top]-'0'][char2idx[production[idx]]];
		outputStack(stack);
		cout << " & " << production.substr(idx) << endl;

		switch (now.first) {
			case Error: {
				throw logic_error("Error when parsing...");
				return ;
			} break;
			case Forward: {
				stack.push_back(production[idx++]);
				stack.push_back(now.second + '0');
			} break;
			case Reduction: {
				pair<char, string> p =  productions[now.second];
				int size = p.second.size();
				for (int i = 0; i < size; ++i)
					stack.pop_back(), stack.pop_back();
				top = stack.size() - 1;
				int nextState = GOTO[stack[top]-'0'][char2idx[p.first]];
				stack.push_back(p.first);
				stack.push_back(nextState + '0');
			} break;
			case Receive: {
				return ;
			} break;
			default: break;
		}
	}
}