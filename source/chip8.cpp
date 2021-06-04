#include "chip8.h"

uint8_t fontset[80] =
{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  		0x20, 0x60, 0x20, 0x20, 0x70, // 1
  		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
    pc 		= 0x200;
	opcode 	= 0x0;
	sp 		= 0x0;
	ireg 	= 0x0;

	

	for (uint16_t i = 0; i < 16; i++)					//Clears the Stack
	{
		stack[i] = 0;
	}

	for (int i = 0; i < 4096; i++)					//Clears the Memory
	{
		memory[i] = 0;
	}
	
	
	for (uint8_t i = 0; i < 16; i++)					//Clears the registers
	{
		registers[i] = 0;
	}
	
	
	for (uint32_t i = 0; i < 16; i++)					//Clears the Keys
	{
		keys[i] =  0;							
	}


    for(unsigned int i = 0; i < 80; i++)				//Loads the fontset to memory
    {
        memory[0x50 + i] = fontset[i];
    }

	delayt = 0;
	soundt = 0;

}


void Chip8::LoadRom(char const* filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);

	if (file.is_open())
	{
		std::streampos size = file.tellg();
		char* buffer = new char[size];
		file.seekg(0, std::ios::beg);
		file.read(buffer, size);
		file.close();

		for (long i = 0; i < size; ++i)
		{
			memory[0x200 + i] = buffer[i];
		}

		delete[] buffer;
	}
}
	

