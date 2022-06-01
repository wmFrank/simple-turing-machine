#ifndef PCH_H
#define PCH_H

// TODO: 添加要在此处预编译的标头

#endif //PCH_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <string>
#include <cmath>
#include <sstream>
using namespace std;

struct Transfunc {
	string oldstate;
	vector<char> oldstr;
	vector<char> newstr;
	vector<char> dir;
	string newstate;
};

struct Turing {
	vector<string> stateSet;
	vector<char> inputSet;
	vector<char> tapeSet;
	string q0;
	char space;
	vector<string> finalSet;
	int tapenum;
	vector<Transfunc *> tf;
};

struct Tape {
	vector<int> index;
	vector<char> str;
	int head;
};

struct ID {
	int step;
	string state;
	vector<Tape *> tapes;
};

class TM {
public:
	Turing *turing;
	string path;
	vector<string> input;
	vector<string> output;
	ID *id;

public:
	TM(string tmpath) {
		turing = new Turing;
		path = tmpath;
	}

	void control() {
		readin();
		readfile();		
		for (int i = 0; i < input.size(); i++) {
			isValid(input[i]);
		}
	}

	void readfile() {
		ifstream in((path + "/input.txt").c_str(), ios::in);
		string line;
		while (getline(in, line)) {
			line = line.substr(0, line.length() - 1);
			input.push_back(line);
		}
		
		in.close();
	}

	void readin() {
		ifstream fin;
		fin.open((path + "/test.tm").c_str(), ios_base::in);
		string line = "";
		while (getline(fin, line)) {
			if(line.length() == 0)
				continue;
			if(line[0] == ';')
				continue;
			//cout << line << endl; 
			if(line[0] == '#'){
				char ch = line[1];
				if (ch == 'Q') {
					int left = line.find('{'), right = line.find('}');
					string temp = line.substr(left + 1, right - left - 1);
					string m = "";
					for (int t = 0; t < temp.length(); t++) {
						if (temp[t] != ',') {
							m = m + temp[t];
						}
						else {
							turing->stateSet.push_back(m);
							m = "";
						}
					}
					turing->stateSet.push_back(m);
				/*	for (int i = 0; i < turing->stateSet.size(); i++) {
						cout << turing->stateSet[i] << endl;
					}
					break;*/
				}
				else if (ch == 'S') {
					int left = line.find('{'), right = line.find('}');
					string temp = line.substr(left + 1, right - left - 1);
					for (int t = 0; t < temp.length(); t++) {
						if (temp[t] != ',') {
							turing->inputSet.push_back(temp[t]);
						}
					}
					/*for (int i = 0; i < turing->inputSet.size(); i++) {
						cout << turing->inputSet[i] << endl;
					}*/
					//break;
				}
				else if (ch == 'G') {
					int left = line.find('{'), right = line.find('}');
					string temp = line.substr(left + 1, right - left - 1);
					for (int t = 0; t < temp.length(); t++) {
						if (temp[t] != ',') {
							turing->tapeSet.push_back(temp[t]);
						}
					}
				/*	for (int i = 0; i < turing->tapeset.size(); i++) {
						cout << turing->tapeset[i] << endl;
					}
					break;*/
				}
				else if (ch == 'q') {
					turing->q0 = line.substr(6);
					//cout << turing->q0 << endl;
				}
				else if (ch == 'B') {
					turing->space = line[5];
					//cout << turing->space << endl;
				}
				else if (ch == 'F') {
					int left = line.find('{'), right = line.find('}');
					string temp = line.substr(left + 1, right - left - 1);
					string m = "";
					for (int t = 0; t < temp.length(); t++) {
						if (temp[t] != ',') {
							m = m + temp[t];
						}
						else {
							turing->finalSet.push_back(m);
							m = "";
						}
					}
					turing->finalSet.push_back(m);
					/*	for (int i = 0; i < turing->finalSet.size(); i++) {
							cout << turing->finalSet[i] << endl;
						}
						break;*/
				}
				else {
					string temp = line.substr(5);
					istringstream is(temp);
					int tempi;
					is >> tempi;
					turing->tapenum = tempi;
					//cout << turing->tapenum << endl;
				}
			}
			else {
				Transfunc *trans = new Transfunc;
				int pos = line.find(';');
				if (pos != -1) {
					int location = 0;
					for (int k = pos - 1; k >= 0; k--) {
						if (line[k] != ' ') {
							location = k + 1;
							break;
						}
					}
					line = line.erase(location);
				}
				int p = 0;
				string os = "";
				while (line[p] != ' ') {
					os = os + line[p];
					p++;
				}
				trans->oldstate = os;
				p++;
				while (line[p] != ' ') {
					trans->oldstr.push_back(line[p]);
					p++;
				}
				p++;
				while (line[p] != ' ') {
					trans->newstr.push_back(line[p]);
					p++;
				}
				p++;
				while (line[p] != ' ') {
					trans->dir.push_back(line[p]);
					p++;
				}
				p++;
				string ns = "";
				while (p < line.length()) {
					if(line[p] != ' ')
						ns = ns + line[p];
					p++;
				}
				trans->newstate = ns;
				turing->tf.push_back(trans);
			/*	cout << trans->oldstate << " ";
				for (int i = 0; i < trans->oldstr.size(); i++) {
					cout << trans->oldstr[i];
				}
				cout << " ";
				for (int i = 0; i < trans->newstr.size(); i++) {
					cout << trans->newstr[i];
				}
				cout << " ";
				for (int i = 0; i < trans->dir.size(); i++) {
					cout << trans->dir[i];
				}
				cout << " ";
				cout << trans->newstate << endl;*/
				//break;
			}
		}
		fin.close();
	}

