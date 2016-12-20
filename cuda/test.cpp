#include <iostream>
using namespace std;
void test(int a[10]) {
	a[1] = 312;
}
int main1() {
	int v[10] = { 0 };
	test(v);
	cout << v[1]<<endl;
	return 0;
}