#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <fstream>
#include <regex>
#include <conio.h>
#include <ctype.h>

using namespace std;



class Cell {
public:
	int x, y;
	bool gameover;
	Cell(int a, int b, bool gamover) {
		x = a;
		y = b;
		gameover = gamover;
	}
	~Cell() = default;
};

class GameState {
public:
	GameState() = default;
	std::string birth, survival, universe, fileName;
	std::vector <Cell> coordinates;
	int heigth, weidth;
};

class Message {
public:

	Message() = default;
	//instructions
	std::string filename = "\nWrite down your in-file name\n";
	std::string inFile = "\nWrite down your number of ticking and out-file with \"-i number_of_iter -o outfile_name\"\n";
	std::string inputString = "\nPlease, put your choice below\n";
	std::string Mode = "\nChoose Mode\n1.Offline\n2.Real-Time\n";
	std::string version = "#Life v1.06";
	//errors
	std::string smthwrong = "Something is wrong\n";
	std::string SeriousErr = "\nIt's a serious problem. You can't run this game offline without number of ticking";
	std::string unknowncom = "\nUnknown command. Try again with correct request\n";
	//warnings
	std::string noInFile = "\nInput file isn't found or doesn't exist.\n\ Standart settings was applied\n";
	std::string vers = "\nUndefined version of Life was detected.\n\Please, check, that you have valid version\n";
	std::string univName = "\nYour universe haven't any name, did you know?\n";
	std::string noOutFile = "\nOutput file isn't found or doesn't exist.\n\ Result will be displayed in console\n";
	//settings
	std::string settings = "\nDo you want to set default settings? y/n\n";
	std::string name = "\nWrite down your universe name. It's optional\n";
	std::string rules = "\nPut down your universe rules for birth and survival. These can be string with digits from 0 to 9\n";
	std::string size = "\nAnd now write size of field. It can be only positive numbers. First is weidth of field, and second - heigth\n";
	std::string cellsnum = "\nPut down number of alive cells\n";
	std::string inp = "\nYou can write your settings with txt-file. If you agree - put filename below\n";
	std::string cells = "\nAnd now write coordinates of alive cells in format that first is x coordinate and second is y coordinate. Starts from upper left angle\n";
	std::string command = "\nWaiting for your command, dude\n";
	std::string succload = "\nYour universe was succesfully loaded in outfile\n";
	//start
	std::string help = "Help to start : \n\
		1. help - show game commands\n\
		2. dump<filename> -save current state\n\
		3. exit - end the game\n\
		4. tick<n> -count game state after n iteration\n";
	std::string startGame =
		"	Game Of Life - this is a game, that was made by John Horton Conway in 1970.\n\
	So, I make it for the second time. Let's enjoy it!\n\
Universe input file format: \n\
1. #N Version \n\
2. #S weidth, heigth \n\
3. #R #Bx / Sy - birth and survival rule, { 0..8 }\n\
4. Cells coordinates.\n\
Help to start: \n\
1. help - show game commands\n\
2. dump<filename> - save current state\n\
3. exit - end the game\n\
4. tick<n> - count game state after n iteration\n";
};

class GameLoader : public GameState, public Message {
public:
	GameLoader() = default;
	bool defaultSettingsUp = false;
	bool noOutFile = false;
	int tick;
	std::string outFile;
	ifstream Load(std::string& a) {
		std::string file = a + ".txt";
		ifstream in;
		in.open(file.c_str());
		if (!in) {
			cout << Message::noInFile;
			SetDefaultSettings();
			defaultSettingsUp = true;
		}
		return in;
	}
	ofstream LoadOut(std::string& a) {
		std::string file = a + ".txt";
		ofstream out;
		out.open(file.c_str());
		if (!out) {
			cout << Message::noOutFile;
			noOutFile = true;
		}
		return out;
	}

