#pragma once

#include <stdlib.h>
#include <cstdint>
#include <fstream>

const unsigned int MEMORY = 4096;
const unsigned int DISPLAY = 64 * 32;
const unsigned int STACK = 16;

class Chip8
{       
    public:
        Chip8();                          //Initializes the chip8
        uint16_t keys[16]{};              //The keypad
        uint32_t  display[DISPLAY]{};     //Monochrome display

    
        uint8_t memory[MEMORY]{};
        uint8_t registers[16]{};          //From V0 to VF
        uint16_t ireg{};                  //Only the rightmost 12 bits are used
        uint32_t delayt{};                 //decrement at 60hz
        uint32_t soundt{};                 //decrement at 60hz
        uint16_t pc{};                          //Program counter
        uint16_t sp{};                          //Stack pointer
        uint16_t stack[STACK]{};                //The stack
        uint16_t opcode;

        void LoadRom(char const* filename);
        void EmulationCycle();

        //Opcode functions
        
      /*void OP_00E0();                   //CLS                 - Clears the display
        void OP_00EE();                   //RET                 - Return from subroutine
        void OP_1nnn();                   //JP addr             - Jump to location nnn
        void OP_2nnn();                   //CALL addr           - Call subroutine at nnn
        void OP_3xkk();                   //SE Vx byte          - Skip next instruction if Vx = kk
        void OP_4xkk();                   //SNE Vx byte         - Skip next instruction if Vx != kk
        void OP_5xy0();                   //SE Vx Vy            - Skip next instruction if Vx = Vy
        void OP_6xkk();                   //LD Vx byte          - Set Vx = kk
        void OP_7xkk();                   //ADD Vx byte         - Set Vx = Vx + kk
        void OP_8xy0();                   //LD Vx Vy            - Set Vx = Vy
        void OP_8xy1();                   //OR Vx Vy            - Set Vx = Vx | Vy
        void OP_8xy2();                   //AND Vx Vy           - Set Vx = Vx & Vy
        void OP_8xy3();                   //XOR Vx Vy           - Set Vx = Vx ^ Vy
        void OP_8xy4();                   //ADD Vx Vy           - Set Vx = Vx + Vy, set VF = carry
        void OP_8xy5();                   //Sub Vx Vy           - Set Vx = Vx - Vy, set VF = NOT borrow
        void OP_8xy6();                   //SHR Vx {, Vy}       - Set Vx = Vx SHR 1
        void OP_8xy7();                   //SUBN Vx Vy          - Set Vx = Vy - Vx, set VF = NOT borrow
        void OP_8xyE();                   //SHL Vx {, Vy}       - Set Vx = Vx SHL 1     
        void OP_9xy0();                   //SNE Vx Vy           - Skip next instruction if Vx != Vy
        void OP_Annn();                   //LD I addr           - Set I = nnn
        void OP_Bnnn();                   //JP V0 addr          - Jump to location nnn + V0
        void OP_Cxkk();                   //RND Vx byte         - Set Vx = random byte AND kk
        void OP_Dxyn();                   //DRW Vx Vy nibble    - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
        void OP_Ex9E();                   //SKP Vx              - Skip next instruction if key with the value of Vx is pressed
        void OP_ExA1();                   //SKNP Vx             - Skip next instruction if key with the value of Vx is not pressed
        void OP_Fx07();                   //LD Vx DT            - Set Vx = Delay timer value
        void OP_Fx0A();                   //LD Vx k             - Wait for a key press, store the value of the key in Vx
        void OP_Fx15();                   //LD DT Vx            - Set delay timer = Vx
        void OP_Fx18();                   //LD ST Vx            - Set sound timer = Vx
        void OP_Fx1E();                   //ADD I Vx            - Set I = I + Vx
        void OP_Fx29();                   //LD F Vx             - Set I = location of sprite for digit Vx
        void OP_Fx33();                   //LD B Vx             - Store BCD representation of Vx in memory locations I, I+1 and I+2
        void OP_Fx55();                   //LD [I] Vx           - Store registers V0 through Vx in memory starting at location I
        void OP_Fx65();                   //LD Vx [I]           - Read registers V0 through Vx from memory starting at location I */
};