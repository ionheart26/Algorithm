#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int dnc(const int temp[], const int left, const int right, int& start, int& finish) {
    int maxLeftSum, maxRightSum;
    int maxLBS = 0, maxRBS = 0;
    int leftBorderSum = 0, rightBorderSum = 0;

    if (left == right) {
        start = left; finish = right;
        return temp[left];
    }

    int middle = (left + right) / 2;
    int start1, finish1;
    maxLeftSum = dnc(temp, left, middle, start1, finish1);
    int start2, finish2;
    maxRightSum = dnc(temp, middle + 1, right, start2, finish2);

    int start3 = 0, finish3 = 0;
    for (int i = middle; i >= left; i--) {
        leftBorderSum += temp[i];
        if (leftBorderSum > maxLBS) {
            maxLBS = leftBorderSum;
            start3 = i;
        }
    }
    for (int i = middle + 1; i <= right; i++) {
        rightBorderSum += temp[i];
        if (rightBorderSum > maxRBS) {
            maxRBS = rightBorderSum;
            finish3 = i;
        }
    }

    int max = INT_MIN;
    if (max < maxLeftSum) {
        max = maxLeftSum;
        start = start1; finish = finish1;
    }
    if (max < maxRightSum) {
        max = maxRightSum;
        start = start2; finish = finish2;
    }
    int maxMiddleSum = maxLBS + maxRBS;
    if (maxMiddleSum != 0 && max < maxMiddleSum) {
        max = maxMiddleSum;
        start = start3; finish = finish3;
    }
    return max;
}

int kadane(const int temp[], int& start, int& finish, const int n) {
    int temp_sum = 0, maxSum = INT_MIN;
    finish = -1;

    int temp_start = 0;
    for (int i = 0; i < n; i++) {
        temp_sum += temp[i];
        if (temp_sum < 0) {
            temp_sum = 0;
            temp_start = i + 1;
        }
        else if (temp_sum > maxSum) {
            maxSum = temp_sum;
            start = temp_start;
            finish = i;
        }
    }
    if (finish != -1) return maxSum;

    maxSum = temp[0];
    start = finish = 0;
    for (int i = 1; i < n; i++) {
        if (temp[i] > maxSum) {
            maxSum = temp[i];
            start = finish = i;
        }
    }
    return maxSum;
}

void a1(const string& name1, const string& name2) {
    ifstream inFile(name1, ios::in | ios::binary);
    int n, target;
    inFile.read((char*)(&n), sizeof(int));
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        inFile.read((char*)(&target), sizeof(int));
        arr[i] = target;
    }
    inFile.close();

    int start, finish;
    int maxS = dnc(arr, 0, n - 1, start, finish);

    ofstream outFile(name2, ios::out | ios::binary);
    outFile.write((char*)(&maxS), sizeof(int));
    outFile.write((char*)(&start), sizeof(int));
    outFile.write((char*)(&finish), sizeof(int));
    outFile.close();
}

void a2(const string& name1, const string& name2) {
    ifstream inFile(name1, ios::in | ios::binary);
    int n, target;
    inFile.read((char*)(&n), sizeof(int));
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        inFile.read((char*)(&target), sizeof(int));
        arr[i] = target;
    }
    inFile.close();

    int start, finish;
    int maxS = kadane(arr, start, finish, n);

    ofstream outFile(name2, ios::out | ios::binary);
    outFile.write((char*)(&maxS), sizeof(int));
    outFile.write((char*)(&start), sizeof(int));
    outFile.write((char*)(&finish), sizeof(int));
    outFile.close();
}

/***********************************************************************************************
O(n^4)
Summed Area Table
SAT�� 2���� �迭�μ� SAT[i][j]�� 0�� 0������ i�� j�������� ��� ������ ���� �����ϰ� �ֽ��ϴ�.
SAT�� �̿��ϸ� ��� subrectangle�� ���� ����ð��� ���� �� �ֽ��ϴ�.
***********************************************************************************************/
void a3(const string& name1, const string& name2) {
    ifstream inFile(name1, ios::in | ios::binary);
    int n, target;
    inFile.read((char*)(&n), sizeof(int));
    int** arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            inFile.read((char*)(&target), sizeof(int));
            arr[i][j] = target;
        }
    inFile.close();

    int** sat = new int* [n];
    for (int i = 0; i < n; i++) {
        sat[i] = new int[n];
        memset(sat[i], 0, sizeof(int) * n);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 && j == 0) sat[0][0] = arr[0][0];
            else if (i == 0) sat[0][j] = sat[0][j - 1] + arr[0][j];
            else if (j == 0) sat[i][0] = sat[i - 1][0] + arr[i][0];
            else sat[i][j] = sat[i - 1][j] + sat[i][j - 1] - sat[i - 1][j - 1] + arr[i][j];
        }
    }

    int maxS = INT_MIN, maxL, maxR, maxT, maxB;
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            for (int t = 0; t < n; t++) {
                for (int b = t; b < n; b++) {
                    int tempSum = sat[b][r];
                    if (l > 0) tempSum -= sat[b][l - 1];
                    if (t > 0) tempSum -= sat[t - 1][r];
                    if (l > 0 && t > 0) tempSum += sat[t - 1][l - 1];
                    if (tempSum > maxS) {
                        maxS = tempSum;
                        maxL = l;
                        maxR = r;
                        maxT = t;
                        maxB = b;
                    }
                }
            }
        }
    }

    ofstream outFile(name2, ios::out | ios::binary);
    outFile.write((char*)(&maxS), sizeof(int));
    outFile.write((char*)(&maxT), sizeof(int));
    outFile.write((char*)(&maxL), sizeof(int));
    outFile.write((char*)(&maxB), sizeof(int));
    outFile.write((char*)(&maxR), sizeof(int));
    outFile.close();
}

