// Problem#: 17901
// Submission#: 4723834
// The source code is licensed under Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
// URI: http://creativecommons.org/licenses/by-nc-sa/3.0/
// All Copyright reserved by Informatic Lab of Sun Yat-sen University
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <iomanip>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

const char Empty = 'k', Stop = '#';
char Start;
vector<string> NTs;
vector<char> VTs;
set<string> canReachEmpty;
map<string, vector<string> > prods;
vector<pair<string, string> > prods_inorder;
map<string, set<char> > FIRST, FOLLOW;
map<string, int> NTs_to_idx;
map<char, int> VTs_to_idx;
string production;

void Input();
template<typename T>
typename vector<T>::const_iterator find(const vector<T>& data, T key);
bool insertExcept(const set<char>& from, set<char>& to, char out);
bool calFirst();
bool calFollow();
template<typename T>
void printStack(stack<T> s);
void parse(const vector<vector<string> >& matrix, const string& production);


int main() {
    Input();

    while (calFirst());
    while (calFollow());

    VTs.push_back(Stop);
    VTs_to_idx[Stop] = VTs.size() - 1;

    vector<vector<string> > matrix(NTs.size(), vector<string>(VTs.size()));
    for (int i = 0; i < prods_inorder.size(); ++i) {
        const string& left = prods_inorder[i].first, right = prods_inorder[i].second;
        const set<char>& first = FIRST[string(1, right[0])];
        int row = NTs_to_idx[left];

        for (int j = 0; j < VTs.size(); ++j) {
            if (first.find(VTs[j]) != first.end())
                matrix[row][j] = right;
        }

        if (first.find(Empty) != first.end()) {
            const set<char>& fo = FOLLOW[left];
            for (set<char>::const_iterator it = fo.begin(); it != fo.end(); ++it) {
                matrix[row][VTs_to_idx[*it]] = right;
            }
        }
    }

    try {
        parse(matrix, production);      
    }
    catch (logic_error& ex) {
        cout << ex.what() << endl;
        exit(-1);
    }

    return 0;
}


void Input() {
    int n, id;
    string left, right;
    cin >> Start >> n;
    NTs = vector<string>(n);
    for (int i = 0; i < n; ++i) {
        cin >> NTs[i];
        NTs_to_idx[NTs[i]] = i;
    }
        
    cin >> n;
    VTs = vector<char>(n);
    for (int i = 0; i < n; ++i) {
        cin >> VTs[i];
        FIRST[string(1, VTs[i])].insert(VTs[i]);
        VTs_to_idx[VTs[i]] = i;
    }
    FIRST[string(1, Empty)].insert(Empty);

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> id >> left >> right;
        prods_inorder.push_back(make_pair(left, right));
        prods[left].push_back(right);
        if (right.size() == 1 && right[0] == Empty)
            canReachEmpty.insert(left);
    }
    // the remain part is no use...
    cin >> n;
    string line;
    while (getline(cin, line)) {
        if (!line.empty())
            production = line;
    }
}


template<typename T>
typename vector<T>::const_iterator find(const vector<T>& data, T key) {
    for (int i = 0; i < data.size(); ++i)
        if (data[i] == key)
            return data.begin() + i;
    return data.end();
}


// if change then return true
bool insertExcept(const set<char>& from, set<char>& to, char out) {
    int size = to.size();
    set<char>::const_iterator fit = from.begin();
    while (fit != from.end()) {
        if (*fit != out)
            to.insert(*fit);
        ++fit;
    }

    return size != to.size();
}

// if change then return true
bool calFirst() {
    bool change = false;
    for (int i = 0; i < NTs.size(); ++i) {
        string left = NTs[i];
        set<char>& first = FIRST[left];
        size_t size = first.size();
        vector<string>& rights = prods[left];

        for (int i = 0; i < rights.size(); ++i) {
            if (find(VTs, rights[i][0]) != VTs.end())
                first.insert(rights[i][0]);
            else {
                int pos = 0;
                while (pos < rights[i].size()) {
                    if (canReachEmpty.find(string(1, rights[i][pos])) == canReachEmpty.end())
                        break;
                    ++pos;
                }

                if (pos == rights[i].size())
                    continue;

                string tmp(1, rights[i][pos]);
                set<char>& another_first = FIRST[tmp];
                insertExcept(another_first, first, Empty);
            }
        }
        if (find(rights, string(1, Empty)) != rights.end())
            first.insert(Empty);

        change |= (size != first.size());
    }

    return change;
}

bool calFollow() {
    bool change = false;
    map<string, vector<string> >::iterator pit = prods.begin();
    while (pit != prods.end()) {
        string left = pit->first;
        if (left.size() == 1 && left[0] == Start) {
            set<char>& fo = FOLLOW[left];
            change |= fo.insert(Stop).second;
            // cout << "insert E " << change << endl;
        }

        const vector<string>& rights = pit->second;
        int len = rights.size();
        for (int i = 0; i < len; ++i) {
            const string& right = rights[i];
            int tail = right.size() - 1;
            while (tail >= 0) {
                if (find(NTs, string(1, right[tail])) != NTs.end()) {
                    change |= insertExcept(FOLLOW[left], FOLLOW[string(1, right[tail])], 0);
                }
                if (canReachEmpty.find(string(1, right[tail])) == canReachEmpty.end())
                    break;
                --tail;
            }
            for (int j = 0; j < right.size()-1; ++j) {
                if (find(NTs, string(1, right[j])) == NTs.end())
                    continue;
                change |= insertExcept(FIRST[string(1, right[j+1])], FOLLOW[string(1, right[j])], Empty);
            }
        }
        ++pit;
    }

    return change;
}

template<typename T>
void printStack(stack<T> s) {
    while (!s.empty()) {
        cout << s.top();
        s.pop();
    }
}

void parse(const vector<vector<string> >& matrix, const string& production) {
    stack<char> s;
    s.push(Stop);
    s.push(Start);
    int pos = 0;
    char matching = production[pos];
    char err_msg[100];

    while (!s.empty()) {
        char top = s.top();
        if (top == Empty) {
            s.pop();
            continue;
        }
        cout << '#' << production.substr(0, pos) << " & ";
        printStack(s);
        cout << endl;

        // TOP(s) is VT
        if (VTs_to_idx.find(top) != VTs_to_idx.end()) {
            if (top != matching) {
                sprintf(err_msg, "Error matching VT, top=%c, production[%d]=%c", top, pos, matching);
                throw logic_error(err_msg);
            }
            if (top == Stop)
                return ;
            s.pop();
            matching = production[++pos];
        } else {
            string right = matrix[NTs_to_idx[string(1, top)]][VTs_to_idx[matching]];
            if (right.empty()) {
                sprintf(err_msg, "Error matching NT, top=%c, production[%d]=%c", top, pos, matching);
                throw logic_error(err_msg);
            }

            s.pop();
            for (int i = right.size()-1; i >= 0; --i)
                s.push(right[i]);
        }
    }
}                                 