	void SetDefaultSettings() {
		this->universe = "MyUniverse";
		this->weidth = 40;
		this->heigth = 40;
		this->birth = "2";
		this->survival = "23";
	}
	void Parse(ifstream& a) {
		std::string temp;
		std::regex ureg("[#N ]([\D]*)"), breg("(#R )(B[0-9]+\/S[0-9]+)"), ite("[0-9]+"), size("[#S ] ([0-9]+) ([0-9]+)");
		std::smatch iterator;
		getline(a, temp);
		if (temp != Message::version) {
			cout << Message::vers;
		}
		getline(a, temp);
		if (std::regex_search(temp, ureg)) {
			std::istringstream is(temp);
			std::vector<std::string> words{ std::istream_iterator<std::string>(is), std::istream_iterator<std::string>() };
			this->universe = words[1];

		}
		else {
			cout << Message::univName;
			this->universe = "StandartName";
		}
		getline(a, temp);
		if (std::regex_search(temp, breg)) {
			std::string str(temp);
			auto iter = std::sregex_iterator(str.begin(), str.end(), ite);
			iterator = *iter;
			this->birth = iterator.str();
			iter++;
			iterator = *iter;
			this->survival = iterator.str();
		}
		else {
			this->birth = "3";
			this->survival = "23";
		}
		getline(a, temp);
		if (std::regex_search(temp, size)) {
			std::string str(temp);
			auto iter = std::sregex_iterator(str.begin(), str.end(), ite);
			iterator = *iter;
			this->weidth = std::stol(iterator.str());
			iter++;
			this->heigth = std::stol(iterator.str());
			iterator = *iter;
		}
		else {
			this->weidth = 40;
			this->heigth = 40;
		}
		int x, y;
		while (!a.eof()) {
			a >> x;
			a >> y;
			this->coordinates.push_back(Cell(x, y, false));
		}
		return;
	}

	void InputGetter() {
		std::regex inp("(-i) [0-9]+"), inp1("--input [0-9]"), out("-o [a-zA-Z]*"), out1("--output [a-zA-Z]*");
		cout << Message::inFile;
		std::string temp;
		getline(cin, temp);
		getline(cin, temp);
		std::istringstream is(temp);
		std::vector<std::string> words{ std::istream_iterator<std::string>(is), std::istream_iterator<std::string>() };
		if (std::regex_search(temp, inp) || std::regex_search(temp, inp1)) {
			this->tick = std::stoi(words[1]);
		}
		else {
			throw(Message::SeriousErr);
		}
		if (std::regex_search(temp, out) || std::regex_search(temp, out1)) {
			this->outFile = words[3];
		}
		else {
			cout << Message::noOutFile;
			this->noOutFile = true;
		}
		return;
	}


};

class Field : public GameLoader {
public:
	std::vector<std::vector<Cell>> field;
	void setField() {
		std::vector <Cell> temp;
		for (int j = 0; j < this->weidth; j++) {
			for (int i = 0; i < this->heigth; i++) {
				Cell moreTemp = Cell(i, j, true);
				temp.push_back(moreTemp);
			}
			this->field.push_back(temp);
			temp.clear();
		}

		return;
	}
	void showField() {
		for (int i = this->coordinates.size() - 1; i >= 0; i--) {
			this->field[this->coordinates[i].x][this->coordinates[i].y] = coordinates[i];
			coordinates.pop_back();

		}
		for (int i = 0; i < this->weidth; i++) {
			for (int j = 0; j < this->heigth; j++) {
				if (!this->field[i][j].gameover) {
					cout << ".";
				}
				else cout << "*";
				cout << "  ";
			}
			cout << "\n";
		}
		return;
	}

};

class Game : public Field {
public:
	Game() = default;

	void LetsBegin() {
		cout << Message::startGame;
		ModeSwitching();

	}
	void ModeSwitching() {
		cout << Message::Mode;
		std::string temp;
		cin >> temp;
		if (temp == "Offline") {
			OfflineGame();
		}
		else if (temp == "Real-Time") {
			RTgame();
		}
		else {
			cout << Message::smthwrong;
			ModeSwitching();
		}
		return;
	}


