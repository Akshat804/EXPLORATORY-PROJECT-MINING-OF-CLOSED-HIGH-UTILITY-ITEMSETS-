#include <bits/stdc++.h>
using namespace std;

int MIN_UTIL = 6;

/* ---------- READ DATABASE (FIXED) ---------- */
vector<map<string,int>> readDatabase(string filename) {
    
    vector<map<string,int>> db;
    ifstream fin(filename);
    string line, item;
    int util;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        map<string,int> transaction;
        stringstream ss(line);
        while (ss >> item >> util)
            transaction[item] = util;
        db.push_back(transaction);
    }
    fin.close();
    return db;
}

/* ---------- UTILITY OF ITEMSET ---------- */
int utilityOfSet(const set<string>& itemset,
                 const vector<map<string,int>>& db) {

    int total = 0;
    for (auto &trans : db) {
        bool present = true;
        int u = 0;
        for (auto &it : itemset) {
            if (!trans.count(it)) {
                present = false;
                break;
            }
            u += trans.at(it);
        }
        if (present) total += u;
    }
    return total;
}

/* ---------- APRIORI JOIN ---------- */
set<set<string>> aprioriJoin(const set<set<string>>& prev) {
    set<set<string>> candidates;
    for (auto it1 = prev.begin(); it1 != prev.end(); ++it1) {
        for (auto it2 = next(it1); it2 != prev.end(); ++it2) {
            set<string> joined = *it1;
            joined.insert(it2->begin(), it2->end());
            if (joined.size() == it1->size() + 1)
                candidates.insert(joined);
        }
    }
    return candidates;
}

/* ---------- MAIN ---------- */
int main() {

    vector<map<string,int>> database = readDatabase("data.txt");

    /* ---------- 1-ITEMSETS ---------- */
    map<string,int> singleUtil;
    for (auto &trans : database)
        for (auto &p : trans)
            singleUtil[p.first] += p.second;

    set<set<string>> Hk;
    for (auto &p : singleUtil)
        if (p.second >= MIN_UTIL)
            Hk.insert({p.first});

    vector<set<set<string>>> allLevels;
    allLevels.push_back(Hk);

    /* ---------- LEVEL-WISE GENERATION ---------- */
    while (!Hk.empty()) {
        set<set<string>> Ck1 = aprioriJoin(Hk);
        set<set<string>> Hk1;

        for (auto &cand : Ck1) {
            if (utilityOfSet(cand, database) >= MIN_UTIL)
                Hk1.insert(cand);
        }

        if (Hk1.empty()) break;
        allLevels.push_back(Hk1);
        Hk = Hk1;
    }

    /* ---------- CORRECT CLOSURE CHECK ---------- */
    cout << "Closed High Utility Itemsets:\n";

    for (int i = 0; i < allLevels.size(); i++) {
        for (auto &X : allLevels[i]) {
            int uX = utilityOfSet(X, database);
            bool closed = true;

            // check ALL higher levels
            for (int j = i + 1; j < allLevels.size() && closed; j++) {
                for (auto &Y : allLevels[j]) {
                    if (includes(Y.begin(), Y.end(),
                                 X.begin(), X.end())) {
                        if (utilityOfSet(Y, database) == uX) {
                            closed = false;
                            break;
                        }
                    }
                }
            }

            if (closed) {
                cout << "{ ";
                for (auto &it : X) cout << it << " ";
                cout << "} -> Utility = " << uX << endl;
            }
        }
    }

    return 0;
}
