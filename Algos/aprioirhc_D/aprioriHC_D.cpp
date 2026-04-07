#include <bits/stdc++.h>
using namespace std;

int MIN_UTIL = 6;

/* ---------- READ DATABASE ---------- */
vector<map<string,int>> readDatabase(string filename) {
    vector<map<string,int>> db;
    ifstream fin(filename);
    string line, item;
    int util;

    while (getline(fin, line)) {
        if (line.empty()) continue;
        map<string,int> trans;
        stringstream ss(line);
        while (ss >> item >> util)
            trans[item] = util;
        db.push_back(trans);
    }
    fin.close();
    return db;
}

/* ---------- TRANSACTION UTILITY ---------- */

int transactionUtility(const map<string,int>& trans) {
    int sum = 0;
    for (auto &p : trans) sum += p.second;
    return sum;
}

/* ---------- UTILITY OF ITEMSET ---------- */

int utilityOfSet(const set<string>& itemset,
                 const vector<map<string,int>>& db) {
                    
    int total = 0;
    for (auto &trans : db) {
        bool ok = true;
        int u = 0;
        for (auto &it : itemset) {
            if (!trans.count(it)) {
                ok = false;
                break;
            }
            u += trans.at(it);
        }
        if (ok) total += u;
    }
    return total;
}

/* ---------- APRIORI JOIN ---------- */
set<set<string>> aprioriJoin(const set<set<string>>& prev) {
    set<set<string>> cand;
    for (auto it1 = prev.begin(); it1 != prev.end(); ++it1) {
        for (auto it2 = next(it1); it2 != prev.end(); ++it2) {
            set<string> joined = *it1;
            joined.insert(it2->begin(), it2->end());
            if (joined.size() == it1->size() + 1)
                cand.insert(joined);
        }
    }
    return cand;
}

/* ---------- MAIN ---------- */
int main() {

    vector<map<string,int>> DB = readDatabase("data.txt");

    /* ---------- STEP 1: COMPUTE TWU ---------- */
    map<string,int> TWU;
    for (auto &trans : DB) {
        int tu = transactionUtility(trans);
        for (auto &p : trans)
            TWU[p.first] += tu;
    }

    /* ---------- STEP 2: DGU (SAFE) ---------- */
    set<string> promisingItems;
    for (auto &p : TWU)
        if (p.second >= MIN_UTIL)
            promisingItems.insert(p.first);

    /* ---------- STEP 3: 1-ITEMSETS ---------- */
    map<string,int> singleUtil;
    for (auto &trans : DB)
        for (auto &p : trans)
            if (promisingItems.count(p.first))
                singleUtil[p.first] += p.second;

    set<set<string>> Hk;
    for (auto &p : singleUtil)
        if (p.second >= MIN_UTIL)
            Hk.insert({p.first});

    vector<set<set<string>>> allLevels;
    allLevels.push_back(Hk);

    /* ---------- STEP 4: LEVEL-WISE MINING ---------- */
    while (!Hk.empty()) {
        set<set<string>> Ck1 = aprioriJoin(Hk);
        set<set<string>> Hk1;

        for (auto &cand : Ck1) {
            // prune if any item is globally unpromising
            bool valid = true;
            for (auto &it : cand)
                if (!promisingItems.count(it))
                    valid = false;

            if (!valid) continue;

            int u = utilityOfSet(cand, DB);
            if (u >= MIN_UTIL)
                Hk1.insert(cand);
        }

        if (Hk1.empty()) break;
        allLevels.push_back(Hk1);
        Hk = Hk1;
    }

    /* ---------- STEP 5: CORRECT CLOSURE CHECK ---------- */
    cout << "\nClosed High Utility Itemsets:\n";

    for (int i = 0; i < allLevels.size(); i++) {
        for (auto &X : allLevels[i]) {
            int uX = utilityOfSet(X, DB);
            bool closed = true;

            // CHECK AGAINST ALL SUPERSETS
            for (int j = i + 1; j < allLevels.size() && closed; j++) {
                for (auto &Y : allLevels[j]) {
                    if (includes(Y.begin(), Y.end(),
                                 X.begin(), X.end())) {
                        if (utilityOfSet(Y, DB) == uX) {
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
