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

	
	MasterTable[0x0] = &Chip8::opcodes0;
	MasterTable[0x1] = &Chip8::OP_1nnn;
	MasterTable[0x2] = &Chip8::OP_2nnn;
	MasterTable[0x3] = &Chip8::OP_3xkk;
	MasterTable[0x4] = &Chip8::OP_4xkk;
	MasterTable[0x5] = &Chip8::OP_5xy0;
	MasterTable[0x6] = &Chip8::OP_6xkk;
	MasterTable[0x7] = &Chip8::OP_7xkk;
	MasterTable[0x8] = &Chip8::opcodes8;
	MasterTable[0x9] = &Chip8::OP_9xy0;
	MasterTable[0xA] = &Chip8::OP_Annn;
	MasterTable[0xB] = &Chip8::OP_Bnnn;
	MasterTable[0xC] = &Chip8::OP_Cxkk;
	MasterTable[0xD] = &Chip8::OP_Dxyn;
	MasterTable[0xE] = &Chip8::opcodesE;
	MasterTable[0xF] = &Chip8::opcodesF;

	TableOf0[0x0] = &Chip8::OP_00E0;
	TableOf0[0xE] = &Chip8::OP_00EE;

	TableOf8[0x0] = &Chip8::OP_8xy0;
	TableOf8[0x1] = &Chip8::OP_8xy1;
	TableOf8[0x2] = &Chip8::OP_8xy2;
	TableOf8[0x3] = &Chip8::OP_8xy3;
	TableOf8[0x4] = &Chip8::OP_8xy4;
	TableOf8[0x5] = &Chip8::OP_8xy5;
	TableOf8[0x6] = &Chip8::OP_8xy6;
	TableOf8[0x7] = &Chip8::OP_8xy7;
	TableOf8[0xE] = &Chip8::OP_8xyE;

	TableOfE[0xE] = &Chip8::OP_Ex9E;
	TableOfE[0x1] = &Chip8::OP_ExA1;

	TableOfF[0x07] = &Chip8::OP_Fx07;
	TableOfF[0x0A] = &Chip8::OP_Fx0A;
	TableOfF[0x15] = &Chip8::OP_Fx15;
	TableOfF[0x18] = &Chip8::OP_Fx18;
	TableOfF[0x1E] = &Chip8::OP_Fx1E;
	TableOfF[0x29] = &Chip8::OP_Fx29;
	TableOfF[0x33] = &Chip8::OP_Fx33;
	TableOfF[0x55] = &Chip8::OP_Fx55;
	TableOfF[0x65] = &Chip8::OP_Fx65;

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
	opcode = ((memory[pc] << 8) | memory[pc +1]);	//Fetch opcode


	(this->*(MasterTable[(opcode & 0xF000) >> 12]))();


	if (delayt > 0)
	{
		--delayt;
	}

	if (soundt > 0)
	{
		--soundt;
	}

	
}


void Chip8::opcodes0()
{
	(this->*(TableOf0[opcode & 0x000F]))();
}

void Chip8::opcodes8()
{
	(this->*(TableOf8[opcode & 0x000F]))();
}

void Chip8::opcodesE()
{
	(this->*(TableOfE[opcode & 0x000F]))();
}

void Chip8::opcodesF()
{
	(this->*(TableOfF[opcode & 0x00FF]))();
}

