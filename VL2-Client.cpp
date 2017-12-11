#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
/* #include <bits/stdc++.h> */

using namespace std;

#define INP1 		"inp1.txt"
#define INP2 		"inp2.txt"
#define INP3 		"inp3.txt"
#define INP4 		"inp4.txt"
#define INP5 		"inp5.txt"
#define INP6		"inp6.txt"
#define INP7 		"inp7.txt"

#define LOG			"log.txt"
#define CONTEST		"contestant.txt"
#define RECORD		"record.txt"

#define BND			BAN NOI DUNG

const int X_max		= 	60;		// 	number of contestants
const int Y_max		= 	7;		// 	number of rounds
/* const int Z_max		= 	10;		// 	number of questions in (Y-1)st rounds */
/* const int Z_final 	= 	12;		// 	number of questions in the final round */ 
const int K_1200 	=	40;
const int K_1400	= 	30;
const int K_1600	= 	20;
const int K_1800	= 	10;

struct contestant {
	int id;
	string name;
	long double ratings;
	vector < vector <int> > results;
};

vector < contestant > board;
vector < vector < vector <bool> > > isMedian;
vector < vector < vector <long double> > > change;

int qst_MAX(int block);
string cur();
void setup();
void input_update(int block, int question);
void input(int block);
long double expect(long double rating_A, long double rating_B);
int K_const(long double ratings);
bool ratingsRank(contestant A, contestant B);
bool idRank(contestant A, contestant B);
long double median(vector <contestant> scores, int size, int type, int block, int question);
void rating_update(int block, int question);
void execute(int block);
void output(int block);
void _rank();
void showlog();

string command;
string fname[] = {"inp1.txt", "inp2.txt", "inp3.txt", "inp4.txt", "inp5.txt", "inp6.txt", "inp7.txt"};
ofstream fLOG;
ifstream fCONTEST, fSHOW /*, fINP1, fINP2, fINP3, fINP4, fINP5, fINP6, fINP7 */;
// New instances
ifstream fINP[7];
int Z[7] = {10, 10, 10, 10, 10, 10, 12};

int main() {
	fLOG.open(LOG);
	if (fLOG.is_open())	{
		cout << "The logfile is OK." << endl;
	}
	else {
		cout << "Please verify the logfile." << endl;
	}

	while ((cout << ">> ") && (cin >> command)) {
		if (command == "SETUP()") {
			cout << "Trying to set up." << endl;
			fLOG << cur() << "\t" << "Trying to set up." << endl;
			setup();
			cout << "Setting up completed." << endl;
			fLOG << cur() << "\t" << "Setting up completed." << endl;
		}	else if (command.at(0) == 'I' &&	//	INP(y), 1 <= y <= 6
		             command.at(1) == 'N' &&
		             command.at(2) == 'P' &&
		             command.at(3) == '(' &&
		             command.at(5) == ')' ) {
			int blck = (int)(unsigned char)(command.at(4)) - 48;
			fLOG << cur() << "\t" << "Reading input from block " << blck << endl;
			cout << "Reading input from block " << blck << endl;
			input(blck-1);
			fLOG << cur() << "\t" << "Reading completed." << endl;
			cout << "Reading completed." << endl;
		}	else if (command.at(0) == 'E' &&	//	EXE(y), 1 <= y <= 6
		             command.at(1) == 'X' &&
		             command.at(2) == 'E' &&
		             command.at(3) == '(' &&
		             command.at(5) == ')' ) {
			int blck = (int)(unsigned char)(command.at(4)) - 48;
			fLOG << cur() << "\t" << "Executing block " << blck << endl;
			cout << "Executing block " << blck << endl;
			execute(blck-1);
			fLOG << cur() << "\t" << "Executing completed." << endl;
			cout << "Executing completed." << endl;
		}	else if (command.at(0) == 'O' &&	//	OUT(y), 1 <= y <= 6
		             command.at(1) == 'U' &&
		             command.at(2) == 'T' &&
		             command.at(3) == '(' &&
		             command.at(5) == ')' ) {
			int blck = (int)(unsigned char)(command.at(4)) - 48;
			fLOG << cur() << "\t" << "Showing results of block " << blck << endl;
			cout << "Showing results of block " << blck << endl;
			output(blck-1);
			fLOG << cur() << "\t" << "The result of block " << blck << " is shown." << endl;
			cout << "The result of block " << blck << " is shown." << endl;
		}	else if (command.at(0) == 'E' &&
		             command.at(1) == 'X' &&
		             command.at(2) == 'I' &&
		             command.at(3) == 'T' &&
		             command.at(4) == '(' &&
		             command.at(5) == ')' ) {
			break;
		}	else if (command == "RANK()") {	//	RANK()
			_rank();
		}	else if (command == "SHOWLOG()") {
			cout << "Reading logfile." << endl;
			showlog();
			cout << "Reading completed." << endl;
		}	else {
			cout << "Invalid command. Please try again." << endl;
		}
	}

	return 0;
}

