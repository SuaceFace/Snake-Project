
#include "Candy.h"

/*********************************************************************************/
// Random generation. Use as-is

static int randANSI() {
	static unsigned n = 1; // or a varying seed: chrono::system_clock::now().time_since_epoch().count()
	return ((n = (n * 1103515245 + 12345) % 2147483648) >> 16) & 0x7FFF;
}

static int Random(int min, int max) {
	return min + randANSI() % (max - min);
}

static void RandomPosition(const Snake &snake, int *row, int *col, int rowCount, int colCount) {
	do {
		*row = Random(0, rowCount);
		*col = Random(0, colCount);
	} while (snake.intersects(*row, *col));
}

static Color RandomColor() {
	Color prettyColors[] = { Color::red, Color::green, Color::yellow, Color::cyan, Color::white };
	int count = sizeof(prettyColors) / sizeof(*prettyColors);
	return prettyColors[Random(0, count)];
}

/*********************************************************************************/
// Candy method definitions

Candy Candy::randomCandy(const Snake &snake, int rowCount, int colCount) {
	int row, col;
	RandomPosition(snake, &row, &col, rowCount, colCount);
	Color color = RandomColor();

	int calories = Random(1, 10);
	int glyph = '0' + calories;
	return Candy(row, col, calories, glyph, color);
}

void Candy::draw(ConsoleGrid *grid) const {
	grid->drawCharAt(_row, _col, _glyph, _color);
}
