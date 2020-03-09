#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	string s1;
	string s2;
	int n = 0;
	vector<string> v;
	string s;
	cout << "the stage1 is:" << endl;
	cin >> s1;
	cout << "the stage2 is:" << endl;
	cin >> s2;
	cout << "the number of transition is:" << endl;	
	cin >> n;
	cout << "enter all the trnasition" << endl;
	for (int i = 0; i < n; i++) {
		cin >> s;
		v.push_back(s);
	}
	for (int j = 0; j < n; j++) {
		cout << s1 << " " << v[j] << " " << s2 << endl;
	}
	return 1;
}
