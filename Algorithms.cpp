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

// Find longest strictly increasing subsequence in O(nlogn)
// reference: find_lis on website Algorithmist
// A more straightforward method is based on LCS. That is, given any string A, find LCS of A and its sorted version B. Complexity O(n^2)
// Idea: dynamic programming
//       let X[k] (k = 0, ..., n-1) denote the sequence of size n, while the algorithm iterates the sequence from left to right,
//       it maintains two vectors, vector M that contains the positions of the elements that are potentially in the LIS,
//       	and vector P (of the same size as the sequence) that stores at P[k] the position of the predecessor of X[k] in the LIS ending at X[k]
// e.g., X = {1, 9, 3, 8, 11, 4, 5, 6, 4, 19, 7, 17}
//		 (1) Put the first two elements into M and update P: M = {0, 1}, P = {0, 0}
//           (particularly, P[1] = 0 means that in the LIS ending at X[1] = 9, the predecessor of X[1] = 9 is at position 0 (i.e., X[0] = 1))
//       (2) Since the next element X[2] = 3 < X[M[1]] = 9, we need to insert it into P at the appropriate position so that elements in M is LIS
//           Now M = {0, 2}, and P = {0, 0, 0}. (P[2] = 0 because the predecessor of X[2] = 3 is still X[0] = 1 in the LIS {1, 3} ending at 3)
//       (3) Trivially, after processing X[4] = 11, we have M = {0, 2, 3, 4}, P = {0, 0, 0, 2, 3}.
//       (4) Similar to (2), after processing X[5] = 4, we have M = {0, 2, 5, 4}, P = {0, 0, 0, 2, 3, 2}
//           Note that if the sequence ended here, then M = {0, 2, 5, 4} (corresponds to elements {1, 3, 4, 11})
//           would not be the LIS of X (X[5] = 4 should be after X[4] = 11)
//           But 4 will not occur in the final LIS, because we construct the LIS by reading backward on P.
//           We start from the last element pointed by M[3], which is X[4]; then, based on P[4] = 3,
//           we know that the predecessor of X[4] = 11 in the final LIS should be X[3]; similarly, we find X[2]= 3 and X[0] = 1, and thus LIS = 1,3,8,11
//       (5) After processing X[7] = 6, we have M = {0, 2, 5, 6, 7}, and P = {0, 0, 0, 2, 3, 2, 5, 6}.
//           Note that 5 and 6 override the previous stored element 11, because now we have a longer LIS, and it is safe to ignore the previously longest
//       (6) the rest executions are similar
void findLIS (vector<int>& seq, vector<int>& lis) {
	// lis temporarily serves as vector M
	// P array stores the position of the predecessor of X[k] in the longest increasing subsequence ending at X[k]
	if (seq.empty()) return;
	vector<int> P(seq.size());
	int left, right;
	lis.push_back(0);
	for (size_t i=1; i<seq.size(); ++i) {
		// If next element seq[i] is greater than last element of current longest subsequence seq[b.back()],
		// 	just push it at back of lis and continue
		if (seq[lis.back()] < seq[i]) { // if the next element is larger
			P[i] = lis.back(); // record the predecessor of seq[i]
			lis.push_back(i);  // store the larger element into M
			continue;
		}
		// Otherwise, store seq[i] into M at the appropriate position: the smallest element referenced by M which is just bigger than seq[i]
        // Binary search to find the smallest element referenced by M which is just bigger than a[i]
         // Note : Binary search is performed on M (and not seq). Size of b is always <=n and hence contributes O(log n) to complexity.
		left = 0;
		right = lis.size()-1;
		while (left < right) {
			int mid = (left + right) / 2;
			if (seq[lis[mid]] < seq[i])
				left = mid+1;
			else
				right = mid;
		}
		// Update b if new value is smaller then previously referenced value
		if (seq[i] < seq[lis[left]]) {
			if (left > 0)
				P[i] = lis[left-1];
			lis[left] = i;
		}
	}
	// backtrack P and put the true LIS into lis
	int n = lis.size();
	int current = lis.back();
	while (n) {
		lis[n-1] = current;
		current = P[current];
		--n;
	}
}


void testLIS() {
	int a[] = { 1, 9, 3, 8, 11, 4};
	vector<int> seq(a, a+sizeof(a)/sizeof(a[0])); // seq : Input Vector
	vector<int> lis;                              // lis : Vector containing indexes of longest subsequence
        findLIS(seq, lis);

        //Printing actual output
	for (size_t i = 0; i < lis.size(); i++)
		printf("%d ", seq[lis[i]]);
        printf("\n");
}

int main() {
	// testLCS();
	testLIS();
	return 0;
}
