#include <iostream>
#include <queue>
#include <unordered_map>
#include <deque>

using namespace std;

enum VehicleType {
    TWO_WHEELER,
    THREE_WHEELER,
    FOUR_WHEELER,
    INVALID
};

struct VehicleInfo {
    string number;
    VehicleType type;
    string token;
};

class ParkingLot {
private:
    int tier1Slots, tier2Slots;

    priority_queue<int, deque<int>, greater<int>> tier1Available;
    priority_queue<int, deque<int>, greater<int>> tier2Available;

    unordered_map<string, pair<int, VehicleInfo>> tier1Occupied;
    unordered_map<string, pair<int, VehicleInfo>> tier2Occupied;

public:
    ParkingLot(int twoWheelerCount, int threeFourWheelerCount) {
        tier2Slots = twoWheelerCount;
        tier1Slots = threeFourWheelerCount;

        for (int i = 1; i <= tier1Slots; ++i)
            tier1Available.push(i);
        for (int i = 1; i <= tier2Slots; ++i)
            tier2Available.push(i);
    }

    VehicleType getVehicleType(int type) {
        switch (type) {
            case 2: return TWO_WHEELER;
            case 3: return THREE_WHEELER;
            case 4: return FOUR_WHEELER;
            default: return INVALID;
        }
    }

    bool paymentProcess(VehicleType type) {
        int price = (type == TWO_WHEELER) ? 20 : 30;
        char confirm;
        do {
            cout << "Parking Price for " << (type == TWO_WHEELER ? "2-wheeler" : "3/4-wheeler") << ": Rs." << price << endl;
            cout << "Proceed with payment? (Y/N): ";
            cin >> confirm;

            if (confirm == 'Y' || confirm == 'y') {
                cout << "Payment Successful!" << endl;
                return true;
            } else {
                int retryChoice;
                cout << "Payment Failed!" << endl;
                cout << "Choose an option:\n1) Cancel Parking\n2) Try Again\n3) Go to Main Menu\nEnter your choice: ";
                cin >> retryChoice;
                if (retryChoice == 1) exit(0);
                else if (retryChoice == 2) continue;
                else return false;
            }
        } while (true);
    }

    void parkVehicle(VehicleType type, const string& number) {
        if (type == INVALID) {
            cout << "Invalid vehicle type." << endl;
            return;
        }

        if (!paymentProcess(type)) return;

        if (type == TWO_WHEELER) {
            if (tier2Available.empty()) {
                cout << "No Tier 2 slots available." << endl;
                return;
            }
            int slot = tier2Available.top();
            tier2Available.pop();
            string token = "T2S" + to_string(slot);
            tier2Occupied[token] = {slot, {number, type, token}};
            cout << "Vehicle parked! Token: " << token << " | Slot: " << slot << " | Tier: 2\n";
        } else {
            if (tier1Available.empty()) {
                cout << "No Tier 1 slots available." << endl;
                return;
            }
            int slot = tier1Available.top();
            tier1Available.pop();
            string token = "T1S" + to_string(slot);
            tier1Occupied[token] = {slot, {number, type, token}};
            cout << "Vehicle parked! Token: " << token << " | Slot: " << slot << " | Tier: 1\n";
        }

        showSlotStatus();
    }

    void removeVehicle(const string& token) {
        if (token.substr(0, 2) == "T1" && tier1Occupied.count(token)) {
            int slot = tier1Occupied[token].first;
            cout << tier1Occupied[token].second.number << " removed from Tier 1 - Slot " << slot << endl;
            tier1Available.push(slot);
            tier1Occupied.erase(token);
        } else if (token.substr(0, 2) == "T2" && tier2Occupied.count(token)) {
            int slot = tier2Occupied[token].first;
            cout << tier2Occupied[token].second.number << " removed from Tier 2 - Slot " << slot << endl;
            tier2Available.push(slot);
            tier2Occupied.erase(token);
        } else {
            cout << "Invalid token or vehicle not found." << endl;
        }
        showSlotStatus();
    }

    void showSlotStatus() {
        cout << "\nTier 1 Slots (3/4-Wheelers):\n";
        for (int i = 1; i <= tier1Slots; ++i) {
            string token = "T1S" + to_string(i);
            if (tier1Occupied.count(token))
                cout << "Slot " << i << ": Occupied by " << tier1Occupied[token].second.number << " (Token: " << token << ")\n";
            else
                cout << "Slot " << i << ": Free\n";
        }

        cout << "\nTier 2 Slots (2-Wheelers):\n";
        for (int i = 1; i <= tier2Slots; ++i) {
            string token = "T2S" + to_string(i);
            if (tier2Occupied.count(token))
                cout << "Slot " << i << ": Occupied by " << tier2Occupied[token].second.number << " (Token: " << token << ")\n";
            else
                cout << "Slot " << i << ": Free\n";
        }
        cout << endl;
    }

    void displayHelp() {
        cout << "\n===== Help Menu =====\n";
        cout << "1. Remove Vehicle (by Token)\n";
        cout << "2. Show Slot Status\n";
        cout << "3. Help Menu\n";
        cout << "4. Exit\n";
    }
};

int main() {
    int count2, count34;
    cout << "How many 2-wheeler vehicles to allow initially? ";
    cin >> count2;
    cout << "How many 3/4-wheeler vehicles to allow initially? ";
    cin >> count34;

    ParkingLot lot(count2, count34);

    // Book 2-wheeler slots
    if (count2 > 0) {
        cout << "\n--- Register " << count2 << " Two-Wheelers ---\n";
        for (int i = 0; i < count2; ++i) {
            string number;
            cout << "Enter vehicle number for 2-wheeler " << (i + 1) << ": ";
            cin >> number;
            lot.parkVehicle(TWO_WHEELER, number);
        }
    }

    // Book 3/4-wheeler slots
    if (count34 > 0) {
        cout << "\n--- Register " << count34 << " Three/Four-Wheelers ---\n";
        for (int i = 0; i < count34; ++i) {
            string number;
            int wheeler;
            cout << "Enter 3 or 4 for vehicle type for vehicle " << (i + 1) << ": ";
            cin >> wheeler;
            VehicleType type = lot.getVehicleType(wheeler);
            if (type != THREE_WHEELER && type != FOUR_WHEELER) {
                cout << "Invalid type! Skipping...\n";
                continue;
            }
            cout << "Enter vehicle number: ";
            cin >> number;
            lot.parkVehicle(type, number);
        }
    }

    // Main menu
    int choice;
    do {
        cout << "\n===== Smart Parking Menu =====\n";
        cout << "1. Remove Vehicle\n";
        cout << "2. Show Slot Status\n";
        cout << "3. Help\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string token;
            cout << "Enter your Token (e.g., T1S1): ";
            cin >> token;
            lot.removeVehicle(token);
        } else if (choice == 2) {
            lot.showSlotStatus();
        } else if (choice == 3) {
            lot.displayHelp();
        } else if (choice == 4) {
            cout << "Thank you for using Smart Parking System!\n";
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
