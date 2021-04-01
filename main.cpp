
#include "awsmblr.h"

using namespace awsmblr;
using namespace instruction_helpers;

// undefine this if you want to test the real deal
#define EASY

// undefine this to stop telling you what you're missing
#define TEST

int main()
{
	// example of execution:
#ifdef EASY
	auto program = Program{
		pushc(10),	// stack: 10 |
		pushc(22),	// stack: 10 | 22 |
		add(),		// stack: 32 |
		add(),		// stack: 32 |
		pushc(5),	// stack: 32 | 5 |
		sub(),		// stack: 27 |
		popr(3)		// R3 = 27, stack: <empty>
	};
#else
	auto program = Program{
		pushc(10),	// stack: 10 |
		pushc(22),	// stack: 10 | 22 |
		pushc(2),	// stack: 10 | 22 | 2 |
		add(),		// stack: 10 | 44 |
		add(),		// stack: 54 |
		popr(1),	// R1 = 54, stack: <empty>				<----------------+
		pushc(5),	// R1 = 54, stack: 5 |									 |
		popr(2),	// R1 = 54, R2 = 5, stack: <empty>						 |
		pushr(1),	// R1 = 54, R2 = 5, stack: 54 |							 |
		pushr(2),	// R1 = 54, R2 = 5, stack: 54 | 5 |						 |
		sub(),		// R1 = 54, R2 = 5, stack: 49 |							 |
		dup(),		// R1 = 54, R2 = 5, stack: 49 | 49 |					 |
		pushc(0),	// R1 = 54, R2 = 5, stack: 49 | 49 | 0 |				 |
		jgt(-8),	// R1 = 54, R2 = 5, stack: 49							 |
					//	because 49 > 0, jump -8 instructions back to... -----+
					//  but now when we popr(1), R1 becomes 49 instead of 54!
					// eventually, we'll get here:
		popr(3)		// R3 = ?? <--- mystery box!
	};
#endif //EASY

	auto env = Environment();
	auto stack = Stack();

	auto result = Interpret(program, env, stack);

	switch (result.status)
	{
	case EStatus::Success: 
		std::cout << "Environment:\n" << std::endl;
		PrintEnvironment(env); 

#ifdef TEST
		if (env.find(3) == env.end())
		{
			std::cout << "\nWarning: Seems that we don't have a value in R3. This is odd..." << std::endl;
		}
#endif //TEST
		break;
	default: 
		PrintError(result); 
		break;
	}

	return 0;
}
