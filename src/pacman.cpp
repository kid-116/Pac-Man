#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <cwchar>
#include <conio.h>
#include <ctime>
#include <map>

using namespace std;

typedef long long int lli;

#define MAP_FILE "map.txt"
#define PORTAL_FILE "portals.txt"

#define BLUE 9
#define WHITE 15
#define YELLOW 14
#define RED 4
#define PINK 13
#define BLACK 16
#define GREEN 2
#define ORANGE 12
#define AQUA 11

typedef vector<vector<vector<int>>> ivec3d;
typedef vector<vector<int>> ivec2d;
typedef vector<int> ivec1d;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

/*
		000000000011111111112	
		012345678901234567890

	00	 %%%%%%%%%%%%%%%%%%%
	01	 %........%........%
	02	 %?%%.%%%.%.%%%.%%?%
	03	 %.................%
	04	 %.%%.%.%%%%%.%.%%.%
	05	 %....%...%...%....%
	06	 %%%%.%%% % %%%.%%%%
	07	    %.%       %.%
	08	%%%%%.% %% %% %.%%%%%
	09	     .  %   %  .
	10	%%%%%.% %%%%% %.%%%%%
	11	    %.%       %.%
	12	 %%%%.% %%%%% %.%%%%
	13	 %........%........%
	14	 %.%%.%%%.%.%%%.%%.%
	15	 %?.%..... .....%.?%
	16	 %%.%.%.%%%%%.%.%.%%
	17	 %....%...%...%....%
	18	 %.%%%%%%.%.%%%%%%.%
	19	 %.................%
	20	 %%%%%%%%%%%%%%%%%%%

		0       - wall
		1       - empty
		2       - fruit
		3       - powerup
		5,6,7,8 - ghosts
		4       - pacman

*/

int chtoi(char ch) {
	return ch - 48; // ascii for 0 is 48
}

