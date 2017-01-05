#include <iostream>
using namespace std;
void df(int* a) {
	*a = 99;
}
int main1() {
	int num;
	while (cin >> num) {
		int *a = new int[num];
		for (int i = 0; i < num; i++) {
			a[i] = i;
		}
		for (int i = 1; i < num; i++) {
			a[i] += a[i - 1];
		}
		for (int i = 0; i < num; i++) {
			cout << a[i] << endl;
		}
	}
	return 0;
} 
int main() {
	int k = 0;
	df(&k);
	cout << k << endl;
}