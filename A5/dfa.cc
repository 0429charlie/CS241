#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main() {
	vector<string> alph;
	vector<string> states;
	vector<string> accepts;
	vector<string> trans;
	int noalph = 0;
	cin >> noalph;
	while (noalph != 0) {
		string a;
		cin >> a;
		alph.push_back(a);
		noalph--;
	}
	int nost = 0;
	cin >> nost;
	while (nost != 0) {
		string s;
		cin >> s;
		states.push_back(s);
		nost--;
	}
	string startings;
	cin >> startings;
	int noaccep = 0;
	cin >> noaccep;
	while (noaccep != 0) {
		string accep;
		cin >> accep;
		accepts.push_back(accep);
		noaccep--;
	}
	int notrans = 0;
	cin >> notrans;
	while (notrans != 0) {
		string trans1;
		string trans2;
		string trans3;
		cin >> trans1;
		cin >> trans2;
		cin >> trans3;
		trans.push_back(trans1);
		trans.push_back(trans2);
		trans.push_back(trans3);
		notrans--;
	}
	
	vector<string> result;
	
	string cur = startings;
	string s;
	cin >> s;
	while (cin) {
		for (int i = 0; i < trans.size(); i = i+3) {
			if (cur == trans[i]) {
				if (s == trans[i+1]) {
					cur = trans[i+2];
				}
			}
		}
		int p = 0;
		for (int j = 0; j < accepts.size(); j++) {
			if (cur == accepts[j]) {
				p = 1;
			}		
		}
		if (p == 1) {
			result.push_back("true");
		} else {
			result.push_back("false");
		}
		cin >> s;
		//if (s[s.size()-1] != ' ') {
		//	cur = startings;
		//}
	}
	for (int k = 0; k < result.size(); k++) {
		cout << result[k] << endl;
	}
	return 1;		
}





