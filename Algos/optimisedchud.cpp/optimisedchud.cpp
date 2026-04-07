#include <bits/stdc++.h>
using namespace std;

/* ---------- STRUCTURES ---------- */

struct UEntry {
    int tid;
    long long util;
};

struct Item {
    string name;
    vector<UEntry> ul;   // utility-list
};

/* ---------- GLOBALS ---------- */

long long MIN_UTIL;
vector<Item> items;                 // level-1 items (sorted)
map<string, long long> CHUIs;       // closed HUIs with utility

/* ---------- UTILITY FUNCTIONS ---------- */

long long utility(const vector<UEntry>& ul) {
    long long sum = 0;
    for (auto &e : ul) sum += e.util;
    return sum;

}

vector<UEntry> intersectUL(const vector<UEntry>& A,
                           const vector<UEntry>& B) {
    vector<UEntry> res;
    int i = 0, j = 0;

    while (i < A.size() && j < B.size()) {
        if (A[i].tid == B[j].tid) {
            res.push_back({A[i].tid, A[i].util + B[j].util});
            i++; j++;
        } else if (A[i].tid < B[j].tid) {
            i++;
        } else {
            j++;
        }
    }
    return res;
}

string makeKey(const vector<string>& items) {
    string k;
    for (auto &x : items) k += x + " ";
    return k;
}

/* ---------- CHUD DFS ---------- */

void DFS(const vector<string>& prefix,
         const vector<UEntry>& prefixUL,
         int startIdx) {

      bool closed = true;

    for (int i = startIdx; i < items.size(); i++) {

        vector<UEntry> newUL = intersectUL(prefixUL, items[i].ul);
        long long u = utility(newUL);

        if (u < MIN_UTIL) continue;

        // closure violation: same TID-set
        if (utility(newUL) == utility(prefixUL))
           closed = false;


        vector<string> newPrefix = prefix;
        newPrefix.push_back(items[i].name);

        DFS(newPrefix, newUL, i + 1);

    }

    if (!prefix.empty() && closed) {
        CHUIs[makeKey(prefix)] = utility(prefixUL);
    }
}

/* ---------- MAIN ---------- */

int main() {

    MIN_UTIL = 6;

    /* ---------- READ DATABASE ---------- */
    ifstream fin("data.txt");
    vector<vector<pair<string,int>>> DB;
    string line, item;
    int val;

    while (getline(fin, line)) {
        stringstream ss(line);
        vector<pair<string,int>> trans;
        while (ss >> item >> val)
            trans.push_back({item, val});
        if (!trans.empty())
            DB.push_back(trans);
    }

    /* ---------- BUILD LEVEL-1 UTILITY LISTS ---------- */
    unordered_map<string, vector<UEntry>> temp;

    for (int tid = 0; tid < DB.size(); tid++) {
        for (auto &p : DB[tid]) {
            temp[p.first].push_back({tid, p.second});
        }
    }

    for (auto &p : temp)
        items.push_back({p.first, p.second});

    /* ---------- SORT ITEMS LEXICOGRAPHICALLY ---------- */
    sort(items.begin(), items.end(),
         [](const Item& a, const Item& b) {
             return a.name < b.name;
         });

    /* ---------- DFS FROM EACH SINGLE ITEM ---------- */
    for (int i = 0; i < items.size(); i++) {
        if (utility(items[i].ul) >= MIN_UTIL) {
            DFS({items[i].name}, items[i].ul, i + 1);
        }
    }

    /* ---------- PRINT RESULT ---------- */
    cout << "\nClosed High Utility Itemsets:\n";
    for (auto &x : CHUIs) {
        cout << "{ " << x.first << "} -> Utility = " << x.second << "\n";
    }

    return 0;
}
