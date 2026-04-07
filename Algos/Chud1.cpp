#include <bits/stdc++.h>
using namespace std;

/* ---------------- DATA STRUCTURES ---------------- */

struct Transaction {
    int tid;
    vector<pair<string,int>> items; // (item, quantity)
};

vector<Transaction> database;
unordered_map<string,int> profit;
unordered_map<string,long long> TWU;
unordered_map<string,long long> exactUtility;

/* ---------------- HELPER FUNCTIONS ---------------- */

// check if transaction contains itemset X
bool containsAll(Transaction &t, vector<string> &X) {
    unordered_map<string,int> mp;
    for (auto &p : t.items)
        mp[p.first] = p.second;

    for (auto &item : X)
        if (mp.find(item) == mp.end())
            return false;
    return true;
}

// compute utility of X in transaction T
long long utilityInTransaction(Transaction &t, vector<string> &X) {
    long long u = 0;
    unordered_map<string,int> mp;
    for (auto &p : t.items)
        mp[p.first] = p.second;

    for (auto &item : X)
        u += mp[item] * profit[item];
    return u;
}

// convert itemset to string key
string makeKey(vector<string> &X) {
    string k = "";
    for (auto &i : X) k += i + ",";
    return k;
}

/* ---------------- STEP 1: COMPUTE TWU ---------------- */

void computeTWU() {
    for (auto &t : database) {
        long long TU = 0;
        for (auto &p : t.items)
            TU += p.second * profit[p.first];

        for (auto &p : t.items)
            TWU[p.first] += TU;
    }
}

/* ---------------- STEP 2: GENERATE 1-ITEMSETS ---------------- */

vector<vector<string>> generateL1(long long minUtil) {
    vector<vector<string>> L1;
    for (auto &p : TWU) {
        if (p.second >= minUtil)
            L1.push_back({p.first});
    }
    sort(L1.begin(), L1.end());
    return L1;
}

/* ---------------- STEP 3: JOIN ITEMSETS ---------------- */

vector<vector<string>> joinItemsets(vector<vector<string>> &prev) {
    vector<vector<string>> next;
    int n = prev.size();

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            bool ok = true;
            for (int k = 0; k + 1 < prev[i].size(); k++) {
                if (prev[i][k] != prev[j][k]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                vector<string> candidate = prev[i];
                candidate.push_back(prev[j].back());
                next.push_back(candidate);
            }
        }
    }
    return next;
}

/* ---------------- STEP 4: EXACT UTILITY ---------------- */

void computeExactUtility(vector<vector<string>> &candidates,
                         long long minUtil) {

    for (auto &X : candidates) {
        long long totalUtility = 0;

        for (auto &t : database) {
            if (containsAll(t, X))
                totalUtility += utilityInTransaction(t, X);
        }

        if (totalUtility >= minUtil) {
            exactUtility[makeKey(X)] = totalUtility;
        }
    }
}

/* ---------------- STEP 5: CLOSED CHECKING ---------------- */

void outputCHUIs() {
    cout << "\nClosed High Utility Itemsets:\n";

    for (auto &x : exactUtility) {
        bool closed = true;

        for (auto &y : exactUtility) {
            if (x.first != y.first &&
                y.first.find(x.first) != string::npos &&
                x.second == y.second) {
                closed = false;
                break;
            }
        }

        if (closed)
            cout << x.first << " -> " << x.second << endl;
    }
}

/* ---------------- MAIN ---------------- */

int main() {

    /* PROFIT TABLE */
    profit["A"] = 5;
    profit["B"] = 2;
    profit["C"] = 3;

    /* DATABASE */
    database.push_back({1, {{"A",2},{"B",1},{"C",3}}});
    database.push_back({2, {{"A",1},{"C",2}}});
    database.push_back({3, {{"B",2},{"C",1}}});

    long long minUtil = 20;

    /* CHUD FLOW */
    computeTWU();
    vector<vector<string>> L = generateL1(minUtil);

    while (!L.empty()) {
        computeExactUtility(L, minUtil);
        L = joinItemsets(L);
    }

    outputCHUIs();
    return 0;
}