	void isValid(string inputstr) {
		bool flag = true;
		for (int i = 0; i < inputstr.length(); i++) {
			bool subflag = false;
			for (int j = 0; j < turing->inputSet.size(); j++) {
				if (turing->inputSet[j] == inputstr[i]) {
					subflag = true;
					break;
				}
			}
			if (subflag == false) {
				flag = false;
				break;
			}
		}
		ofstream out((path + "/console.txt").c_str(), ios::app);
		out << "Input: " << inputstr << endl;
		out.close();
		if (flag == false) {
			ofstream out((path + "/console.txt").c_str(), ios::app);
			out << "==================== ERR ====================" << endl;
			out << "The input " + inputstr + " is illegal" << endl;
			out << "==================== END ====================" << endl;
			out.close();
			ofstream out1((path + "/result.txt").c_str(), ios::app);
			out1 << "Error" << endl;
			out1.close();
		}
		else {
			ofstream out((path + "/console.txt").c_str(), ios::app);
			out << "==================== RUN ====================" << endl;
			out.close();
			run(inputstr);
		}
	}

	void run(string inputstr) {
		id = new ID;
		int step = 0;
		id->step = step;
		id->state = turing->q0;
		Tape *tape0 = new Tape;
		tape0->head = 0;
		if (inputstr.size() == 0) {
			tape0->index.push_back(0);
			tape0->str.push_back(turing->space);
		}
		else {
			for (int i = 0; i < inputstr.size(); i++) {
				tape0->index.push_back(i);
				tape0->str.push_back(inputstr[i]);
			}
		}
	
		id->tapes.push_back(tape0);
	/*	for (int i = 0; i < id->tapes[0]->index.size(); i++) {
			cout << id->tapes[0]->index[i] << endl;
		}*/
		for (int k = 0; k < turing->tapenum - 1; k++) {
			Tape * t = new Tape;
			t->head = 0;
			t->index.push_back(0);
			t->str.push_back(turing->space);
			id->tapes.push_back(t);
		}
		print();
		if (check() == true) {
			//cout << "1111111111111111111111111" << endl;
			string ret = "";
			for (int i = 0; i < id->tapes[0]->str.size(); i++) {
				if (id->tapes[0]->str[i] != turing->space) {
					ret = ret + id->tapes[0]->str[i];
				}
			}
			ofstream out((path + "/console.txt").c_str(), ios::app);
			out << "Result: " + ret << endl;
			out << "==================== END ====================" << endl;
			out.close();
			ofstream out1((path + "/result.txt").c_str(), ios::app);
			out1 << ret << endl;
			out1.close();
			for (int i = 0; i < id->tapes.size(); i++) {
				id->tapes[i]->index.clear();
				id->tapes[i]->str.clear();
				delete id->tapes[i];
			}
			delete id;
		}
		else {
			while (true) {
				nextid();
				print();
				//cout << id->step << endl;
				//cout << id->state << endl;
				if (check() == true) {
					string ret = "";
					for (int i = 0; i < id->tapes[0]->str.size(); i++) {
						if (id->tapes[0]->str[i] != turing->space) {
							ret = ret + id->tapes[0]->str[i];
						}
					}
					ofstream out((path + "/console.txt").c_str(), ios::app);
					out << "Result: " + ret << endl;
					out << "==================== END ====================" << endl;
					out.close();
					ofstream out1((path + "/result.txt").c_str(), ios::app);
					out1 << ret << endl;
					out1.close();
					for (int i = 0; i < id->tapes.size(); i++) {
						id->tapes[i]->index.clear();
						id->tapes[i]->str.clear();
						delete id->tapes[i];
					}
					delete id;
					break;
				}
			}
		}
	}