void Chip8::EmulationCycle()
{
	opcode = ((memory[pc] << 8) | memory[pc + 1]);	//Fetch Opcode

	switch (opcode & 0xF000)						//Decode Opcode
	{
	case 0x0000:
	{
		switch(opcode & 0x000F)
		{
				case 0x0000:		//Clears the display
				{
					for (int i = 0; i < (64 * 32); i++)
					{
						display[i] = 0;
					}
					
					pc += 2;
				}
					break;
				case 0x000E:		//Returns from subroutine
				{	
					--sp;
					pc = stack[sp];
					pc += 2;
				}
					break;

				default:
					printf("Unknown opcode [0000]: 0x%X\n", opcode);
		}
	}
	break;

	case 0x1000:					//Sets program counter to NNN
		pc = (opcode & 0x0FFF);
		
		break;
	case 0x2000:					//Calls subroutine from NNN
		
		stack[sp] = pc;
		sp++;
		pc = (opcode & 0x0FFF);
		
		break;
	case 0x3000:					//Skips the next instruction if VX equals NN
		if (registers[(opcode & 0x0F00) >> 8]  == (opcode & 0x00FF))
		{
			pc += 4;
		}

		else pc += 2;

		break;
	case 0x4000:					//Skips the next instruction if VX is not equal to NN
		if (registers[(opcode & 0x0F00) >> 8]  != (opcode & 0x00FF))
		{
			pc += 4;
		}

		else pc += 2;

		break;
	case 0x5000:					//Skips the next instruction if VX equals VY
		
		if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode  & 0x00F0) >> 4] )
		{
			pc += 4;
		}
		
		else pc += 2;
			
		break;
	case 0x6000:					//Sets Vx to NN

		registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
		pc += 2;
		
		break;
	case 0x7000:					//Adds NN to Vx
		
		registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
		pc += 2;
		
		break;
	case 0x8000:
	{
		switch (opcode & 0x000F)
		{
				case 0x0000:		//Sets Vx = Vy
					registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
				    pc += 2;
					
					break;
				case 0x0001:		//Sets Vx |= Vy
					registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					
					break;
				case 0x0002:		//Sets Vx &= Vy
					registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					
					break;
				case 0x0003:		//Sets Vx ^= Vy
					registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					
					break;
				case 0x0004:		//Sets Vx += Vy, sets carry flag if Vx + Vy is bigger than 255
					
					
					if((registers[(opcode & 0x0F00) >> 8] + registers[(opcode & 0x00F0) >> 4]) > 255)
					{
						registers[0xF] = 1;
					}

					else 
					{
						registers[0xF] = 0;
					}

					registers[(opcode & 0x0F00) >> 8] += registers[(opcode & 0x00F0) >> 4];
					pc += 2;
					
					break;
				case 0x0005:		//Sets 	Vx -= Vy, sets carry flag if theres a borrow
				{	
					
					if(registers[(opcode & 0x0F00) >> 8] > registers[(opcode & 0x00F0) >> 4])
					{
						registers[0xF] = 1;
					}
					
					else 
					{
						registers[0xF] = 0;
					}
					
					registers[(opcode & 0x0F00) >> 8] -= registers[(opcode & 0x00F0) >> 4];
					pc += 2;

				}	
					break;

				case 0x0006:		//Sets VF = The least significant bit of Vx, then shifts Vx right once
					
					registers[0xF] = (registers[(opcode & 0x0F00) >> 8] & 0x1);
					registers[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;

					break;
				case 0x0007:		//Sets Vx = Vy - Vx, sets VF when theres a borrow
				{
					
					if(registers[(opcode & 0x00F0) >> 4] > registers[(opcode & 0x0F00) >> 8])
					{
						registers[0xF] = 1;
					}
					
					else 
					{
						registers[0xF] = 0;
					}
					
					registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4] - registers[(opcode & 0x0F00) >> 8];
					pc += 2;
					
				}

					break;
				case 0x000E:		//Sets the most significant bit of Vx in VF, then shifts Vx left once
					
					registers[0xF] = ((registers[(opcode & 0x0F00) >> 8] & 0x80) >> 7);
					registers[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;

					break;

				default:
					printf("Unknown opcode [8000]: 0x%X\n", opcode);
		}
	}
	break;
	
	case 0x9000:		//Skips the next instruction if Vx != Vy
		if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
		{
			pc += 4;
		}

		else pc += 2;
		
		break;
	case 0xA000:		//Sets the Index register to NNN
		
		ireg = (opcode & 0x0FFF);
		pc += 2;
		
		break;
	case 0xB000:		//Sets Program Counter to V0 + NNN
		pc = registers[0x0] + (opcode & 0x0FFF);

		break;
	case 0xC000:		//Sets Vx to rand() & NN
		
		registers[(opcode & 0x0F00) >> 8] = ((rand() % 255) & (opcode & 0x00FF));
		pc += 2;

		break;
	case 0xD000:		//Draws sprite at coordinate Vx, Vy
	{
			unsigned char Vx = registers[(opcode & 0x0F00) >> 8];
			unsigned char Vy = registers[(opcode & 0x00F0) >> 4];
			unsigned char height = (opcode & 0x000F);
			unsigned short pixel;

			
			registers[0xF] = 0;
			
			for (long iHeight = 0; iHeight < height; iHeight++)
			{
				pixel = memory[ireg + iHeight];
				
				for(long width = 0; width < 8; width++)
				{
					if((pixel & (0x80 >> width)) != 0)
					{
						if(display[((Vx + width) + ((Vy + iHeight) * 64))] == 0xFFFFFFFF)
						{
							registers[0xF] = 1;                                    
						}

						display[(Vx + width) + ((Vy + iHeight) * 64)] ^= 0xFFFFFFFF;
					}
				}
			}
	
			pc += 2;
			
	}
	break;

	case 0xE000:		
	{
		switch(opcode & 0x00FF)
		{
				case 0x009E:		//Skips the next instruction if the key stored in VX is pressed
					if(keys[registers[(opcode & 0x0F00) >> 8]] != 0)
					{
						pc += 4;
					}
					
					else pc += 2;

					break;
				case 0x00A1:		//Skips the next instruction if the key stored in VX is not pressed
					if(keys[registers[(opcode & 0x0F00) >> 8]] == 0)
					{
						pc += 4;
					}
					
					else pc += 2;
					break;

				default:
					printf ("Unknown opcode [E000]: 0x%X\n", opcode);
		}
	}
	break;

	case 0xF000:
	{
		switch(opcode & 0x00FF)
		{	
	
				case 0x0007:		//Sets VX to the value of the delay timer. 
					
					registers[(opcode & 0x0F00) >> 8] = delayt;
					pc += 2;

					break;
				case 0x000A:		//A key press is awaited, and then stored in Vx
				{
					bool keyPress = false;

					for(int i = 0; i < 16; ++i)
					{
						if(keys[i] != 0)
						{
							registers[(opcode & 0x0F00) >> 8] = i;
							keyPress = true;
						}
					}

					
					if(!keyPress)						
						return;

					pc += 2;	
				}	
				break;
				case 0x0015:		//Sets the delay timer to Vx
					delayt = registers[(opcode & 0x0F00) >> 8];
					pc += 2;

					break;
				case 0x0018:		//Sets the sound timer to Vx
					
					soundt = registers[(opcode & 0x0F00) >> 8];
					pc += 2;

					break;
				case 0x001E:		//Sets Index += Vx
					
					ireg += registers[(opcode & 0x0F00) >> 8];
					pc += 2;

					break;
				case 0x0029:		//Sets Index to the location of the sprite in Vx
					
					ireg = 0x50 + (registers[(opcode & 0x0F00) >> 8] * 0x5);
					pc += 2;

					break;
				case 0x0033:		//Stores the binary coded representation of Vx in memory starting at Index
				{	
					memory[ireg]     = registers[(opcode & 0x0F00) >> 8] / 100;
					memory[ireg + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[ireg + 2] = (registers[(opcode & 0x0F00) >> 8] % 100) % 10;					
					pc += 2;
				}
					break;
				case 0x0055:		//Stores V0 to Vx in memory starting at address Index.
				{	
					for(long i = 0; i <= ((opcode & 0x0F00)>> 8); i++)
					{
						memory[ireg + i] = registers[i];
					}
										
					pc += 2;
				}
					break;
				case 0x0065:		//Fills V0 to Vx with values from memory starting at address Index
					
					for(uint8_t i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
					{
						registers[i] = memory[ireg + i];
					}
					
					pc += 2;
					break;

				default:
					printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
		}
	
		break;

		default:
			printf ("Unknown opcode: 0x%X\n", opcode);

	}
}
	if (delayt > 0)
	{
		--delayt;
	}

	if (soundt > 0)
	{
		--soundt;
	}

	
}