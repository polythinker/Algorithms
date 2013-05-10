//============================================================================
// Name        : Algorithms.cpp
// Author      : Xi Yun
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>

using namespace std;

// Find the longest common subsequence of two sequences
// 	A subsequence is not necessarily consecutive
// Method: dynamic programming
// Let X and Y be two sequences, and let X_i and Y_j be the prefixes of X and Y, then
//						NULL										if i == 0 or j == 0
// LCS(X_i, Y_j) = { LCS(X_{i-1}, Y_{j-1} + 1)							if X_i == Y_j
//					 longest{LCS(X_i, Y_{j-1}), LCS(X_{i-1}, Y_j)}  if X_i =/= Y_j
// Reference: http://en.wikipedia.org/wiki/Longest_common_subsequence_problem
// Here we only use a two-row matrix to record the LCS for the current row and the previous row
// Note LCS's applications for Shortest Common Supersequence (SCS) and edit distance
int LCS(string X, string Y) {
	// Swap X and Y so that the shorter string is used for the columns
	if (Y.length() > X.length())
		swap(X, Y);
	int Xsize = X.length();
	int Ysize = Y.length();
	// define the assistant matrix that records the LCS for the current row and the previous row
	// the boundary elements are zeros thanks to the initialization
	vector<vector<int> > LCSMatrix(2, vector<int>(Ysize+1, 0));
	for (int i=1; i<=Xsize; ++i) {
		for (int j=1; j<=Ysize; ++j) {
			if (X[i-1] == Y[j-1])
				LCSMatrix[1][j] = LCSMatrix[0][j-1] + 1;
			else
				LCSMatrix[1][j] = max(LCSMatrix[1][j-1], LCSMatrix[0][j]);
		}
		// update the LCS values by swapping
		swap(LCSMatrix[0], LCSMatrix[1]);
	}
	// return the final LCS value
	return LCSMatrix[0][Ysize];
}

// Read out an LCS
string readOutAnLCS(vector<vector<int> > CMatrix, string X, string Y, int i, int j) {
	if (i == 0 || j == 0) return "";
	else if (X[i-1] == Y[j-1]) return readOutAnLCS(CMatrix, X, Y, i-1, j-1) + X[i-1];
	else {
		if (CMatrix[i-1][j] < CMatrix[i][j-1])
			return readOutAnLCS(CMatrix, X, Y, i, j-1);
		else
			return readOutAnLCS(CMatrix, X, Y, i-1, j);
	}
}

// Read out all LCS
vector<string> readOutAllLCSs (vector<vector<int> > CMatrix, string X, string Y, int i, int j) {
	vector<string> ret;
	if (i == 0 || j == 0) {
		ret.push_back("");
	} else if (X[i-1] == Y[j-1]){
		ret = readOutAllLCSs(CMatrix, X, Y, i-1, j-1);
		for (vector<string>::iterator it=ret.begin(); it!=ret.end(); ++it)
			*it = *it + X[i-1];
	} else {
		if (CMatrix[i-1][j] <= CMatrix[i][j-1])
			ret = readOutAllLCSs(CMatrix, X, Y, i, j-1);
		if (CMatrix[i-1][j] >= CMatrix[i][j-1]) {
			vector<string> vStr = readOutAllLCSs(CMatrix, X, Y, i-1, j);
			ret.insert(ret.end(), vStr.begin(), vStr.end());
		}
	}
	return ret;
}

// Find the longest common subsequence of two sequences and also print the subsequences
string LCSFindOne(string X, string Y) {
	int Xsize = X.length();
	int Ysize = Y.length();
	// define the assistant matrix that records the LCS for the current row and the previous row
	// the boundary elements are zeros thanks to the initialization
	vector<vector<int> > LCSMatrix(Xsize+1, vector<int>(Ysize+1, 0));
	for (int i=1; i<=Xsize; ++i) {
		for (int j=1; j<=Ysize; ++j) {
			if (X[i-1] == Y[j-1])
				LCSMatrix[i][j] = LCSMatrix[i-1][j-1] + 1;
			else
				LCSMatrix[i][j] = max(LCSMatrix[i][j-1], LCSMatrix[i-1][j]);
		}
	}
	return readOutAnLCS(LCSMatrix, X, Y, Xsize, Ysize);
}

// Find the longest common subsequence of two sequences and also print the subsequences
vector<string> LCSFindAll(string X, string Y) {
	int Xsize = X.length();
	int Ysize = Y.length();
	// define the assistant matrix that records the LCS for the current row and the previous row
	// the boundary elements are zeros thanks to the initialization
	vector<vector<int> > LCSMatrix(Xsize+1, vector<int>(Ysize+1, 0));
	for (int i=1; i<=Xsize; ++i) {
		for (int j=1; j<=Ysize; ++j) {
			if (X[i-1] == Y[j-1])
				LCSMatrix[i][j] = LCSMatrix[i-1][j-1] + 1;
			else
				LCSMatrix[i][j] = max(LCSMatrix[i][j-1], LCSMatrix[i-1][j]);
		}
	}
	return readOutAllLCSs(LCSMatrix, X, Y, Xsize, Ysize);
}


// test LCS
void testLCS() {
	string X = "abcdefg";
	string Y = "bcdgk";
	cout << "The length of LCS of " << X << " and " << Y << " is " << LCS(X, Y) << endl;
	cout << "The LCS of " << X << " and " << Y << " is " << LCSFindOne(X, Y) << endl;

	// string M = "XMJYAUZ";
	// string N = "MZJAWXU";
	string M = "AGCAT";
	string N = "GAC";
	vector<string> LCSs = LCSFindAll(M, N);
	cout << "The LCS of " << M << " and " << N << " are " << endl;
	for (vector<string>::iterator it=LCSs.begin(); it!=LCSs.end(); ++it)
		cout << *it << endl;

}


int main() {
	testLCS();
	return 0;
}