	bool check() {
		bool haltflag = false;
		bool finalflag = false;
		for (int i = 0; i < turing->finalSet.size(); i++) {
			if (id->state == turing->finalSet[i]) {
				finalflag = true;
				break;
			}
		}
		if (finalflag == true) {
			haltflag = true;
		}
		else {
			bool goonflag = false;
			for (int i = 0; i < turing->tf.size(); i++) {
				if (id->state == turing->tf[i]->oldstate) {
					goonflag = true;
					break;
				}
			}
			if (goonflag == false) {
				haltflag = true;
			}
			else {
				haltflag = false;
			}
		}
		return haltflag;
	}

	void print() {
		ofstream out((path + "/console.txt").c_str(), ios::app);
		out << "Step\t:\t" << id->step << endl;
		for (int i = 0; i < id->tapes.size(); i++) {
			int left = 0;
			int right = 0;
			int boss = id->tapes[i]->head;
			for (int j = 0; j < id->tapes[i]->index.size(); j++) {
				left = id->tapes[i]->index[j];
				if (id->tapes[i]->str[j] != turing->space) {
					break;
				}
			}
			for (int j = id->tapes[i]->index.size() - 1; j >= 0; j--) {
				right = id->tapes[i]->index[j];
				if (id->tapes[i]->str[j] != turing->space) {
					break;
				}
			}
			left = left > boss ? boss : left;
			right = right < boss ? boss : right;
			out << "Index";
			out << i;
			out << '\t';
			out << ':';
			for (int j = 0; j < id->tapes[i]->index.size(); j++) {
				if (id->tapes[i]->index[j] >= left && id->tapes[i]->index[j] <= right) {
					out << '\t';
					out << abs(id->tapes[i]->index[j]);
				}
			}
			out << endl;
			out << "Tape";
			out << i;
			out << '\t';
			out << ':';
			for (int j = 0; j < id->tapes[i]->str.size(); j++) {
				if (id->tapes[i]->index[j] >= left && id->tapes[i]->index[j] <= right) {
					out << '\t';
					out << id->tapes[i]->str[j];
				}
			}
			out << endl;
			out << "Head";
			out << i;
			out << '\t';
			out << ':';
			for (int j = 0; j < id->tapes[i]->index.size(); j++) {
				if (id->tapes[i]->index[j] >= left && id->tapes[i]->index[j] < boss) {
					out << '\t';
				}
			}
			out << '\t';
			out << '^';
			out << endl;
		}
		out << "State\t:\t" << id->state << endl;
		out << "------------------------------------------------------" << endl;
		out.close();
	}

	void nextid() {
		id->step = id->step + 1;
		string os = id->state;
		vector<Transfunc *> possible;
		for (int i = 0; i < turing->tf.size(); i++) {
			bool flag1 = false, flag2 = true;
			if (turing->tf[i]->oldstate == os) {
				flag1 = true;
				//possible.push_back(turing->tf[i]);
			}
			for (int j = 0; j < turing->tf[i]->oldstr.size(); j++) {
				if (id->tapes[j]->str[id->tapes[j]->head - id->tapes[j]->index[0]] != turing->tf[i]->oldstr[j] && turing->tf[i]->oldstr[j] != '*') {
					flag2 = false;
					break;
				}
			}
			if (flag1 == true && flag2 == true) {
				possible.push_back(turing->tf[i]);
			}
		}
		int min = 100000;
		int ind = -1;
		for (int i = 0; i < possible.size(); i++) {
			int min_now = 0;
			for (int j = 0; j < possible[i]->oldstr.size(); j++) {
				if (possible[i]->oldstr[j] == '*') {
					min_now++;
				}
			}
			if (min_now < min) {
				min = min_now;
				ind = i;
			}
		}
		id->state = possible[ind]->newstate;
		for (int i = 0; i < id->tapes.size(); i++) {
			if (possible[ind]->newstr[i] != '*') {
				id->tapes[i]->str[id->tapes[i]->head - id->tapes[i]->index[0]] = possible[ind]->newstr[i];
			}
			if (possible[ind]->dir[i] == '*') {
				continue;
			}
			else if (possible[ind]->dir[i] == 'l') {
				id->tapes[i]->head--;
				if (id->tapes[i]->head < id->tapes[i]->index[0]) {
					id->tapes[i]->index.insert(id->tapes[i]->index.begin(), id->tapes[i]->head);
					id->tapes[i]->str.insert(id->tapes[i]->str.begin(), turing->space);
				}
			}
			else {
				id->tapes[i]->head++;
				if (id->tapes[i]->head > id->tapes[i]->index[id->tapes[i]->index.size() - 1]) {
					id->tapes[i]->index.push_back(id->tapes[i]->head);
					id->tapes[i]->str.push_back(turing->space);
				}
			}
		}
	}
};
