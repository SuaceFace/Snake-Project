#ifndef __SNAKE__
#define __SNAKE__

#include "ConsoleGrid.h"
#include "ConsoleInput.h"

#include "SnakeLinkedList.h"

class Snake : public SnakeLinkedList {
  public:
	Snake();

	// This method is provided for you. Use it as-is
	CommandKey direction();

	// Returns true if the next position of the snake's head
	// is an invalid position. The next position should be determined
	// by the snake's direction of travel.
	// Invalid positions are those off the grid or those intersecting
	// with any of the snake's nodes.
	bool willCrash(const ConsoleGrid *grid) const;

	// Returns whether the position matches the coordinates of any
	// of the snake's nodes.
	bool intersects(int row, int col) const;

	// Draws each of the snake's nodes on the ConsoleGrid.
	// This is called by main(). It is called to start the game and to
	// change the snake's color after eating.
	// Your code should **never** need to call this.
	void drawChars(ConsoleGrid *grid) const;


	// Update _dRow, _dCol to indicate the direction of next step.
	// For those building an Autonomous subclass, overide this method.
	// The overriden method should ignore the cmd parameter.
	// This base class implementation should ignore grid and candy-coords.
	virtual void steer(char cmd, const ConsoleGrid *grid, int candyRow,
					   int candyCol);

	// Have the snake move one step in its direction of travel.
	// Snakes move by adding a head node and removing their tail. They
	// grow by adding a head node and **not** removing their tail.
	// IMPORTANT: Draw and erase only the nodes that are necessary to perform
	// the step.
	void step(ConsoleGrid *grid);

	// eat() tells the snake that it has eaten a Candy with the given calories.
	// In N subequent turns (where N is the number of calories eaten)
	// the snake must increase its length by one segment per turn.

	// HINT: Use calories as an indicator that the snake should grow
	// in the step() method. Decrement when the snake grows, and
	// grow when _calories are > 0.
	void eat(int calories){_calories = calories;}



	// Return a bool indiciating whether the head has the given position.
	bool headIntersects(int row, int col) {
		return head()->row() == row && head()->col() == col;
	}

	int dRow() const { return _dRow; }
	int dCol() const { return _dCol; }
	int calories() const { return _calories; }

	void setColor(Color color) { _color = color; }

  protected:
	int _calories = 0;
	Color _color = defaultColor;
	char _glyph = 'o';
	char _headGlyph = '@';
	int _dRow = 0;
	int _dCol = 1;
};

class AutonomousSnake : public Snake{
public:
	void steer(char cmd, const ConsoleGrid *grid, int candyRow,
						int candyCol) override;
};


#endif