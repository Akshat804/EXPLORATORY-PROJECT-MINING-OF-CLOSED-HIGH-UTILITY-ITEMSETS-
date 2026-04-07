#include <bits/stdc++.h>
using namespace std;

int main() {

    ofstream fout("data1.txt");

    // Large realistic item pool (100+ distinct items)
    vector<string> items = {

        // Electronics
        "laptop","tablet","smartphone","monitor","keyboard","mouse",
        "printer","scanner","router","modem","webcam","headphones",
        "speaker","microphone","harddrive","ssd","usbdrive","graphicscard",
        "processor","motherboard","ram","powersupply","coolingfan",

        // Software & Services
        "antivirus","cloudstorage","vpn","office365","photoshop",
        "illustrator","autocad","matlab","spotify","netflix",
        "amazonprime","github","dropbox","slack","zoom",
        "notion","canva","trello","figma","aws",

        // Office Equipment
        "notebook","pen","pencil","marker","stapler","envelope",
        "folder","calculator","whiteboard","projector","desk",
        "chair","lamp","filingcabinet","shredder",

        // Home & Lifestyle
        "airconditioner","refrigerator","microwave","washingmachine",
        "television","vacuumcleaner","fan","heater","sofa","bed",
        "curtains","carpet","mirror","clock","bookshelf",

        // Fitness
        "treadmill","dumbbells","yogamat","proteinpowder",
        "fitnessband","cyclingbike","skippingrope","sportswatch",

        // Automotive
        "carbattery","engineoil","dashboardcamera","carcover",
        "helmet","toolkit","gps","seatcover",

        // Gaming
        "gamingconsole","controller","vrheadset","gamingmouse",
        "mechanicalkeyboard","gamingchair","gamepad","joystick",

        // Education
        "textbook","onlinecourse","examguide","researchpaper",
        "labkit","scientificcalculator","eBook","journal",

        // Miscellaneous
        "camera","tripod","powerbank","extensioncord",
        "waterfilter","securitycamera","doorlock","smartswitch",
        "bluetoothadapter","networkcable","hdmicable"
    };

    int numTransactions = 20000;      // Adjust if needed
    int maxItemsPerTransaction = 10;  // Up to 10 items per txn

    srand(time(0));

    for (int t = 0; t < numTransactions; t++) {

        int numItems = rand() % maxItemsPerTransaction + 3;

        set<int> chosen;

        while (chosen.size() < numItems)
            chosen.insert(rand() % items.size());

        for (int idx : chosen) {
            int util = rand() % 50 + 1;  // Utility 1–50
            fout << items[idx] << " " << util << " ";
        }

        fout << "\n";
    }

    fout.close();

    cout << "Large dataset generated successfully: data.txt\n";
    return 0;
}
