#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

size_t xSize, ySize = 0;
std::vector<int>::size_type maxXSets, maxYSets;

void setDimensions(std::ifstream * inFile) {
	using namespace std;

	inFile->ignore(numeric_limits<streamsize>::max(), '\n');
	xSize = inFile->gcount() - 1;
	ySize++;

	while (!inFile->eof()) {
		inFile->ignore(numeric_limits<streamsize>::max(), '\n');
		ySize++;
	}

	inFile->seekg(0, ios_base::beg);
}

char ** getNonoArray(std::ifstream * inFile) {
	char ** nonoArray = new char*[ySize];

	for (size_t col = 0; col < ySize; col++) {
		nonoArray[col] = new char[xSize + 1];
		inFile->getline(nonoArray[col], xSize + 1);
	}

	return nonoArray;
}

std::vector<int> * getRows(char ** nonogram) {
	using namespace std;

	vector<int> * rows = new std::vector<int>[ySize];

	for (size_t row = 0; row < ySize; row++) {
		char last = '*';
		rows[row].push_back(0);
		vector<int>::size_type sets = 1;

		for (size_t col = 0; col < xSize; col++) {
			if (nonogram[row][col] == '*') {
				if (last == '*' || rows[row].back() == 0)
					rows[row].back() += 1;
				else {
					rows[row].push_back(1);
					sets++;
				}
			}

			last = nonogram[row][col];
		}
		if (sets > maxXSets)
			maxXSets = sets;
	}

	return rows;
}

std::vector<int> * getCols(char ** nonogram) {
	using namespace std;

	vector<int> * cols = new std::vector<int>[xSize];

	for (size_t col = 0; col < xSize; col++) {
		char last = '*';
		cols[col].push_back(0);
		vector<int>::size_type sets = 1;

		for (size_t row = 0; row < ySize; row++) {
			if (nonogram[row][col] == '*') {
				if (last == '*' || cols[col].back() == 0)
					cols[col].back() += 1;
				else {
					cols[col].push_back(1);
					sets++;
				}
			}

			last = nonogram[row][col];
		}
		if (sets > maxYSets)
			maxYSets = sets;
	}

	return cols;
}

void outputCols(std::vector<int> * cols) {
	using namespace std;

	for (vector<int>::size_type set = 0; set < maxYSets; set++) {
		cout << string(3 * maxXSets, ' ');

		for (size_t col = 0; col < xSize; col++) {
			vector<int> * current = &cols[col];
			stringstream out;
			if (current->size() >= maxYSets - set) {
				out << current->front();
				current->erase(current->begin());
			}
			else
				out << ' ';
			printf("%3s", out.str().c_str());
		}
		cout << endl;
	}
}

void outputRows(std::vector<int> * rows) {
	using namespace std;

	for (vector<int>::size_type row = 0; row < ySize; row++) {
		cout << string(3 * (maxXSets - rows[row].size()), ' ');
		for (int current : rows[row]) {
			printf("%3d", current);
		}
		cout << endl;
	}
}

int main(int argc, char * argv[]) {
	using namespace std;

	ifstream inFile;
	inFile.open(argv[1], ios_base::in);

	setDimensions(&inFile);

	char ** nonoArray = getNonoArray(&inFile);

	vector<int> * cols = getCols(nonoArray);
	vector<int> * rows = getRows(nonoArray);

	for (size_t col = 0; col < ySize; col++) {
		delete[] nonoArray[col];
	}
	delete[] nonoArray;

	outputCols(cols);
	delete[] cols;

	outputRows(rows);
	delete[] rows;

	return 0;
}