	void OfflineGame() {
		cout << Message::filename;
		std::string temp;
		cin >> temp;
		ifstream flow = this->Load(temp);
		if (!this->defaultSettingsUp) Parse(flow);
		InputGetter();
		setField();
		offlineGame();

	}
	void RTgame() {
		cout << Message::settings;
		std::string ans;
		cin >> ans;
		if (ans == "y") {
			this->SetDefaultSettings();
		}
		else if (ans == "n") {
			handleSetting();
		}
		else {
			cout << Message::smthwrong;
			RTgame();
		}
		system("cls");
		setField();
		std::string temp;
		while (true) {
			cout << Message::command;
			getline(cin, temp);
			if (temp == "exit") {
				exit(0);
			}
			else if (temp == "help") {
				cout << Message::help;
			}
			else if (temp.substr(0, 4) == "tick") {
				this->tick = std::stoi(temp.substr(5));
				system("cls");
				_game();
			}
			else if (temp.substr(0, 4) == "dump") {
				this->outFile = temp.substr(5);
				offlineGame();
				cout << Message::succload;
			}
			else cout << Message::unknowncom;
		}

	}

	bool mustSurvive(int x, int y) {
		int aliveneigbour = coord(x, y);

		for (int i = 0; i <= this->survival.size() - 1; i++) {
			if (this->survival.c_str()[i] - 48 == aliveneigbour) return false;
		}
		return true;
	}
	int coord(int x, int y) {
		int aliveneigbour = 0;
		int xminus = x - 1;
		int yminus = y - 1;
		if (yminus < 0) yminus = this->heigth - 1;
		if (xminus < 0) xminus = this->weidth - 1;
		int xplus = x + 1;
		int yplus = y + 1;
		if (xplus > this->weidth - 1) xplus %= this->weidth;
		if (yplus > this->weidth - 1)yplus %= this->weidth;


		if (!this->field[yplus][xminus].gameover) aliveneigbour++;
		if (!this->field[yminus][x].gameover) aliveneigbour++;
		if (!this->field[y][xplus].gameover) aliveneigbour++;
		if (!this->field[y][xminus].gameover) aliveneigbour++;
		if (!this->field[yplus][x].gameover) aliveneigbour++;
		if (!this->field[yplus][xplus].gameover) aliveneigbour++;
		if (!this->field[yminus][xminus].gameover) aliveneigbour++;
		if (!this->field[yminus][xplus].gameover) aliveneigbour++;
		return aliveneigbour;
	}


	bool mustBorn(int x, int y) {
		int aliveneigbour = coord(x, y);
		for (int i = 0; i <= this->birth.size() - 1; i++) {
			if (this->birth.c_str()[i] - 48 == aliveneigbour) return false;
		}
		return true;
	}

