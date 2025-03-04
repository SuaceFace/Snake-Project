#include "ConsoleInput.h"
#include <cstdio>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include <thread>

uint16_t Fletcher16Checksum(const string &filename);


ConsoleInput::~ConsoleInput() {
	if (_file.is_open()) {
		_file.close();
	}

	uint16_t checksum = Fletcher16Checksum(_filename);
	ofstream file(_filename, ios::app);
	if (file.is_open()) {
		file << "Checksum: " << checksum << endl;
	}
}

void ConsoleInput::openKeystrokeFile(ios::openmode mode) {
	_file.open(_filename, mode);
	if (!_file.is_open()) {
		perror(("error opening " + _filename).c_str());
	}
}

char UserConsoleInput::nextCommandKeyHelper(unsigned timeout) {
	char result = noKey;

	struct termios oldAttrs, newAttrs;
	if (tcgetattr(STDIN_FILENO, &oldAttrs) < 0) {
		perror("tcgetattr()");
	}

	newAttrs = oldAttrs;
	cc_t vMin =
		(timeout == 0) ? 1 : 0; // synchronous, wait for at least one char
	cc_t vTime = timeout;
	newAttrs.c_cc[VMIN] = vMin;
	newAttrs.c_cc[VTIME] = vTime;
	newAttrs.c_lflag &= (~ICANON) & (~ECHO);

	if (tcsetattr(STDIN_FILENO, TCSANOW, &newAttrs) < 0) {
		perror("tcsetattr newAttrs");
	}

	// Discards queued up keys by only processing one key from buf.
	// The alternative approach, only reading one key's worth, is complicated
	// Esc sequences require looking ahead and pushing back chars if it turns
	// out not to be an esc sequence.
	char buf[32];
	ssize_t readCount = read(STDIN_FILENO, buf, sizeof(buf));
	if (readCount < 0) {
		perror("read()");
	}

	if (readCount > 0) {
		if (readCount > 2 && buf[0] == '\x1B' && buf[1] == '[') {
			result = escapedCommandKey(buf[2]);
		} else {
			// DJH - dropped support for wasd and nethack cmds
			result = buf[0];
		}

		if (result == unknownKey) {
			printf("Unrecognized key %zu bytes:", readCount);
			for (int i = 0; i < readCount; ++i) {
				printf(" \\x%X ", buf[i]);
			}
			cout << endl;
		}
	}

	if (tcsetattr(STDIN_FILENO, TCSADRAIN, &oldAttrs) < 0) {
		perror("tcsetattr oldAttrs");
	}

	return result;
}

char UserConsoleInput::nextCommandKey(unsigned timeout) {
	char result = nextCommandKeyHelper(timeout);
	unconditionalWriteCommandKey(result);
	return result;
}

void UserConsoleInput::unconditionalWriteCommandKey(char cmd) {
	if (!_file.is_open()) {
		openKeystrokeFile(ios::out);
	}
	if (cmd) {
		_file << to_string(static_cast<int>(cmd)) << endl;
	}
}

char UserConsoleInput::escapedCommandKey(char c) {
	// Interpret special keys based on the 3rd char of 3 or 4.
	// "\x1B[{c}" or "\x1B[{c}~"
	switch (c) {
	case 'A':
		return arrowUpKey;
	case 'B':
		return arrowDownKey;
	case 'C':
		return arrowRightKey;
	case 'D':
		return arrowLeftKey;
	case '3':
		return deleteForwardKey;
	case '5':
		return pageUpKey;
	case '6':
		return pageDownKey;
	default:
		return unknownKey;
	}
}

/*******************************************************************************/

char RecordedConsoleInput::nextCommandKey(unsigned timeout) {
	int charCode = 'q';

	if (!_file.is_open()) {
		openKeystrokeFile(ios::in);
	}

	if (_file >> charCode) {
		unsigned timeout_ms = timeout == 0 ? 50 : timeout * 100;
		this_thread::sleep_for(chrono::milliseconds(timeout_ms));
	}
	return static_cast<char>(charCode);
}

/*******************************************************************************/


void RobotConsoleInput::writeCommandKey(char cmd) {
	unconditionalWriteCommandKey(cmd);
}

char RobotConsoleInput::nextCommandKey(unsigned timeout) {
	return nextCommandKeyHelper(timeout);
}

/*******************************************************************************/

uint16_t Fletcher16Checksum(const string &filename) {
	ifstream file(filename);
	if (!file) {
		cerr << "Error opening file: " << filename << endl;
		return 0;
	}

	uint8_t sum1 = 0, sum2 = 0;
	string line;
	while (getline(file, line)) {
		for (char c : line) {
			sum1 = (sum1 + static_cast<uint8_t>(c)) % 255;
			sum2 = (sum2 + sum1) % 255;
		}
		sum1 = (sum1 + '\n') % 255;
		sum2 = (sum2 + sum1) % 255;
	}
	file.close();
	return (sum2 << 8) | sum1;
}