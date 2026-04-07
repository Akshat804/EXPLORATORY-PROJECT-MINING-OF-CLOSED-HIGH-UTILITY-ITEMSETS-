#include <bits/stdc++.h>
using namespace std;

/* ================= CONFIG ================= */

int minUtil = 10000;

/* ================= DATA STRUCTURES ================= */

struct Node {
    string item;
    int sumIUtil;     // utility of this item at this node
    int sumRUtil;     // remaining utility
    Node* parent;
    unordered_map<string, Node*> children;
    Node* nodeLink;

    Node(string it, Node* p) {
        item = it;
        parent = p;
        sumIUtil = 0;
        sumRUtil = 0;
        nodeLink = nullptr;
    }
};

struct Tree {
    Node* root;
    unordered_map<string, Node*> header;
};

/* ================= GLOBAL ================= */

vector<vector<pair<string,int>>> DB;
unordered_map<string,int> TWU;
map<vector<string>, int> HUIs;

/* ================= READ DATABASE ================= */

void readDB(const string& file) {
    ifstream fin(file);
    string line;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string item;
        int util;

        vector<pair<string,int>> trans;

        while (ss >> item >> util)
            trans.push_back({item, util});

        DB.push_back(trans);
    }
}

/* ================= TWU ================= */

void computeTWU() {
    for (auto &t : DB) {
        int tu = 0;
        for (auto &p : t)
            tu += p.second;

        for (auto &p : t)
            TWU[p.first] += tu;
    }
}

/* ================= INSERT TRANSACTION ================= */

void insertTransaction(Tree &T,
                       vector<pair<string,int>> trans) {

    Node* cur = T.root;

    int remaining = 0;
    for (auto &p : trans)
        remaining += p.second;

    for (int i = 0; i < trans.size(); i++) {
        string item = trans[i].first;
        int util = trans[i].second;

        remaining -= util;

        if (!cur->children.count(item)) {
            Node* newNode = new Node(item, cur);
            cur->children[item] = newNode;

            if (!T.header[item])
                T.header[item] = newNode;
            else {
                Node* tmp = T.header[item];
                while (tmp->nodeLink)
                    tmp = tmp->nodeLink;
                tmp->nodeLink = newNode;
            }
        }

        cur = cur->children[item];
        cur->sumIUtil += util;
        cur->sumRUtil += remaining;
    }
}

/* ================= BUILD TREE ================= */

Tree buildTree(vector<vector<pair<string,int>>> dataset) {

    Tree T;
    T.root = new Node("ROOT", nullptr);

    for (auto &t : dataset)
        insertTransaction(T, t);

    return T;
}

/* ================= COMPUTE UTILITY ================= */

int computeUtility(Tree &T, string item) {
    int total = 0;
    Node* n = T.header[item];

    while (n) {
        total += n->sumIUtil;
        n = n->nodeLink;
    }
    return total;
}

/* ================= MINE ================= */

void mine(Tree &T, vector<string> prefix) {

    vector<string> items;

    for (auto &h : T.header)
        items.push_back(h.first);

    sort(items.begin(), items.end(),
         [&](string a, string b) {
             return TWU[a] < TWU[b];
         });

    for (auto &item : items) {

        vector<string> newPrefix = prefix;
        newPrefix.push_back(item);

        int util = computeUtility(T, item);

        if (util >= minUtil) {
            vector<string> sortedPrefix = newPrefix;
            sort(sortedPrefix.begin(), sortedPrefix.end());
            HUIs[sortedPrefix] = util;
        }

        // Build conditional pattern base
        vector<vector<pair<string,int>>> condDB;

        Node* node = T.header[item];

        while (node) {

            vector<pair<string,int>> path;
            Node* parent = node->parent;

            while (parent && parent->item != "ROOT") {
                path.push_back({parent->item, node->sumIUtil});
                parent = parent->parent;
            }

            if (!path.empty()) {
                reverse(path.begin(), path.end());
                condDB.push_back(path);
            }

            node = node->nodeLink;
        }

        if (!condDB.empty()) {
            Tree condTree = buildTree(condDB);
            mine(condTree, newPrefix);
        }
    }
}

/* ================= CLOSED CHECK ================= */

bool isSubset(const vector<string>& a,
              const vector<string>& b) {
    return includes(b.begin(), b.end(),
                    a.begin(), a.end());
}

/* ================= MAIN ================= */

int main() {

    readDB("data1.txt");
    computeTWU();

    // Filter and sort DB
    for (auto &t : DB) {
        vector<pair<string,int>> filtered;

        for (auto &p : t)
            if (TWU[p.first] >= minUtil)
                filtered.push_back(p);

        sort(filtered.begin(), filtered.end(),
             [&](auto &a, auto &b) {
                 return TWU[a.first] > TWU[b.first];
             });

        t = filtered;
    }

    Tree T = buildTree(DB);

    mine(T, {});

    cout << "\nClosed High Utility Itemsets:\n\n";

    for (auto &p : HUIs) {

        bool closed = true;

        for (auto &q : HUIs) {

            if (p.first == q.first) continue;

            if (q.first.size() > p.first.size() &&
                q.second == p.second &&
                isSubset(p.first, q.first)) {

                closed = false;
                break;
            }
        }

        if (closed) {
            cout << "{ ";
            for (auto &x : p.first)
                cout << x << " ";
            cout << "} Utility = "
                 << p.second << "\n";
        }
    }

    return 0;
}
