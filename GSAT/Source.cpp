#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

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

void printData(vector<vector<variable*>> imps, vector<bool> variables, vector<bool>negations, bool completed) {
	ofstream output("output.txt");
	cout << "Oryginalne wartosci zmiennych: "<<endl;
	for (int i = 0; i < variables.size(); i++) {
		cout << "x" << i << " = " << variables[i] << endl;
	}
	if (completed) {
		cout << "Wartosci zmiennych po przeprowadzeniu algorytmu WalkSAT: " << endl;
		for (int i = 0; i < variables.size(); i++) {
			if (negations[i]) {
				cout << "~x" << i << " = " << !variables[i] << endl;
				output << "~x" << i << " = " << !variables[i] << endl;
			}
			else {
				cout << "x" << i << " = " << variables[i] << endl;
				output << "~x" << i << " = " << !variables[i] << endl;
			}
		}
	}
	cout << "y = ";
	for (int i = 0; i < imps.size(); i++) {
		cout << "( ";
		output << "( ";
		for (int j = 0; j < imps[i].size(); j++) {
			if (imps[i][j]->isNot) {
				cout << "~x";
				output << "~x";
			}
			else {
				cout << "x";
				output << "x";
			}
			cout << imps[i][j]->id;
			output << imps[i][j]->id;
			if (j != imps[i].size() - 1) {
				cout << " + ";
				output << " + ";
			}
		}
		cout << " )";
		output << " ) ";
		if (i != imps.size() - 1) {
			cout << " * ";
			output << " * ";
		}
	}
	cout << endl;
}
void generateImps(vector<vector<variable*>>& imps, vector<bool> &variables, vector<bool> &negations) {
	int imNo, n, k=0, tempInt, kMin, kMax;
	bool flag;
	variable* tempVariable;
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
	do {
		cout << "Minimalny rozmiar implicentu: ";
		cin >> kMin;
	} while (kMin <= 0 || kMin > n);
	do {
		cout << "Maksymalny rozmiar implicentu: ";
		cin >> kMax;
	} while (kMax < kMin || kMax> n);
	for (int i = 0; i < imNo; i++) {
		if (kMax == kMin)
			k = kMax;
		else
			k = rand() % (kMax - kMin) + kMin;
		for (int j = 0; j < k; j++) {
			do {
				flag = false;
				tempVariable = new variable(rand() % n);
				for (int k = 0; k < temp.size(); k++) {
					if (temp[k]->id == tempVariable->id) {
						flag = true;
						break;
					}
				}
			} while (flag);
			temp.push_back(tempVariable);
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
void walkSat() {
	srand(time(NULL));
	long iterations = 0;
	vector<vector<variable*>>imps;
	vector<variable*> variablesInFalseImp;
	vector<bool>variables;
	vector<bool> negations;
	vector<int> falseImpsIndex;
	int temp;
	generateImps(imps, variables, negations);
	while (!isSatisfied(imps, variables, negations) && iterations <10000) {
		falseImpsIndex = getFalseImpsIndex(imps, variables, negations);
		temp = falseImpsIndex[rand() % falseImpsIndex.size()];
		variablesInFalseImp = imps[temp];
		temp = variablesInFalseImp[rand() % variablesInFalseImp.size()]->id;
		negations[temp] = !negations[temp];
		iterations++;
	}
	if (iterations < 10000) {
		printData(imps, variables, negations, 1);
		cout << "Liczba iteracji: " << iterations << endl;
	}
	else {
		printData(imps, variables, negations, 0);
		cout << "Brak rozwiazania, przekroczono " <<iterations<<" iteracji"<< endl;
	}
}
int main() {
	cout << "Program rozwiazujacy problem spelnialnosci SAT przy uzyciu algorytmu WalkSAT."<<endl;
	walkSat();
	return 0;
}