	void _game() {
		for (int i = this->coordinates.size() - 1; i >= 0; i--) {
			this->field[this->coordinates[i].x][this->coordinates[i].y] = coordinates[i];
			coordinates.pop_back();

		}
		std::vector<Cell> temp;
		for (int k = 0; k < this->tick; k++) {
			cout << Message::version << '\n';
			cout << this->universe << '\n';
			cout << "Current state of field after " + std::to_string(k) + " iterations\n";
			for (int i = 0; i < this->weidth; i++) {
				for (int j = 0; j < this->heigth; j++) {
					if (this->field[i][j].gameover) {
						cout << ".";
					}
					else cout << "*";
					cout << "  ";
					if (!this->field[i][j].gameover) {
						if (this->field[i][j].gameover != mustSurvive(j, i))
							temp.push_back(Cell(j, i, mustSurvive(j, i)));
					}
					else {
						if (this->field[i][j].gameover != mustBorn(j, i))
							temp.push_back(Cell(j, i, mustBorn(j, i)));
					}
				}
				cout << "\n";
			}
			Sleep(1000);

			for (int i = temp.size() - 1; !temp.empty(); i--) {
				this->field[temp[i].y][temp[i].x].gameover = temp[i].gameover;
				temp.pop_back();
			}
			system("cls");
		}
	}
	void offlineGame() {
		if (this->noOutFile) {
			cout << Message::noOutFile;
			_game();
		}
		else {
			ofstream a = LoadOut(this->outFile);

			for (int i = this->coordinates.size() - 1; i >= 0; i--) {
				this->field[this->coordinates[i].x][this->coordinates[i].y] = coordinates[i];
				coordinates.pop_back();

			}
			std::vector<Cell> temp;
			for (int t = 0; t < this->tick; t++) {
				for (int i = 0; i < this->weidth; i++) {
					for (int j = 0; j < this->heigth; j++) {
						if (this->field[i][j].gameover) {
						}
						if (!this->field[i][j].gameover) {
							if (this->field[i][j].gameover != mustSurvive(j, i))
								temp.push_back(Cell(j, i, mustSurvive(j, i)));
						}
						else {
							if (this->field[i][j].gameover != mustBorn(j, i))
								temp.push_back(Cell(j, i, mustBorn(j, i)));
						}
					}
				}
				for (int i = temp.size() - 1; !temp.empty(); i--) {
					this->field[temp[i].y][temp[i].x].gameover = temp[i].gameover;
					temp.pop_back();
				}
			}
			a << Message::version << "\n";
			a << this->universe << '\n';
			for (int i = 0; i < this->weidth; i++) {
				for (int j = 0; j < this->heigth; j++) {
					if (this->field[i][j].gameover)
						a << '.';
					else a << '*';
					a << ' ';
				}
				a << "\n";
			}
			a << "Field after " + std::to_string(this->tick) + " iterations";
		}
	}
	void handleSetting() {
		cout << Message::inp;
		std::string temp;
		getline(cin, temp);
		getline(cin, temp);
		ifstream in = this->Load(temp);
		if (this->defaultSettingsUp) {
			//name
			cout << Message::name;
			std::string nam;
			getline(cin, nam);
			getline(cin, nam);
			this->universe = nam;
			//survival,birth
			cout << Message::rules;
			std::string rule;
			getline(cin, rule);
			std::istringstream is(rule);
			std::vector<std::string> words{ std::istream_iterator<std::string>(is), std::istream_iterator<std::string>() };
			if (words.size() < 2 || words.size() > 2) {
				throw(Message::smthwrong);
			}
			for (int i = 0; i < words[0].size(); i++) {
				if (!isdigit(words[0][i])) throw(Message::smthwrong);
			}
			this->birth = words[0];
			for (int i = 0; i < words[1].size(); i++) {
				if (!isdigit(words[1][i])) throw(Message::smthwrong);
			}
			this->survival = words[1];
			//size
			cout << Message::size;
			std::string siz;
			getline(cin, siz);
			is = std::istringstream(siz);
			std::vector<std::string> ruls{ std::istream_iterator<std::string>(is), std::istream_iterator<std::string>() };
			if (ruls.size() < 2 || ruls.size() > 2) {
				throw(Message::smthwrong);
			}
			if (std::stoi(ruls[0]) < 0) throw(Message::smthwrong);
			else this->weidth = std::stoi(ruls[0]);
			if (std::stoi(ruls[1]) < 0) throw(Message::smthwrong);
			else this->heigth = std::stoi(ruls[1]);

			//alivecells
			cout << Message::cellsnum;
			int n;
			cin >> n;
			cout << Message::cells;
			for (int i = 0; i < n; i++) {
				int x, y;
				cin >> x;
				cin >> y;
				this->coordinates.push_back(Cell(x, y, false));
			}
			return;
		}
		else this->Parse(in);

	}


};

void Start() {
	Game lst;
	lst.LetsBegin();
}


int main() {
	Start();
	return 0;
}