string cur() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}

int qst_MAX(int block) {
    return Z[block];
}

void setup() {
	fCONTEST.open(CONTEST);
	board.resize(X_max);
	change.resize(X_max);
	isMedian.resize(X_max);

	for (int x = 0; x < X_max; x++) {
		fCONTEST >> board[x].id;
		fCONTEST >> board[x].name;
		board[x].ratings = 1200;

		board[x].results.resize(Y_max);
		change[x].resize(Y_max);
		isMedian[x].resize(Y_max);

		for (int y = 0; y < Y_max; y++) {
                        board[x].results[y].resize(Z[y]);
                        change[x][y].resize(Z[y]);
                        isMedian[x][y].resize(Z[y]);
		}
	}

	for (int x = 0; x < X_max; x++) {
		for (int y = 0; y < Y_max; y++) {
			for (int z = 0; z < qst_MAX(y); z++) {
				isMedian[x][y][z] = false;
			}
		}
	}

	return;
}

void input(int block) {
        fINP[block].open(fname[block]);
        if (fINP[block].is_open()) {
                cout << "The input file for block " << block+1 << " is OK." << endl;
                fLOG << cur() << "\t" << "The input file for block " << block+1 << " is OK." << endl;
                for (int x = 0; x < X_max; x++) {
                        int temp;
                        fINP[block] >> temp;
                        for (int z = 0; z < qst_MAX(block); z++) {
                                fINP[block] >> board[temp - 1].results[block - (block == 7-1)][z];
                        }
                }
        }	else {
                cout << "Please verify the input file for block " << block+1 << '.' << endl;
                fLOG << cur() << "\t" << "Please verify the input file for block " << block+1 << '.' << endl;
        }
	return;
}

long double expect(long double rating_A, long double rating_B) {
	long double expect_A;
	long double Q_A =  powf(10, rating_A / 400);
	long double Q_B =  powf(10, rating_B / 400);
	expect_A = Q_A / (Q_A + Q_B);
	return expect_A;
}

int K_const(long double ratings) {
	if (ratings < 1400) {
		return K_1200;
	}	else if (ratings < 1600) {
		return K_1400;
	}	else if (ratings < 1800) {
		return K_1600;
	}	else {
		return K_1800;
	}
}

bool ratingsRank(contestant A, contestant B) {
	return (A.ratings > B.ratings);
}

bool idRank(contestant A, contestant B) {
	return (A.id < B.id);
}

