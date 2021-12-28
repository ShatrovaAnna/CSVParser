#include <iostream>
#include "CSVparser.h"
#include "Tuple.h"


using namespace std;

int main() {

	ifstream file;

	file.open("in.csv", ios::in);

	if (!file.is_open())
	{
		cerr << "EXCEPTION: can't open file\n";
		return 0;
	}

	try {
		CSVparser<string, string, int> parse(file, 0, ',', '\n', '\"');

		for (auto it = parse.begin(); it != parse.end(); ++it) {
			cout << *it;
		}

	}
	catch (exception& exp) {
		cout << exp.what();
	}
	file.close();

	return 0;
}