#include <iostream>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif


#include "utilities.h"

bool ReadFile(const char* fileName, std::string& output) {
	std::ifstream f(fileName);

	bool res = false;

	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			output.append(line);
			output.append("\n");
		}

		f.close();

		res = true;
	}
	else {
		fprintf(stderr, "file read error %s\n", fileName);
	}

	return res;

}