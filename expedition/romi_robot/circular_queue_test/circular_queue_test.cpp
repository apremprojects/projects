#include <iostream>
#include "circular_queue.h"

struct MotionCmd {
	MotionCmd() : MotionCmd(0, 0, 0) {};
	MotionCmd(const int8_t lv, const int8_t rv, const int16_t tick)
		: left_v(lv),
		  right_v(rv),
		  ticks(tick) {};
	int8_t left_v;
	int8_t right_v;
	int16_t ticks;
};

int main() {
	CircularQueue<MotionCmd> cmd_q;

	for (int i = 1; i < 6; i++) {
		if (!cmd_q.isFull()) {
			cmd_q.push(MotionCmd(i, i, i));
		} else {
			std::cout << "Failed to add " << i << ", " << i << ", " << i << "\n";
		}
	}

	while (!cmd_q.isEmpty()) {
		MotionCmd cmd;
		cmd_q.front(cmd);
		cmd_q.pop();
		std::cout << (int)cmd.left_v << ", " << (int)cmd.right_v << ", " << (int)cmd.ticks << "\n";
	}
	return 0;
}