void gotoxy(short x, short y) {
    COORD c = { y, x };  
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void clrscr() {
	system("CLS");
}

int signum(int n) {
	if(n < 0) {
		return -1;
	}
	else if(n > 0) {
		return 1;
	}
	return 0;
}

int relu(int n) {
	if(n < 0) {
		return 0;
	}
	return n;
}

enum Direction {
	NONE = -1,
	UP = 8,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 6
};

map<Direction, Direction> opposite {
	{UP, DOWN},
	{LEFT, RIGHT},
	{RIGHT, LEFT},
	{DOWN, UP}
};

class Token {
private:
	char ch;
	int color;
public:
	Token(char ch, int color) {
		this->ch = ch;
		this->color = color;
	}
	print(int alt_color = 0) {
		if(alt_color != 0) {
			SetConsoleTextAttribute(hConsole, alt_color);
		}
		else {
			SetConsoleTextAttribute(hConsole, color);
		}
		// SetConsoleTextAttribute(hConsole, color);
		cout << ch;
	} 
};

Token WALL('%', BLUE);
Token EMPTY('\'', BLACK);
Token FRUIT('.', WHITE);
Token POWERUP('?', WHITE);

struct Portal {
	int x1, y1;
	Direction d1;
	int x2, y2;
	Direction d2;
};

class Map {
private:
	ivec2d map;
	int height;
	int width;
	vector<Portal> portals;
public:
	bool is_empty() {
		for(int r = 0; r < height; r++) {
			for(int c = 0; c < width; c++) {
				if(map[r][c] == 2 || map[r][c] == 3) {
					return false;
				}
			}
		}
		return true;
	}
	bool completed() {
		for(int r = 0; r < height; r++) {
			for(int c = 0; c < width; c++) {
				if(map[r][c] == 2 || map[r][c] == 3) {
					return false;
				}
			}
		}
		return true;
	}
	Map(const string mfname, const string pfname) {
		load_map(mfname);
		load_portals(pfname);
	}
	void load_portals(const string pfname) {
		ifstream fportals;
		fportals.open(pfname);
		if(!fportals) {
			cout << "404: portal(s) file <" << pfname << "> not found!\n";
			exit(-1);
		}
		while(!fportals.eof()) {
			Portal portal;
			int d1, d2;
			fportals >> portal.x1 >> portal.y1 >> d1;
			fportals >> portal.x2 >> portal.y2 >> d2;
			portal.d1 = Direction(d1);
			portal.d2 = Direction(d2);
			portals.push_back(portal);
		}
		fportals.close();
	}
	void load_map(const string mfname) {
		// opening the file
		ifstream fmap;
		fmap.open(mfname);
		if(!fmap) {
			cout << "404: map file <" << mfname << "> not found!\n";
			exit(-1);
		}
		// vaidating dimensions
		// all files are ended with '\n'
		height = 0;
		width = -1;
		{
			int w = 0;
			char ch;
			while(!fmap.eof()) {
				char ch = fmap.get();
				if(ch == '\n') {
					++height;
					if(width == -1) {
						width = w;
					}
					else if(width != w) {
						cout << "INVALID FILE ERROR <" << mfname << ">\n";
						exit(-1);
					}
					w = 0;
				}
				else {
					++w;
				}
			}
		}
		map.resize(height, vector<int>(width, -1));
		fmap.clear();
		fmap.seekg(0, ios::beg);
		{
			int r = 0, c = 0;
			while(!fmap.eof()) {
				char ch;
				fmap.get(ch);
				if(ch == '\n') {
					++r;
					c = 0;
					continue;
				}
				map[r][c++] = chtoi(ch);
			}
		}
		fmap.close();
	}
	void print_at(int r, int c) {
		gotoxy(r, c);
		switch(map[r][c]) {
			case 0:
				WALL.print();
				break;
			case 1:
				cout << " ";
				break;
			case 2:
				FRUIT.print();
				break;
			case 3:
				POWERUP.print();
				break;
			default:
				printf("ERROR: invalid token!\n");
				exit(-1);
		}
	}
	void print() {
		clrscr();
		for(int r = 0; r < height; r++) {	
			for(int c = 0; c < width; c++) {
				Sleep(5);
				print_at(r, c);
			}
			cout << endl;
		}
		gotoxy(height + 2, 0);
		printf("POINTS:\t\tLIVES:");
	}
	void print_points(int points) {
		gotoxy(height + 2, 7);
		cout << points;
	}
	void print_lives(int lives) {
		gotoxy(height + 2, 22);
		cout << lives;
	}
	friend class Ghost;
	friend class Pacman;
};

/*
	directions
		8
	4		6
		2
*/

class Pacman {
private:
	Direction next_dir;
	int lives;
	bool alive;
	bool is_powered;
	int points;
	void get_input(char key) {
		switch(key) {
			case 'w':
				next_dir = UP;
				break;
			case 'a':
				next_dir = LEFT;
				break;
			case 's':
				next_dir = DOWN;
				break;
			case 'd':
				next_dir = RIGHT;
				break;
		}
	}
	void change_dir(Map& map) {
		switch(next_dir) {
			case UP:
				if(map.map[x - 1][y]) {
					dir = next_dir;
				}
				break;
			case DOWN:
				if(map.map[x + 1][y]) {
					dir = next_dir;
				}
				break;
			case LEFT:
				if(map.map[x][y - 1]) {
					dir = next_dir;
				}
				break;
			case RIGHT:
				if(map.map[x][y + 1]) {
					dir = next_dir;
				}
				break;
		}
	}
	bool eat(Map& map) {
		switch(map.map[x][y]) {
			case 2:
				map.map[x][y] = 1;
				++points;
				return false;
				break;
			case 3:
				points += 5;
				map.map[x][y] = 1;
				return true;
				break;
			default:
				return false;
				break;
		}
	}
protected:
	Direction dir;
	int x, y;
	Token token;
	void print(int color = 0) {
		gotoxy(x, y);
		token.print(color);
	}
	void change_coor(Map& map) {
		bool teleported = false;
		for(int p = 0; p < map.portals.size(); p++) {
			Portal portal = map.portals[p];
			if(x == portal.x1 && y == portal.y1 && dir == portal.d1) {
				teleported = true;
				x = portal.x2;
				y = portal.y2;
				break;
			}
			else if(x == portal.x2 && y == portal.y2 && dir == portal.d2) {
				teleported = true;
				x = portal.x1;
				y = portal.y1;
				break;
			}
		}
		if(!teleported) {
			switch(dir) {
				case UP:
					if(map.map[x - 1][y]) {
						x -= 1;
					}
					break;
				case DOWN:
					if(map.map[x + 1][y]) {
						x += 1;
					}
					break;
				case LEFT:
					if(map.map[x][y - 1]) {
						y -= 1;
					}
					break;
				case RIGHT:
					if(map.map[x][y + 1]) {
						y += 1;
					}
					break;
			}
		}
	}
public:
	void reset() {
		x = 15;
		y = 10;
	}
	Pacman(char ch='@', int color=YELLOW, int x=15, int y=10) : token(ch, color) {
		next_dir = NONE;
		dir = NONE;
		this->x = x;
		this->y = y;
		lives = 2;
		points = 0;
		alive = true;
		is_powered = false;
	}
	bool update(Map& map) {
		if(kbhit()) {
			get_input(getch());
		}
		change_dir(map);
		map.print_at(x, y);
		change_coor(map);
		print();
		return eat(map);
	}
	friend class Ghost;
	friend class Game;
};

class Ghost : public Pacman {
private:
	bool exists;
	int id;
	bool poisoned;
	void poison() {}
	void go_up() {
		dir = UP;
	}
	void go_left() {
		dir = LEFT;
	}
	void go_down() {
		dir = DOWN;
	}
	void go_right() {
		dir = RIGHT;
	}
	bool is_dead() {
		return exists;
	}
	void die() {
		exists = 0;
	}
	bool is_poisoned;
	void change_coor(Map& map) {
		bool teleported = false;
		for(int p = 0; p < map.portals.size(); p++) {
			Portal portal = map.portals[p];
			if(x == portal.x1 && y == portal.y1 && dir == portal.d1) {
				teleported = true;
				x = portal.x2;
				y = portal.y2;
				break;
			}
			else if(x == portal.x2 && y == portal.y2 && dir == portal.d2) {
				teleported = true;
				x = portal.x1;
				y = portal.y1;
				break;
			}
		}
		if(!teleported) {
			switch(dir) {
				case UP:
					if(!senses.blocked[UP]) {
						x -= 1;
					}
					break;
				case DOWN:
					if(!senses.blocked[DOWN]) {
						x += 1;
					}
					break;
				case LEFT:
					if(!senses.blocked[LEFT]) {
						y -= 1;
					}
					break;
				case RIGHT:
					if(!senses.blocked[RIGHT]) {
						y += 1;
					}
					break;
			}
		}
	}
	struct Sense {
		map<Direction, bool> blocked;
		map<Direction, bool> ghosts;
		map<Direction, bool> pacman;
		void update(Map& map, Ghost& g1, Ghost& g2, Ghost& g3, Pacman& p, int x, int y) {
			// sensing immediate blocks
			blocked[UP] = !map.map[x - 1][y] || ((y == g1.y && x - 1 == g1.x) || (y == g2.y && x - 1 == g2.x) || (y == g3.y && x - 1 == g3.x));
			blocked[DOWN] = !map.map[x + 1][y] || ((y == g1.y && x + 1 == g1.x) || (y == g2.y && x + 1 == g2.x) || (y == g3.y && x + 1 == g3.x));
			blocked[RIGHT] = !map.map[x][y + 1] || ((x == g1.x && y + 1 == g1.y) || (x == g2.x && y + 1 == g2.y) || (x == g3.x && y + 1 == g3.y));
			blocked[LEFT] = !map.map[x][y - 1] || ((x == g1.x && y - 1 == g1.y) || (x == g2.x && y - 1 == g2.y) || (x == g3.x && y - 1 == g3.y));
			// sensing ghosts
			ghosts[UP] = ((y == g1.y && x - 1 == g1.x) || (y == g2.y && x - 1 == g2.x) || (y == g3.y && x - 1 == g3.x) || (y == g1.y && x - 2 == g1.x) || (y == g2.y && x - 2 == g2.x) || (y == g3.y && x - 2 == g3.x));
			ghosts[DOWN] = ((y == g1.y && x + 1 == g1.x) || (y == g2.y && x + 1 == g2.x) || (y == g3.y && x + 1 == g3.x) || (y == g1.y && x + 2 == g1.x) || (y == g2.y && x + 2 == g2.x) || (y == g3.y && x + 2 == g3.x));
			ghosts[LEFT] = ((x == g1.x && y - 1 == g1.y) || (x == g2.x && y - 1 == g2.y) || (x == g3.x && y - 1 == g3.y) || (x == g1.x && y - 2 == g1.y) || (x == g2.x && y - 2 == g2.y) || (x == g3.x && y - 2 == g3.y));
			ghosts[RIGHT] = ((x == g1.x && y + 1 == g1.y) || (x == g2.x && y + 1 == g2.y) || (x == g3.x && y + 1 == g3.y) || (x == g1.x && y + 2 == g1.y) || (x == g2.x && y + 2 == g2.y) || (x == g3.x && y + 2 == g3.y));
			// sensing pacman
			pacman[UP] = ((y == p.y && x - 1 == p.x) || (y == p.y && x - 1 == p.x) || (y == p.y && x - 1 == p.x) || (y == p.y && x - 2 == p.x) || (y == p.y && x - 2 == p.x) || (y == p.y && x - 2 == p.x));
			pacman[DOWN] = ((y == p.y && x + 1 == p.x) || (y == p.y && x + 1 == p.x) || (y == p.y && x + 1 == p.x) || (y == p.y && x + 2 == p.x) || (y == p.y && x + 2 == p.x) || (y == p.y && x + 2 == p.x));
			pacman[LEFT] = ((x == p.x && y - 1 == p.y) || (x == p.x && y - 1 == p.y) || (x == p.x && y - 1 == p.y) || (x == p.x && y - 2 == p.y) || (x == p.x && y - 2 == p.y) || (x == p.x && y - 2 == p.y));
			pacman[RIGHT] = ((x == p.x && y + 1 == p.y) || (x == p.x && y + 1 == p.y) || (x == p.x && y + 1 == p.y) || (x == p.x && y + 2 == p.y) || (x == p.x && y + 2 == p.y) || (x == p.x && y + 2 == p.y));
		}
	} senses;
	void sense_dir(Map &map, Ghost& g1, Ghost& g2, Ghost& g3, Pacman& p) {
		bool in_portal = false;
		for(int p = 0; p < map.portals.size(); p++) {
			Portal portal = map.portals[p];
			if(x == portal.x1 && y == portal.y1) {
				in_portal = true;
				break;
			}
			else if(x == portal.x2 && y == portal.y2) {
				in_portal = true;
				break;
			}
		}
		if(!in_portal) {
			senses.update(map, g1, g2, g3, p, x, y);
			// monitor();
			head(seq[dir]);
			if(senses.ghosts[dir]) {
				dir = opposite[dir];
			}
			if(senses.pacman[UP]) {
				go_up();
			}
			else if(senses.pacman[LEFT]) {
				go_up();
			}
			else if(senses.pacman[RIGHT]) {
				go_right();
			}
			else if(senses.pacman[DOWN]) {
				go_down();
			}
			if(dir == NONE || senses.blocked[dir]) {
				head(seq[Direction(2 * (1 + rand() % 4))]);
			}
		}
	}
	void head(vector<Direction> seq) {
		for(int i = 0; i < seq.size(); i++) {
			if(!senses.blocked[seq[i]]) {
				dir = seq[i];
				break;
			}
		}
	}
	static map<Direction, vector<Direction>> seq;
	void move(Map& map) {
		map.print_at(x, y);
		change_coor(map);
	}
	void monitor() {
		gotoxy(25, 0); 
		printf("DEBUG: %d\n", id);
		printf("?:%d\tx:%d\ty:%d\td:%d\n", exists, x, y, dir);
		printf("ULDR\n");
		printf("blocked: %d%d%d%d\n", senses.blocked[UP], senses.blocked[LEFT], senses.blocked[DOWN], senses.blocked[RIGHT]);
		printf("ghosts: %d%d%d%d\n", senses.ghosts[UP], senses.ghosts[LEFT], senses.ghosts[DOWN], senses.ghosts[RIGHT]);
		printf("pacman: %d%d%d%d\n", senses.pacman[UP], senses.pacman[LEFT], senses.pacman[DOWN], senses.pacman[RIGHT]);
	} 
public:
	Ghost(int color, int id, int x, int y):Pacman('#', color, x, y) {
		this->id = id;
		exists = false;
		is_poisoned = false;
	}
	void reset(int x, int y, bool exists = true) {
		this->x = x;
		this->y = y;
		is_poisoned = false;
		this->exists = exists;
	}
	void update(Map& map, Ghost&g1, Ghost&g2, Ghost&g3, Pacman &p) {
		if(exists) {
			sense_dir(map, g1, g2, g3, p);
			move(map);
		}
		if(is_poisoned) {
			print(AQUA);
		}
		else {
			print();
		}
	}
	friend class Game;
};

map<Direction, vector<Direction>> Ghost::seq = {
	{UP, {UP, LEFT, RIGHT, DOWN}},
	{LEFT, {DOWN, UP, LEFT, RIGHT}},
	{DOWN, {LEFT, DOWN, RIGHT, UP}},
	{RIGHT, {RIGHT, DOWN, UP, LEFT}}
};

const int RED_X = 7, RED_Y = 10;
const int GREEN_X = 9, GREEN_Y = 9;
const int PINK_X = 9, PINK_Y = 10;
const int ORANGE_X = 9, ORANGE_Y = 11;

class Game {
private:
	Map map;
	Pacman yellow;
	Ghost red, green, pink, orange;
	lli time;
	lli powered_for;
	bool collisions() {
		if(red.x == yellow.x && red.y == yellow.y) {
			if(red.is_poisoned) {
				yellow.points += 50;
				red.reset(RED_X, RED_Y);
			}
			else {
				return true;
			}
		}
		else if(green.x == yellow.x && green.y == yellow.y) {
			if(green.is_poisoned) {
				yellow.points += 50;
				green.reset(GREEN_X, GREEN_Y);
			}
			else {
				return true;
			}
		}
		else if(pink.x == yellow.x && pink.y == yellow.y) {
			if(pink.is_poisoned) {
				yellow.points += 50;
				pink.reset(PINK_X, PINK_Y);
			}
			else {
				return true;
			}
		}
		else if(orange.x == yellow.x && orange.y == yellow.y) {
			if(orange.is_poisoned) {
				yellow.points += 50;
				orange.reset(ORANGE_X, ORANGE_Y);
			}
			else {
				return true;
			}
		}
		return false;
	}
public:
	Game(const string mfname, const string pfname):
		map(mfname, pfname),
		red(RED, 5, RED_X, RED_Y),
		green(GREEN, 6, GREEN_X, GREEN_Y),
		pink(PINK, 7, PINK_X, PINK_Y),
		orange(ORANGE, 8, ORANGE_X, ORANGE_Y) {}
	void setup() {
		map.print();
		map.print_lives(yellow.lives);
		time = 0;
		powered_for = -1;
		red.reset(RED_X, RED_Y, false);
		pink.reset(PINK_X, PINK_Y, false);
		green.reset(GREEN_X, GREEN_Y, false);
		orange.reset(ORANGE_X, ORANGE_Y, false);
		yellow.reset();
	}
	void run() {
		while(yellow.lives) {
			Sleep(500);
			setup();
			while(true) {
				Sleep(150);
				if(yellow.update(map)) {
					red.is_poisoned = true;
					green.is_poisoned = true;
					pink.is_poisoned = true;
					orange.is_poisoned = true;
					yellow.is_powered = true;
					powered_for = 0;
				}
				if(collisions()) {
					--yellow.lives;
					break;
				}
				if(map.is_empty()) {
					++yellow.lives;
					map.load_map(MAP_FILE);
					break;
				}
				red.update(map, green, pink, orange, yellow);
				if(collisions()) {
					--yellow.lives;
					break;
				}
				green.update(map, red, pink, orange, yellow);
				if(collisions()) {
					--yellow.lives;
					break;
				}
				pink.update(map, red, green, orange, yellow);
				if(collisions()) {
					--yellow.lives;
					break;
				}
				orange.update(map, red, green, pink, yellow);
				if(collisions()) {
					--yellow.lives;
					break;
				}
				map.print_points(yellow.points);
				++time;
				if(time > 55) {
					orange.exists = true;	
				}
				else if(time > 35) {
					green.exists = true;	
				}
				else if(time > 20) {
					pink.exists = true;	
				}
				else if(time > 10) {
					red.exists = true;	
				}
				if(yellow.is_powered) {
					if(powered_for > 30) {
						yellow.is_powered = false;
						red.is_poisoned = false;
						green.is_poisoned = false;
						pink.is_poisoned = false;
						orange.is_poisoned = false;
						powered_for = -1;
					}
					++powered_for;
				}
			}
		}
	}
};

int main() {
	srand(time(NULL));
	Game game(MAP_FILE, PORTAL_FILE);
	game.run();
	return 0;
}