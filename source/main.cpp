#include <iostream>
#include <chrono>
#include "chip8.h"
#include "SDLIMP.h"

int main(int argc, char* argv[])
{

    if(argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>" << std::endl;
        std::exit(1);
    }

    int Scale = std::stoi(argv[1]);
	int Delay = std::stoi(argv[2]);
	char const* filename = argv[3];

    SDLIMP MainWindow("Chip Master", 64 * Scale, 32 * Scale, 64, 32);
    Chip8 chip8;
    chip8.LoadRom(filename);

    
    int videoPitch = sizeof(chip8.display[0]) * 64;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = MainWindow.UpdateKeys(chip8.keys);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > Delay)
		{
			lastCycleTime = currentTime;

			chip8.EmulationCycle();

			MainWindow.UpdateScreen(chip8.display, videoPitch);
		}
	}

	return 0;

}
