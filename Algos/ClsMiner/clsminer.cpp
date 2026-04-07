#include <bits/stdc++.h>
using namespace std;

/* ===================== DATA STRUCTURES ===================== */

struct ULEntry {
    int tid;
    int iutil;
    int rutil;
};

struct UtilityList {
    vector<int> items;
    vector<ULEntry> entries;
};

/* ===================== GLOBALS ===================== */

int minUtil = 6;
vector<pair<vector<int>, int>> result;

/* ===================== UTILITY FUNCTIONS ===================== */

int utility(const UtilityList &ul) {
    int sum = 0;
    for (auto &e : ul.entries)
        sum += e.iutil;
    return sum;
}

int upperBound(const UtilityList &ul) {
    int sum = 0;
    for (auto &e : ul.entries)
        sum += e.iutil + e.rutil;
    return sum;
}

/* ===================== JOIN ===================== */

UtilityList joinUL(const UtilityList &A, const UtilityList &B) {
    UtilityList C;
    C.items = A.items;
    C.items.push_back(B.items.back());

    unordered_map<int, ULEntry> mp;
    for (auto &e : B.entries)
        mp[e.tid] = e;

    for (auto &ea : A.entries) {
        if (mp.count(ea.tid)) {
            auto &eb = mp[ea.tid];
            C.entries.push_back({
                ea.tid,
                ea.iutil + eb.iutil,
                eb.rutil
            });
        }
    }
    return C;
}

/* ===================== CLOSURE (CORRECT) ===================== */

bool sameTIDs(const UtilityList &A, const UtilityList &B) {
    if (A.entries.size() != B.entries.size()) return false;
    for (int i = 0; i < A.entries.size(); i++)
        if (A.entries[i].tid != B.entries[i].tid)
            return false;
    return true;
}

bool isClosed(const UtilityList &ul,
              const vector<UtilityList> &extensions) {
    for (auto &e : extensions) {
       if (!e.entries.empty() &&
    sameTIDs(ul, e) &&
    utility(ul) == utility(e))
        return false;

    }
    return true;
}

/* ===================== DFS ===================== */

void dfs(const vector<UtilityList> &ULs) {
    for (int i = 0; i < ULs.size(); i++) {
        const UtilityList &X = ULs[i];

        if (utility(X) >= minUtil) {
            vector<UtilityList> ext;
            for (int j = i + 1; j < ULs.size(); j++)
                ext.push_back(joinUL(X, ULs[j]));

            if (isClosed(X, ext))
                result.push_back({X.items, utility(X)});
        }

        vector<UtilityList> next;
        for (int j = i + 1; j < ULs.size(); j++) {
            UtilityList Z = joinUL(X, ULs[j]);
            if (!Z.entries.empty() &&
                upperBound(Z) >= minUtil)
                next.push_back(Z);
        }

        if (!next.empty())
            dfs(next);
    }
}

/* ===================== MAIN ===================== */

int main() {
    ifstream fin("data.txt");
    if (!fin) return 1;

    unordered_map<string,int> id;
    vector<string> name;
    unordered_map<int, vector<pair<int,int>>> tx;

    string line;
    int tid = 0;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);

        string item;
        int util;
        while (ss >> item >> util) {
            if (!id.count(item)) {
                id[item] = name.size();
                name.push_back(item);
            }
            tx[tid].push_back({id[item], util});
        }
        tid++;
    }

    int m = name.size();

    vector<int> twu(m, 0);
    for (auto &t : tx) {
        int tu = 0;
        for (auto &p : t.second) tu += p.second;
        for (auto &p : t.second) twu[p.first] += tu;
    }

    vector<int> items;
    for (int i = 0; i < m; i++)
        if (twu[i] >= minUtil)
            items.push_back(i);

    sort(items.begin(), items.end(),
         [&](int a, int b) { return twu[a] < twu[b]; });

    unordered_map<int,int> order;
    for (int i = 0; i < items.size(); i++)
        order[items[i]] = i;

    vector<UtilityList> ULs;

    for (int it : items) {
        UtilityList ul;
        ul.items = {it};

        for (auto &t : tx) {
            int iu = 0, ru = 0;
            bool seen = false;

            for (auto &p : t.second)
                if (p.first == it) {
                    iu = p.second;
                    seen = true;
                }

            if (seen) {
                for (auto &p : t.second)
                    if (order.count(p.first) &&
                        order[p.first] > order[it])
                        ru += p.second;

                ul.entries.push_back({t.first, iu, ru});
            }
        }
        ULs.push_back(ul);
    }

    dfs(ULs);

    cout << "Closed High-Utility Itemsets (minUtil = "
         << minUtil << "):\n";

    for (auto &r : result) {
        cout << "{";
        for (int i = 0; i < r.first.size(); i++) {
            cout << name[r.first[i]];
            if (i + 1 < r.first.size()) cout << ",";
        }
        cout << "} Utility = " << r.second << "\n";
    }
}
