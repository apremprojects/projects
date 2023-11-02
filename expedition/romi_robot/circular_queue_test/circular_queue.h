#pragma once

template <typename T>
class CircularQueue {
public:
	static constexpr size_t SIZE = 4;

	CircularQueue() {
		clear();
	};

	bool isEmpty() const {
		return front_ == rear_;
	}

	bool isFull() const {
		return ((rear_ + 1) % ARRAY_SIZE) == front_;
	}

	void front(T& d) {
		d = data[front_];
	}

	void pop() {
		front_ = (front_ + 1) % ARRAY_SIZE;
	}

	void push(const T& d) {
		data[rear_] = d;
		rear_ = (rear_ + 1) % ARRAY_SIZE;
	}

	void clear() {
		front_ = rear_ = 0;
	}

private:
	int rear_;
	int front_;
	static constexpr size_t ARRAY_SIZE = (SIZE + 1);
	T data[ARRAY_SIZE];
};
