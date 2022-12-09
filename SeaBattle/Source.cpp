#define _CRT_SECURE_NO_WARNINGS

/*#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")*/

#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <stack>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <cstring>
#include <cstdio>
#include <climits>
#include <unordered_map>
#include <unordered_set>
#include <time.h>
#include <windows.h>
#include <conio.h>

using namespace std;

typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef pair <ll, ll> pll;
typedef vector <ll> vec;
typedef vector <pll> vecp;
typedef vector <ld> vecd;
typedef vector <vector <ll>> matrix;

const ld PI = 3.141592653589793;
const ld eps = 1e-9;
const ll INF = 1e16;
const ll MOD = 1e9 + 7;
const ll MAX_N = 1e5 + 3;

#define pb push_back
#define mp make_pair
#define all(v) v.begin(), v.end()
#define fast_io ios::sync_with_stdio(false), cin.tie(0), cout.tie(0)
#define fixed_out(x) fixed << setprecision(x)

const char block[] = "__";
const char sub_block = '_';

const enum Color {
	Black,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Brown,
	LightGray,
	DarkGray,
	LightBlue = 9,
	LightGreen,
	LightCyan,
	LightRed,
	LightMagenta,
	Yellow,
	White
};

const HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

void set_color(Color text, Color background) {
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void set_cursor(ll x, ll y) {
	COORD myCoords = { x, y };
	SetConsoleCursorPosition(hStdOut, myCoords);
}

class Ship {
private:
	ll size, hp;
	bool is_alive, orientation; //0-vertical 1-gorizontal
public:
	Ship(ll new_size, bool new_orientation);
	void hit();
	bool get_is_alive();
	bool get_orientation();
	ll get_size();
};

class Field {
private:
	matrix field;
	Color border_color;
	ll ships_cnt;
	vector <Ship> ships;
	char symbols[10] = { 'À', 'Á', 'Â', 'Ã', 'Ä', 'Å', 'Æ', 'Ç', 'È', 'Ê' };

	void show_border(ll x, ll y);
	void show_field(ll x, ll y);
public:
	bool check_pos(ll x, ll y, Ship ship);
	void set_ship(ll x, ll y, Ship ship);
	void generate_field();
	Field(Color new_border_color);
	void show(ll x, ll y);
	void update_field(ll x, ll y);
};

Ship::Ship(ll new_size = 1, bool new_orientation = 1) {
	size = new_size;
	hp = size;
	is_alive = 1;
	orientation = new_orientation;
}

void Ship::hit() {
	--hp;
	is_alive = (hp > 0);
}

bool Ship::get_is_alive() {
	return is_alive;
}

bool Ship::get_orientation() {
	return orientation;
}

ll Ship::get_size() {
	return size;
}

Field::Field(Color new_border_color) {
	ships_cnt = 0;
	border_color = new_border_color;
	field.resize(10);
	for (int i = 0; i < 10; ++i) {
		field[i].resize(10);
		fill(all(field[i]), 0);
	}
}

void Field::show_border(ll x, ll y) {
	set_color(border_color, border_color);
	set_cursor(x, y);
	cout << block;
	for (int i = 0; i < 10; ++i) {
		set_color(border_color, border_color);
		cout << sub_block;
		set_color(White, border_color);
		cout << symbols[i];
	}
	set_color(border_color, border_color);
	cout << block;
	for (int i = 1; i <= 10; ++i) {
		if (i < 10) {
			set_cursor(x, y + i);
			set_color(border_color, border_color);
			cout << sub_block;
			set_color(White, border_color);
			cout << i;
		}
		else {
			set_cursor(x, y + i);
			set_color(White, border_color);
			cout << i;
		}
		set_cursor(x + 22, y + i);
		set_color(border_color, border_color);
		cout << block;
	}
	set_cursor(x, y + 11);
	set_color(border_color, border_color);
	for (int i = 0; i < 12; ++i) {
		cout << block;
	}
}

void Field::show_field(ll x, ll y) {
	set_cursor(x, y);
	for (int i = 0; i < 10; ++i) {
		set_cursor(x, y + i);
		for (int j = 0; j < 10; ++j) {
			if (field[i][j] == 0) {
				set_color(White, White);
			}
			if (field[i][j] == 1) {
				set_color(LightGray, LightGray);
			}
			if (field[i][j] >= 10) {
				set_color(Red, Red);
			}
			cout << block;
		}
	}
}

bool Field::check_pos(ll x, ll y, Ship ship) {
	ll size = ship.get_size();
	bool flag = 1;
	bool orientation = ship.get_orientation();
	if (orientation) {
		if (y + size > 9) {
			return 0;
		}
	}
	else {
		if (x + size > 9) {
			return 0;
		}
	}
	if (orientation) {
		if (y > 0) {
			if (field[x][y - 1] != 0) {
				flag = 0;
			}
			if (x > 0) {
				if (field[x - 1][y - 1] != 0) {
					flag = 0;
				}
			}
			if (x < 9) {
				if (field[x + 1][y - 1] != 0) {
					flag = 0;
				}
			}
		}
		if (y + size - 1 < 9) {
			if (field[x][y + size] != 0) {
				flag = 0;
			}
			if (x > 0) {
				if (field[x - 1][y + size] != 0) {
					flag = 0;
				}
			}
			if (x < 9) {
				if (field[x + 1][y + size] != 0) {
					flag = 0;
				}
			}
		}
	}
	else {
		if (x > 0) {
			if (field[x - 1][y] != 0) {
				flag = 0;
			}
			if (y > 0) {
				if (field[x - 1][y - 1] != 0) {
					flag = 0;
				}
			}
			if (y < 9) {
				if (field[x - 1][y + 1] != 0) {
					flag = 0;
				}
			}
		}
		if (x + size - 1 < 9) {
			if (field[x + size][y] != 0) {
				flag = 0;
			}
			if (y > 0) {
				if (field[x + size][y - 1] != 0) {
					flag = 0;
				}
			}
			if (y < 9) {
				if (field[x + size][y + 1] != 0) {
					flag = 0;
				}
			}
		}
	}

	for (int i = 0; i < size; ++i) {
		if (orientation) {
			if (field[x][y + i] != 0) {
				flag = 0;
			}
			if (x > 0) {
				if (field[x - 1][y + i] != 0) {
					flag = 0;
				}
			}
			if (x < 9) {
				if (field[x + 1][y + i] != 0) {
					flag = 0;
				}
			}
		}
		else {
			if (field[x + i][y] != 0) {
				flag = 0;
			}
			if (y > 0) {
				if (field[x + i][y - 1] != 0) {
					flag = 0;
				}
			}
			if (y < 9) {
				if (field[x + i][y + 1] != 0) {
					flag = 0;
				}
			}
		}
	}
	return flag;
}

void Field::set_ship(ll x, ll y, Ship ship) {
	ll num = 10 + ships_cnt;
	++ships_cnt;
	ships.pb(ship);
	bool orientation = ship.get_orientation();
	for (int i = 0; i < ship.get_size(); ++i) {
		if (orientation) {
			field[x][y + i] = num;
		}
		else {
			field[x + i][y] = num;
		}
	}
}

void Field::generate_field() {
	for (int i = 0; i < 4; ++i) {
		for (int j = i; j < 4; ++j) {
			Ship ship(i + 1, bool(rand() % 2));
			bool flag = 1;
			while (flag) {
				ll x = rand() % 10;
				ll y = rand() % 10;
				if (check_pos(x, y, ship)) {
					set_ship(x, y, ship);
					flag = 0;
				}
			}
		}
	}
}

void Field::show(ll x, ll y) {
	show_border(x, y);
	show_field(x + 2, y + 1);
}

void Field::update_field(ll x, ll y) {
	if (field[x][y] == 0) {
		field[x][y] = 1;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));
	set_color(White, Black);
	system("cls");

	Field field(Blue);
	set_color(Black, Black);
	system("cls");
	field.generate_field();
	Ship ship1(3, 1), ship2(3, 0);
	/*if (field.check_pos(9, 3, ship1)) {
		field.set_ship(9, 3, ship1);
	}
	if (field.check_pos(6, 5, ship2)) {
		field.set_ship(6, 5, ship2);
	}*/
	field.show(0, 0);

	set_color(Black, Black);
	system("cls");
	set_cursor(0, 0);
	set_color(White, Black);
	return 0;
}

// 9 3 3 1
// 6 5 3 0