long double median(vector <contestant> scores, int size, int type, int block, int question) {
	if (size == 1) {
		isMedian[scores[0].id - 1][block][question] = true;
		return scores[0].ratings;
	}

	long double med;

	sort(scores.begin(), scores.begin() + size - 1, ratingsRank);
	if (size % 2 == 1) {
		isMedian[scores[(size - 1) / 2].id - 1][block][question] = true;
		med = scores[(size - 1) / 2].ratings;
		if (type == 1) {
			fLOG << cur() << "\t" << "The winner median is " << med << ", "
			     << scores[(size - 1) / 2].name << "." << endl;
		}	else {
			fLOG << cur() << "\t" << "The loser median is " << med
			     << ", " << scores[(size - 1) / 2].name << "." << endl;
		}
	}	else if (size % 2 == 0) {
		isMedian[scores[size / 2].id - 1][block][question] = true;
		isMedian[scores[size / 2 - 1].id - 1][block][question] = true;
		med = (scores[size / 2].ratings + scores[size / 2 - 1].ratings) / 2;
		if (type == 1) {
			fLOG << cur() << "\t" << "The winner median is " << med << ", "
			     << scores[size / 2].name << " and " << scores[size / 2 - 1].name << "." << endl;
		}
		else {
			fLOG << cur() << "\t" << "The loser median is " << med << ", "
			     << scores[size / 2].name << " and " << scores[size / 2 - 1].name << "." << endl;
		}
	}

	return med;
}

void rating_update(int block, int question) {
	int cnt_w = 0, cnt_l = 0;
	long double winner, loser;
	vector <contestant> win;
	vector <contestant> lose;

	for (int i = 0; i < X_max; i++) {
		if (board[i].results[block][question] == 1) {
			win.push_back(board[i]);
			cnt_w += 1;
		}	else {
			lose.push_back(board[i]);
			cnt_l += 1;
		}
	}

	if (cnt_w != X_max && cnt_l != X_max) {
		winner 	= 	median(win, cnt_w, 1, block, question);
		loser 	= 	median(lose, cnt_l, -1, block, question);
		long double E_w = expect(winner, loser);
		long double E_l = 1 - E_w;
		fLOG << cur() << "\t" << "E_w = " << E_w << ", E_l = " << E_l << endl;
		for (int x = 0; x < X_max; x++) {
			if (board[x].results[block][question] == 1) {
				long double point = K_const(board[x].ratings) * E_l;
				board[x].ratings += point;
				change[x][block][question] = point;
			}	else if (board[x].results[block][question] == -1) {
				long double point = K_const(board[x].ratings) * E_l;
				board[x].ratings -= point;
				change[x][block][question] = -point;
			}	else {
				change[x][block][question] = 0;
			}
		}
	}	else {
		fLOG << cur() << "\t" << "The ratings do not change." << endl;
	}
	
	return;
}

void execute(int block) {
	for (int question = 0; question < qst_MAX(block); question++) {
		rating_update(block, question);
	}
	return;
}

void output(int block) {
	fLOG << cur() << "\t" << "Records for block " << block << endl;
	for (int x = 0; x < X_max; x++) {
		fLOG 	<< board[x].name << "\t\t";
		cout 	<< board[x].name << "\t\t";
		for (int z = 0; z < qst_MAX(block); z++) {
			if (isMedian[x][block][z] == true) {
				fLOG 	<< "(";
				cout 	<< "(";
			}
			fLOG << (int) change[x][block][z];
			cout << (int) change[x][block][z];
			if (isMedian[x][block][z] == true) {
				fLOG 	<< ")";
				cout 	<< ")";
			}
			fLOG 	<< "\t";
			cout 	<< "\t";
		}
		fLOG << board[x].ratings << endl;
		cout << board[x].ratings << endl;
	}

	fLOG << cur() << "\t" << "Recording completed." << endl;
	return;
}

void _rank() {
	sort(board.begin(), board.begin() + X_max, ratingsRank);

	for (int x = 0, i = 1; x < X_max; x++) {
		if (x >= 1 && board[x].ratings != board[x - 1].ratings) {
			cout << x + 1 << "\t";
			i = x + 1;
		}	else {
			cout << i << "\t";
		}
		cout << board[x].name << "\t" << board[x].ratings << endl;
	}

	sort(board.begin(), board.begin() + X_max, idRank);
	return;
}

void showlog() {
	fSHOW.open(LOG);
	string buffer;

	cout << endl;
	while (getline(fSHOW, buffer)) {
		cout << buffer << endl;
	}

	cout << endl;
	return;
}