/***********************************************************************************************
O(n^3 * logn)
Divide and Conquer
��� left�� right�� ���Ͽ� ��� ���� left���� right������ ���� 1���� �迭 temp�� �����մϴ�.
�׸��� temp�� 1���� Maximum Subsequence Sum �˰����� �����մϴ�.

a4�� ��� left�� right�ֿ� ���Ͽ� temp�� ������ ��,
Maximum Subsequence Sum�� �����ϴ� �Լ� dnc�� ȣ���մϴ�.
***********************************************************************************************/
void a4(const string& name1, const string& name2) {
    ifstream inFile(name1, ios::in | ios::binary);
    int n, target;
    inFile.read((char*)(&n), sizeof(int));
    int** arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            inFile.read((char*)(&target), sizeof(int));
            arr[i][j] = target;
        }
    inFile.close();

    int maxS = INT_MIN, maxL, maxR, maxT, maxB;
    int* temp = new int[n];

    for (int l = 0; l < n; l++) {
        memset(temp, 0, sizeof(int) * n);
        for (int r = l; r < n; r++) {
            for (int i = 0; i < n; i++)
                temp[i] += arr[i][r];
            int start, finish;
            int sum = dnc(temp, 0, n - 1, start, finish);

            if (sum > maxS) {
                maxS = sum;
                maxL = l; maxR = r;
                maxT = start; maxB = finish;
            }
        }
    }

    ofstream outFile(name2, ios::out | ios::binary);
    outFile.write((char*)(&maxS), sizeof(int));
    outFile.write((char*)(&maxT), sizeof(int));
    outFile.write((char*)(&maxL), sizeof(int));
    outFile.write((char*)(&maxB), sizeof(int));
    outFile.write((char*)(&maxR), sizeof(int));
    outFile.close();
}

/***********************************************************************************************
O(n^3)
Kadane's algorithm
Dynamic Programming�� �̿��մϴ�.
��� left�� right�� ���Ͽ� ��� ���� left���� right������ ���� 1���� �迭 temp�� �����մϴ�.
�׸��� temp�� 1���� Maximum Subsequence Sum �˰����� �����մϴ�.

a5�� ��� left�� right�ֿ� ���Ͽ� temp�� ������ ��,
Maximum Subsequence Sum�� �����ϴ� �Լ� kadane�� ȣ���մϴ�.
***********************************************************************************************/
void a5(const string& name1, const string& name2) {
    ifstream inFile(name1, ios::in | ios::binary);
    int n, target;
    inFile.read((char*)(&n), sizeof(int));
    int** arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            inFile.read((char*)(&target), sizeof(int));
            arr[i][j] = target;
        }
    inFile.close();

    int maxS = INT_MIN, maxL, maxR, maxT, maxB;
    int* temp = new int[n];

    for (int l = 0; l < n; l++) {
        memset(temp, 0, sizeof(int) * n);
        for (int r = l; r < n; r++) {
            for (int i = 0; i < n; i++)
                temp[i] += arr[i][r];
            int start, finish;
            int sum = kadane(temp, start, finish, n);

            if (sum > maxS) {
                maxS = sum;
                maxL = l; maxR = r;
                maxT = start; maxB = finish;
            }
        }
    }

    ofstream outFile(name2, ios::out | ios::binary);
    outFile.write((char*)(&maxS), sizeof(int));
    outFile.write((char*)(&maxT), sizeof(int));
    outFile.write((char*)(&maxL), sizeof(int));
    outFile.write((char*)(&maxB), sizeof(int));
    outFile.write((char*)(&maxR), sizeof(int));
    outFile.close();
}

int main() {
    ifstream inFile("HW1_config.txt");
    if (inFile.is_open()) {
        int t;
        inFile >> t;
        while (t--) {
            int n;
            string name1, name2;
            inFile >> n >> name1 >> name2;
            switch (n) {
            case 1:
                a1(name1, name2); break;
            case 2:
                a2(name1, name2); break;
            case 3:
                a3(name1, name2); break;
            case 4:
                a4(name1, name2); break;
            case 5:
                a5(name1, name2); break;
            }
        }
    }
    inFile.close();
    return 0;
}