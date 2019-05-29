#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct variable {
	int id;
	bool isNot;
	variable(int id) {
		this->id = id;
		isNot = false;
	}
	void negation() {
		this->isNot = !isNot;
	}
};

void printData(vector<vector<variable*>> imps, vector<bool> variables, vector<bool>negations) {
	cout << "Oryginalne wartosci zmiennych: "<<endl;
	for (int i = 0; i < variables.size(); i++) {
		cout << "x" << i << " = " << variables[i] << endl;
	}
	cout << "W artosci zmiennych po przeprowadzeniu algorytmu GSAT: " << endl;
	for (int i = 0; i < variables.size(); i++) {
		if (negations[i])
			cout << "~x" << i << " = " << !variables[i] << endl;
		else
			cout << "x" << i << " = " << variables[i] << endl;
	}
	for (int i = 0; i < imps.size(); i++) {
		cout << "( ";
		for (int j = 0; j < imps[i].size(); j++) {
			if (imps[i][j]->isNot)
				cout << "~x";
			else
				cout << "x";
			cout << imps[i][j]->id;
			if (j != imps[i].size() - 1)
				cout << " \\/ ";
		}
		cout << " )";
		if (i != imps.size() - 1)
			cout << " /\\ ";
	}
	cout << endl;
}
void generateImps(vector<vector<variable*>>& imps, vector<bool> &variables, vector<bool> &negations) {
	int imNo, n, k, tempInt;
	vector<variable*>temp;
	do {
		cout << "Liczba implicentow: ";
		cin >> imNo;
	} while (imNo <= 0);
	do {
		cout << "Liczba zmiennych: ";
		cin >> n;
	} while (n <= 0);
	for (int i = 0; i < n; i++) {
		variables.push_back(rand() % 2);
		negations.push_back(false);
	}
	for (int i = 0; i < imNo; i++) {
		do {
			cout << "Rozmiar implicentu " << i + 1 << ": ";
			cin >> k;
		} while (k <= 0);
		for (int j = 0; j < k; j++) {
			temp.push_back(new variable(rand() % n));
			tempInt = rand() % 2;
			if (tempInt == 1)
				temp[j]->negation();
		}
		imps.push_back(temp);
		temp.clear();
	}
}
vector<int> getFalseImpsIndex(vector<vector<variable*>> imps, vector<bool> variables, vector<bool> negations) {
	bool variable;
	bool flag;
	vector<int> falseImpsIndex;
	for (int i = 0; i < imps.size(); i++) {
		flag = false;
		for (int j = 0; j < imps[i].size(); j++) {
			variable = variables[imps[i][j]->id];
			if (negations[imps[i][j]->id])
				variable = !variable;
			if (imps[i][j]->isNot)
				variable = !variable;
			if (variable) {
				flag = true;
				break;
			}
		}
		if (!flag) {
			falseImpsIndex.push_back(i);
		}
			
	}
	return falseImpsIndex;
}
bool isSatisfied(vector<vector<variable*>> imps, vector<bool> variables, vector<bool>negations) {
	bool variable;
	bool flag;
	for (int i = 0; i < imps.size(); i++) {
		flag = false;
		for (int j = 0; j < imps[i].size(); j++) {
			variable = variables[imps[i][j]->id];
			if (negations[imps[i][j]->id])
				variable = !variable;
			if (imps[i][j]->isNot)
				variable = !variable;
			if (variable) {
				flag = true;
				break;
			}
		}
		if (!flag)
			return false;
	}
	return true;
}
void GSat() {
	srand(time(NULL));
	long iterations = 0;
	vector<vector<variable*>>imps;
	vector<variable*> variablesInFalseImp;
	vector<bool>variables;
	vector<bool> negations;
	vector<int> falseImpsIndex;
	int temp;
	generateImps(imps, variables, negations);
	while (!isSatisfied(imps, variables, negations) && iterations <1000) {
		falseImpsIndex = getFalseImpsIndex(imps, variables, negations);
		temp = falseImpsIndex[rand() % falseImpsIndex.size()];
		variablesInFalseImp = imps[temp];
		temp = variablesInFalseImp[rand() % variablesInFalseImp.size()]->id;
		negations[temp] = !negations[temp];
		iterations++;
	}
	printData(imps, variables, negations);
	cout << "Liczba iteracji: " << iterations << endl;
}
int main() {
	GSat();
	return 0;
}