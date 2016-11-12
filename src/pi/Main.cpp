#include "ui/MainUI.hpp"
#include "car/LinkSTM32.hpp"
#include "car/Car.hpp"

int main() {
	LinkSTM32 link(50);

	// Main loop
	runUI();

	return 0;
}

