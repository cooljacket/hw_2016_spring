/*
Problem: http://soj.sysu.edu.cn/show_problem.php?pid=1000&cid=2207
Content: 简单模拟词法分析
Algorithm:
	1）重点在于split函数的逻辑，画一个有限自动机就可以很清晰地表示出来～注意状态转移即可
	2）使用map可以简化和加快查找的过程
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

typedef map<string, int> MAP;
typedef pair<string, int> PAIR;
typedef map<string, int>::iterator Iterator;

vector<PAIR> program;
map<string, int> global;
vector<MAP> index;
vector<int> cnt;

string kw[] = {"int", "main", "return"};
string op[] = {"+", "*", "=", "+=", "*="};
string bd[] = {"(", ")", "{", "}", ",", ";"};
enum TYPES
{
	KW, OP, BD, ID, UINT
};
enum MODE
{
	SHARP_END, IN_THE_AIR, ALPHA, DIGIT, SYMBOL
};

inline int getIndex(const string& s, int order) {
	return index[order][s];
}
inline bool isalpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
inline bool isdigit(char c) {
	return c >= '0' && c <= '9';
}
inline int iskw(const string& s) {
	for (int i = 0; i < 3; ++i)
		if (s == kw[i])
			return true;
	return false;
}
inline bool isBD(const string& s) {
	return index[BD].find(s) != index[BD].end();
}
inline bool isKW(const string& s) {
	return index[KW].find(s) != index[KW].end();
}
inline bool isOP(const string& s) {
	return index[OP].find(s) != index[OP].end();
}

void initHelper_global(const string* words, int size, int order);
void initHelper_local(const string* words, int size, int order);
void init();
void split();
void print_a_vector(const vector<string>& vs, const string& tips);
void result_out();
bool isuint(string& s);
bool isIdentifier(string& s);

int main() {
	init();
	split();
	result_out();

	return 0;
}


void initHelper_global(const string* words, int size, int order) {
	for (int i = 0; i < size; ++i)
		global[words[i]] = order;
}

void initHelper_local(const string* words, int size, int order) {
	for (int i = 0; i < size; ++i)
		index[order-1][words[i]] = i;
}

void init() {
	initHelper_global(kw, 3, 1);
	initHelper_global(op, 5, 2);
	initHelper_global(bd, 6, 3);

	index = vector<MAP>(5);
	initHelper_local(kw, 3, 1);
	initHelper_local(op, 5, 2);
	initHelper_local(bd, 6, 3);
}

void print_a_vector(const vector<string>& vs, const string& tips) {
	printf("%s", tips.data());
	int len = vs.size();

	for (int i = 0; i < len-1; ++i)
		printf("%s ", vs[i].data());
	if (len > 0)
		printf("%s\n", vs[len-1].data());
}

void result_out() {
	vector<string> identifiers, uints;
	set<string> ids, uis;

	for (int i = 0; i < program.size(); ++i) {
		string word = program[i].first;
		int type = program[i].second;

		printf("<%d,%d>", type+1, getIndex(word, type));
		if (type == int(ID) && ids.find(word) == ids.end()) {
			identifiers.push_back(word);
			ids.insert(word);
		}
		else if (type == int(UINT) && uis.find(word) == uis.end()) {
			uints.push_back(word);
			uis.insert(word);
		}
	}
	printf("\n");
	
	print_a_vector(identifiers, "identifieres:");
	print_a_vector(uints, "Unsigned_integer:");
}

bool isuint(string& s) {
	for (int i = 0; i < s.size(); ++i)
		if (!isdigit(s[i]))
			return false;

	return !s.empty();
}

bool isIdentifier(string& s) {
	for (int i = 0; i < s.size(); ++i)
		if (!isdigit(s[i]) && !isalpha(s[i]))
			return false;

	return !s.empty();
}

void split() {
	char c;
	string buffer(200, ' ');
	int len = 0, last_mode = IN_THE_AIR, current_mode = IN_THE_AIR;

	while (scanf("%c", &c) == 1) {
		string now(buffer.substr(0, len));
		bool toZero = true, saveChar = true;
		int type = -1;

		if (c == ' ' || c == '\n' || c == '\t') {
			current_mode = IN_THE_AIR;
			saveChar = false;
		}
		else if (c == '#')
			current_mode = SHARP_END;
		else if (isdigit(c))
			current_mode = DIGIT;
		else if (isalpha(c))
			current_mode = ALPHA;
		else 
			current_mode = SYMBOL;

		if (isBD(now))
			type = BD;
		else if ((last_mode == SYMBOL || last_mode == IN_THE_AIR) && current_mode != SYMBOL && isOP(now))
			type = OP;
		else if (current_mode != ALPHA && isKW(now))
			type = KW;
		else if ((last_mode == DIGIT || last_mode == IN_THE_AIR) && current_mode != DIGIT && isuint(now)) {
			type = UINT;
			if (index[int(UINT)].find(now) == index[int(UINT)].end())
				index[int(UINT)][now] = index[int(UINT)].size() - 1;
		}
		else if (current_mode != DIGIT && current_mode != ALPHA && isIdentifier(now)) {
			type = ID;
			if (index[int(ID)].find(now) == index[int(ID)].end())
				index[int(ID)][now] = index[int(ID)].size() - 1;
		}
		else
			toZero = false;
			
	/*printf("last_mode=%d, current_mode=%d, toZero=%d, type=%d, now=%s\n", 
		last_mode, current_mode, int(toZero), type, now.data());*/
		
		if (toZero && type != -1) {
			len = 0;
			current_mode = IN_THE_AIR;
			program.push_back(make_pair(now, type));
		}

		last_mode = current_mode;
		if (saveChar)
			buffer[len++] = c;

		if (c == '#')
			break;
	}
}