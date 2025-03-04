#include "ConsoleGrid.h"
#include "Snake.h"

// Use this constructor as-is.
Snake::Snake() {
	pushBack(5, 5);
	pushBack(5, 4);
	pushBack(5, 3);
}

// This method is provided for you. Use it as-is
CommandKey Snake::direction() {
	CommandKey result = CommandKey('q');
	if (_dRow == 0) {
		result = (_dCol == 1) ? arrowRightKey : arrowLeftKey;
	} else if (_dCol == 0) {
		result = (_dRow == 1) ? arrowDownKey : arrowUpKey;
	}
	return result;
}

// Provide the definitions of the other methods declared in the header file.
// Returns false if the next position of the snake's head
// is a valid position. The next position should be determined
// by the snake's direction of travel.
// Valid positions are those on the grid and those not intersecting
// with any of the snake's nodes.
bool Snake::willCrash(const ConsoleGrid *grid) const {
	// DJH: Google "short circuit evaluation", then ask yourself, which part of the condition would I rather have short circuited? DONE I THINK???
	// EH: Well done on remembering isValidCoord and on the short-circuit evaluation,
	//	   It's better to check bounds first, as intersects() traverses the whole list.
	int nextRow = head()->row() + _dRow;
	int nextCol = head()->col() + _dCol;
	return !(grid->isValidCoord(nextRow, nextCol)) || (intersects(nextRow, nextCol));
}
// Return true if oob or intersects.
	//true if not validcoord or Intersects


// Returns whether the position matches the coordinates of any
// of the snake's nodes.
bool Snake::intersects(int row, int col) const {
	SnakeListNode *curNode = head()->nextNode();
	bool doesIntersect = false;
	while (curNode) {
		if (row == curNode->row() && col == curNode->col()) {
			doesIntersect = true;
			break;
		}
		curNode = curNode->nextNode();
	}
	return doesIntersect;
}

// Draws each of the snake's nodes on the ConsoleGrid.
// This is called by main(). It is called to start the game and to
// change the snake's color after eating.
// Your code should **never** need to call this.
void Snake::drawChars(ConsoleGrid *grid) const {
	SnakeListNode *curNode = head();
	// DJH: you might find a good use for a local var called char glyph (as opposed to _glyph) DONE
	char glyph = _headGlyph;
	while (curNode) {
		grid->drawCharAt(curNode->row(), curNode->col(), glyph, _color);
		curNode = curNode->nextNode();
		glyph = _glyph; // DJH: super duper here.  assignments are fast!
	}
}
// gotta go, class break over, leave any comments you want, I'll look over them tonight
// DJH: OK, search for EH, too.

// Update _dRow, _dCol to indicate the direction of next step.
// For those building an Autonomous subclass, overide this method.
// The overriden method should ignore the cmd parameter.
// This base class implementation should ignore grid and candy-coords.
void Snake::steer(char cmd, const ConsoleGrid *grid, int candyRow,
				  int candyCol) {
	switch (cmd) {
	case arrowUpKey:
		_dCol = 0;
		_dRow = -1;
		break;
	case arrowLeftKey:
		_dCol = -1;
		_dRow = 0;
		break;
	case arrowDownKey:
		_dCol = 0;
		_dRow = 1;
		break;
	case arrowRightKey:
		_dCol = 1;
		_dRow = 0;
	}
}

// Have the snake move one step in its direction of travel.
// Snakes move by adding a head node and removing their tail. They
// grow by adding a head node and **not** removing their tail.
// IMPORTANT: Draw and erase only the nodes that are necessary to perform the
// step.
void Snake::step(ConsoleGrid *grid) {
	grid->drawCharAt(head()->row(), head()->col(), _glyph, _color); // Change old head to regular body part char
	pushFront(head()->row() + _dRow, head()->col() + _dCol);
	grid->drawCharAt(head()->row(), head()->col(), _headGlyph, _color); // Draw new head
	if (_calories == 0) {
		SnakeListNode *tailNode = tail(); 
		// NICE
		grid->eraseCharAt(tailNode->row(), tailNode->col()); // erase tail
		popBack();	// Remove tail
	} else {
		_calories--;
	}
}

void AutonomousSnake::steer(char cmd, const ConsoleGrid *grid, int candyRow, int candyCol) {
	//DD: 3/3, I could clean this up and make it more elegant, but I don't feel like it yet. Maybe next week!
	int r = head()->row();
	int c = head()->col();
	int maxCol = grid->colCount() - 1;
	int maxRow = grid->rowCount() - 1;
	if(c == maxCol && r > 0){
		_dRow = -1;
		_dCol = 0;
	}
	else if(r == 0 && c > 0){
		_dRow = 0;
		_dCol = -1;
	}
	else if(c == 0 && r < maxRow){
		_dRow = 1;
		_dCol = 0;
	}
	else if(r == maxRow){
		if(_dRow == 1){
			_dRow = 0; 
			_dCol = 1;
		} 
		else if(_dCol == 1) { 
			_dRow = -1;
			_dCol = 0;
		}
	}
	else if(r == 1){
		if(_dRow == -1){
			_dRow = 0;
			_dCol = 1;
		}
		else if (_dCol == 1 ){
			_dRow = 1;
			_dCol = 0;
		}
	}
	
}