#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

// ============================================================
// BAI 1: CAI DAT 3 THUAT TOAN SAP XEP
// ============================================================

void bubbleSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Bubble Sort ---\n";
    cout << "Ban dau: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        cout << "Buoc " << i + 1 << ": ";
        for (int x : arr) cout << x << " ";
        cout << endl;
        if (!swapped) break; // Toi uu: dung som neu da sap xep
    }
}

void selectionSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Selection Sort ---\n";
    cout << "Ban dau: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) swap(arr[i], arr[minIdx]);
        cout << "Buoc " << i + 1 << ": ";
        for (int x : arr) cout << x << " ";
        cout << endl;
    }
}

void insertionSort(vector<int> arr) {
    int n = arr.size();
    cout << "\n--- Insertion Sort ---\n";
    cout << "Ban dau: ";
    for (int x : arr) cout << x << " ";
    cout << endl;

    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
        cout << "Buoc " << i << ": ";
        for (int x : arr) cout << x << " ";
        cout << endl;
    }
}

// ============================================================
// BAI 2: DEM SO PHEP SO SANH VA HOAN VI
// ============================================================

struct Stats {
    long long comparisons = 0;
    long long swaps = 0;
};

Stats bubbleSortCount(vector<int> arr) {
    Stats s;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            s.comparisons++;
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                s.swaps++;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
    return s;
}

Stats selectionSortCount(vector<int> arr) {
    Stats s;
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            s.comparisons++;
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
            s.swaps++;
        }
    }
    return s;
}

Stats insertionSortCount(vector<int> arr) {
    Stats s;
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            s.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                s.swaps++;
                j--;
            }
            else break;
        }
        arr[j + 1] = key;
    }
    return s;
}

void bai2() {
    const int N = 100;
    vector<int> arr(N);
    srand(42); // seed co dinh de ket qua nhat quan
    for (int i = 0; i < N; i++) arr[i] = rand() % 1000;

    Stats bs = bubbleSortCount(arr);
    Stats ss = selectionSortCount(arr);
    Stats is = insertionSortCount(arr);

    cout << "\n============================================================\n";
    cout << "BAI 2: SO SANH SO PHEP SO SANH VA HOAN VI (mang 100 phan tu)\n";
    cout << "============================================================\n";
    cout << left << setw(20) << "Thuat toan"
        << setw(20) << "So sanh"
        << setw(20) << "Hoan vi" << endl;
    cout << string(60, '-') << endl;
    cout << left << setw(20) << "Bubble Sort"
        << setw(20) << bs.comparisons
        << setw(20) << bs.swaps << endl;
    cout << left << setw(20) << "Selection Sort"
        << setw(20) << ss.comparisons
        << setw(20) << ss.swaps << endl;
    cout << left << setw(20) << "Insertion Sort"
        << setw(20) << is.comparisons
        << setw(20) << is.swaps << endl;
}

// ============================================================
// BAI 3: BEST / WORST / AVERAGE CASE
// ============================================================

using Clock = chrono::high_resolution_clock;

double measureBubble(vector<int> arr) {
    auto start = Clock::now();
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) { swap(arr[j], arr[j + 1]); swapped = true; }
        if (!swapped) break;
    }
    return chrono::duration<double, micro>(Clock::now() - start).count();
}

double measureSelection(vector<int> arr) {
    auto start = Clock::now();
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[minIdx]) minIdx = j;
        if (minIdx != i) swap(arr[i], arr[minIdx]);
    }
    return chrono::duration<double, micro>(Clock::now() - start).count();
}


double measureInsertion(vector<int> arr) {
    auto start = Clock::now();
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; j--; }
        arr[j + 1] = key;
    }
    return chrono::duration<double, micro>(Clock::now() - start).count();
}

void bai3() {
    const int N = 5000;
    srand(42);

    // Best case: mang da sap xep tang dan
    vector<int> best(N);
    for (int i = 0; i < N; i++) best[i] = i;

    // Worst case: mang sap xep nguoc chieu
    vector<int> worst(N);
    for (int i = 0; i < N; i++) worst[i] = N - i;

    // Average case: mang ngau nhien
    vector<int> avg(N);
    for (int i = 0; i < N; i++) avg[i] = rand() % 10000;

    cout << "\n============================================================\n";
    cout << "BAI 3: BEST / WORST / AVERAGE CASE (mang " << N << " phan tu)\n";
    cout << "============================================================\n";
    cout << left << setw(20) << "Thuat toan"
        << setw(20) << "Best (us)"
        << setw(20) << "Worst (us)"
        << setw(20) << "Average (us)" << endl;
    cout << string(80, '-') << endl;

    cout << fixed << setprecision(2);
    cout << left << setw(20) << "Bubble Sort"
        << setw(20) << measureBubble(best)
        << setw(20) << measureBubble(worst)
        << setw(20) << measureBubble(avg) << endl;

    cout << left << setw(20) << "Selection Sort"
        << setw(20) << measureSelection(best)
        << setw(20) << measureSelection(worst)
        << setw(20) << measureSelection(avg) << endl;

    cout << left << setw(20) << "Insertion Sort"
        << setw(20) << measureInsertion(best)
        << setw(20) << measureInsertion(worst)
        << setw(20) << measureInsertion(avg) << endl;

    cout << "\nKET LUAN:\n";
    cout << "- Insertion Sort: tot nhat voi mang GAN NHU da sap xep (best case O(n))\n";
    cout << "- Selection Sort: so sanh it hon Bubble, nhung luon O(n^2)\n";
    cout << "- Bubble Sort:    cham nhat trong worst/average case\n";
    cout << "=> Khuyen nghi:\n";
    cout << "   + Mang nho (<1000):    Dung ca 3 deu duoc\n";
    cout << "   + Mang gan sap xep:    Dung Insertion Sort\n";
    cout << "   + Mang lon ngau nhien: Dung thuat toan tot hon (QuickSort, MergeSort)\n";
}

// ============================================================
// MAIN
// ============================================================

int main() {
    // Bai 1: demo voi mang nho
    cout << "============================================================\n";
    cout << "BAI 1: CAI DAT 3 THUAT TOAN - IN TUNG BUOC\n";
    cout << "============================================================\n";
    vector<int> demo = { 64, 34, 25, 12, 22, 11, 90 };
    bubbleSort(demo);
    selectionSort(demo);
    insertionSort(demo);

    // Bai 2
    bai2();

    // Bai 3
    bai3();

    return 0;
}
