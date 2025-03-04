#ifndef __CANDY_H__
#define __CANDY_H__

#include "ConsoleGrid.h"
#include "Snake.h"

class Candy {
public:

  Candy() = default;

  Candy(int row, int col, int calories, int glyph, Color color) : _row(row), _col(col), _calories(calories), _glyph(glyph), _color(color) {}

  static Candy randomCandy(const Snake &snake, int rowCount, int colCount);

  void draw(ConsoleGrid *grid) const;

  int row() const { return _row; }
  int col() const { return _col; }
  int calories() const { return _calories; }
  Color color() const { return _color; }

private:
  int _row = -1;
  int _col = -1;
  int _calories = 0;
  char _glyph = ' ';
  Color _color = defaultColor;

};

#endif

