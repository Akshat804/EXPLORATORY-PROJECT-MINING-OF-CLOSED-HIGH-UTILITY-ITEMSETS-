#include <bits/stdc++.h>
using namespace std;

int main() {

    ofstream fout("data.txt");

    // Item pool (realistic grocery dataset)
    vector<string> items = {
        "bread","milk","butter","rice","chicken","fish","meat",
        "oil","sugar","tea","coffee","salt","flour","cheese",
        "yogurt","honey","eggs","juice","chips","biscuits",
        "snacks","cookies","vegetables","fruits","spices"
    };

    int numTransactions = 20000;  // Change to 10000 if needed
    int maxItemsPerTransaction = 8;

    srand(time(0));

    for (int t = 0; t < numTransactions; t++) {

        int numItems = rand() % maxItemsPerTransaction + 2;

        set<int> chosen;

        while (chosen.size() < numItems)
            chosen.insert(rand() % items.size());

        for (int idx : chosen) {
            int util = rand() % 20 + 1;  // Utility 1–20
            fout << items[idx] << " " << util << " ";
        }

        fout << "\n";
    }

    fout.close();

    cout << "Dataset generated successfully: data.txt\n";
    return 0;
}
