#include <CPU.h>


//Push Register
class PUSH_1_OPCODE
{
private:
    CPU *cpu;

public:
    PUSH_1_OPCODE();
    void execute(uint8_t opcode);
    ~PUSH_1_OPCODE();
};

