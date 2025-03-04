#include "ConsoleGrid.h"
#include "ConsoleInput.h"

#include "Candy.h"
#include "Snake.h"

#include <iostream>

static const int kBoardRowCount = 20;
static const int kBoardColCount = 30;

/*****************************************************************************/


// return snake length concatenated with some status message
string Message(string status, int turnCount, const Snake &snake) {
	int length = snake.length();
	double metric =
		turnCount == 0 ? 0 : static_cast<double>(length) / turnCount;
	return "Length: " + to_string(length) + " Turns: " + to_string(turnCount) +
		   ". Metric: " + to_string(metric) +
		   ". Calories: " + to_string(snake.calories()) + ". " + status;
}

/*****************************************************************************/

int main() {
	bool gameOver = false;
	int turnCount = 0;
	bool twitchMode = false;
	string status = "";
	ConsoleGrid grid(kBoardRowCount, kBoardColCount);

	UserConsoleInput ci; // optionally provide an output filename
						 // UserConsoleInput ci("./recordedkeys.txt");
	// RecordedConsoleInput ci; 		// optionally provide an input filename
	// to playback keys recorded above

	// use this if you are developing an automated player. optionally provide an
	// output filename to submit a contest entry RobotConsoleInput ci;
	//Snake snake;
	AutonomousSnake snake;
	Candy candy = Candy::randomCandy(snake, kBoardRowCount, kBoardColCount);

	snake.drawChars(&grid);
	candy.draw(&grid);

	while (!gameOver) {
		grid.setStatusLine(Message(status, turnCount, snake));
		status = "";

		unsigned int timeout = twitchMode ? 1 : 0;
		char cmd = ci.nextCommandKey(timeout);

		switch (cmd) {
		case ' ':
			twitchMode = !twitchMode;
			break;
		case 'q':
		case 'Q':
			cout << "Player quit" << endl;
			gameOver = true;
			break;
		default:
			snake.steer(cmd, &grid, candy.row(), candy.col());
			cmd = snake.direction();
			ci.writeCommandKey(cmd); // only RobotConsoleInput writes here
			if (cmd == 'q') {
				cout << "Robot quit" << endl;
				gameOver = true;
			}
			break;
		}

		if (snake.willCrash(&grid)) {
			status = "Ouch, my nose!";
			gameOver = true;
			continue;
		}

		snake.step(&grid);

		if (snake.headIntersects(candy.row(), candy.col())) {
			snake.eat(candy.calories());
			snake.setColor(candy.color());
			snake.drawChars(&grid); // needed to change snake's color

			candy = Candy::randomCandy(snake, kBoardRowCount, kBoardColCount);
			candy.draw(&grid);
			// ci.publicWriteSomethng(candy location and cals)
		}

		++turnCount;
	}
	snake.drawChars(&grid);
	grid.setStatusLine(Message(status, turnCount, snake));
}
//Super Duper Dumb version 1 of AutonomousSnake: Length 600, Turns 17312, Metric 0.034658, with 4 spare calories.