void Chip8::OP_00E0()
{
	for (int i = 0; i < (64 * 32); i++)
		{
			display[i] = 0;
		}
					
			pc += 2;
}
void Chip8::OP_00EE()
{
	--sp;
	pc = stack[sp];
	pc += 2;
}
void Chip8::OP_1nnn()
{
	pc = (opcode & 0x0FFF);
}
void Chip8::OP_2nnn()
{
	stack[sp] = pc;
	sp++;
	pc = (opcode & 0x0FFF);
}
void Chip8::OP_3xkk()
{
	if (registers[(opcode & 0x0F00) >> 8]  == (opcode & 0x00FF))
	{
		pc += 4;
	}

	else pc += 2;
}
void Chip8::OP_4xkk()
{
	if (registers[(opcode & 0x0F00) >> 8]  != (opcode & 0x00FF))
	{
		pc += 4;
	}
	
	else pc += 2;

}
void Chip8::OP_5xy0()
{
	if (registers[(opcode & 0x0F00) >> 8] == registers[(opcode  & 0x00F0) >> 4] )
	{
		pc += 4;
	}
	
	else pc += 2;
}
void Chip8::OP_6xkk()
{
	registers[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
	pc += 2;
}
void Chip8::OP_7xkk()
{
	registers[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
	pc += 2;
}
void Chip8::OP_8xy0()
{
	registers[(opcode & 0x0F00) >> 8] = registers[(opcode & 0x00F0) >> 4];
	pc += 2;
}
void Chip8::OP_8xy1()
{
	registers[(opcode & 0x0F00) >> 8] |= registers[(opcode & 0x00F0) >> 4];
	pc += 2;
}
void Chip8::OP_8xy2()
{
	registers[(opcode & 0x0F00) >> 8] &= registers[(opcode & 0x00F0) >> 4];
	pc += 2;
}
void Chip8::OP_8xy3()
{
	registers[(opcode & 0x0F00) >> 8] ^= registers[(opcode & 0x00F0) >> 4];
	pc += 2;
}
void Chip8::OP_8xy4()
{
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
}
void Chip8::OP_8xy5()
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
void Chip8::OP_8xy6()
{
	registers[0xF] = (registers[(opcode & 0x0F00) >> 8] & 0x1);
	registers[(opcode & 0x0F00) >> 8] >>= 1;
	pc += 2;
}
void Chip8::OP_8xy7()
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
void Chip8::OP_8xyE()
{
	registers[0xF] = ((registers[(opcode & 0x0F00) >> 8] & 0x80) >> 7);
	registers[(opcode & 0x0F00) >> 8] <<= 1;
	pc += 2;

}
void Chip8::OP_9xy0()
{
	if (registers[(opcode & 0x0F00) >> 8] != registers[(opcode & 0x00F0) >> 4])
	{
		pc += 4;
	}

	else pc += 2;
}
void Chip8::OP_Annn()
{
	ireg = (opcode & 0x0FFF);
	pc += 2;
}
void Chip8::OP_Bnnn()
{
	pc = registers[0x0] + (opcode & 0x0FFF);

}
void Chip8::OP_Cxkk()
{
	registers[(opcode & 0x0F00) >> 8] = ((rand() % 255) & (opcode & 0x00FF));
	pc += 2;
}
void Chip8::OP_Dxyn()
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
void Chip8::OP_Ex9E()
{
	if(keys[registers[(opcode & 0x0F00) >> 8]] != 0)
	{
		pc += 4;
	}
	
	else pc += 2;
}
void Chip8::OP_ExA1()
{
	if(keys[registers[(opcode & 0x0F00) >> 8]] == 0)
	{
		pc += 4;
	}
	
	else pc += 2;
}
void Chip8::OP_Fx07()
{
	registers[(opcode & 0x0F00) >> 8] = delayt;
	pc += 2;
}
void Chip8::OP_Fx0A()
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
void Chip8::OP_Fx15()
{
	delayt = registers[(opcode & 0x0F00) >> 8];
	pc += 2;
}
void Chip8::OP_Fx18()
{
	soundt = registers[(opcode & 0x0F00) >> 8];
	pc += 2;
}
void Chip8::OP_Fx1E()
{
	ireg += registers[(opcode & 0x0F00) >> 8];
	pc += 2;
}
void Chip8::OP_Fx29()
{
	ireg = 0x50 + (registers[(opcode & 0x0F00) >> 8] * 0x5);
	pc += 2;
}
void Chip8::OP_Fx33()
{
	memory[ireg]     = registers[(opcode & 0x0F00) >> 8] / 100;
	memory[ireg + 1] = (registers[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[ireg + 2] = (registers[(opcode & 0x0F00) >> 8] % 100) % 10;					
	pc += 2;
}
void Chip8::OP_Fx55()
{
	for(long i = 0; i <= ((opcode & 0x0F00)>> 8); i++)
	{
		memory[ireg + i] = registers[i];
	}
						
	pc += 2;
}
void Chip8::OP_Fx65()
{
	for(uint8_t i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
	{
		registers[i] = memory[ireg + i];
	}
	
	pc += 2;
}