#pragma once

#include <vector>
#include <map>
#include <iostream>
#include <stack>

/*
	 AWSMBLR is a cool stack-based integer-only virtual machine and instruction set.
					Search for "TODO:" to see what's missing!

	--------------------------    awsmblr value set    ------------------------------

		<reg>	-- a numbered registry holding a *positive* integer expression;
				   when printed, will be represented by [R<number>]
				    example: R1, R2, R3

		<const> -- a constant integer expression
					example: 1234, 1, -5, 20, 0

		stack	-- a LIFO structure that can store integers

	----------------------  awsmblr conditions and notes  ---------------------------

	notes:
		[.puts-value]: this instruction adds one more value to the stack
		[.removes-top]: this instruction removes the top value off the stack

	conditions:
		[!source-exists]: this instruction should fail if its source registry doesn't exist
		[!non-empty-stack]: this instruction should fail if the stack is empty
		[!two-on-stack]: this instruction should fail if the stack doesn't have 2 elements
			(this is a fake condition that should be checked using [!non-empty-stack])
		[!valid-instruction-ptr]: this instruction should fail if it modifies the instruction
			pointer but the result is out of scope

	-------------------------- awsmblr instruction set ------------------------------

		PUSH <const> C			-- push the constant value C onto the stack
									[.puts-value]

		PUSH <reg> A			-- push the value of the registry A onto the stack
									[!source-exists][.puts-value]

		POP <reg> A				-- pop the value off the stack into registry A
									[!source-exists][!non-empty-stack][.removes-top]

		DUP						-- duplicates the last value on the stack
									[!non-empty-stack][.puts-value]

		ADD						-- take two values off the stack and put their sum onto the stack
									[!two-on-stack][.removes-two][.puts-value]

		SUB						-- take two values off the stack and put their difference onto the stack
									[!two-on-stack][.removes-two][.puts-value]

		JEQ <const> C			-- take two values off the stack and compare if they're equal. If they are
								   equal, jump C instructions away. Note: C might be negative.
									[!two-on-stack][!valid-instruction-ptr][.removes-two]

		JGT <const> C			-- take two values off the stack and compare if one is bigger than the other. 
								   If it is, jump C instructions away. Note: C might be negative.
									[!two-on-stack][!valid-instruction-ptr][.removes-two]
*/

#define RESET   "\033[0m"
#define RED     "\033[31m"

namespace awsmblr
{
	enum class EInstruction
	{
		PushC, 
		PushR,
		PopR, 
		Dup, 
		Add,
		Sub,
		Jeq, 
		Jgt,
	};

	enum class EStatus
	{
		Success,
		SourceRegistryDoesntExist,	// [!source-exists]
		RequiresStackToNotBeEmpty,	// [!non-empty-stack]
		InstructionPointerInvalid,	// [!valid-instruction-ptr]
	};

	struct RunResult
	{
		EStatus status;
		int ip;

		RunResult()
			: status{ EStatus::Success }
			, ip{ 0 }
		{}
	};

	using Instruction = std::vector<int>;
	using Registry = unsigned int;
	using Integer = int;

	using Program = std::vector<Instruction>;
	using Stack = std::stack<Integer>;
	using Environment = std::map<Registry, Integer>;

	namespace instruction_helpers
	{
		inline Instruction pushc(Integer c_) { return Instruction{ (int)EInstruction::PushC, c_ }; }
		inline Instruction pushr(Registry r_) { return Instruction{ (int)EInstruction::PushR, (int)r_ }; }
		inline Instruction popr(Registry r_) { return Instruction{ (int)EInstruction::PopR, (int)r_ }; }
		inline Instruction dup() { return Instruction{ (int)EInstruction::Dup }; }
		inline Instruction add() { return Instruction{ (int)EInstruction::Add }; }
		inline Instruction sub() { return Instruction{ (int)EInstruction::Sub }; }
		inline Instruction jeq(Integer c_) { return Instruction{ (int)EInstruction::Jeq, c_ }; }
		inline Instruction jgt(Integer c_) { return Instruction{ (int)EInstruction::Jgt, c_ }; }
	}

	// TODO: this is the function you need to fill in!
	RunResult Interpret(Program program_, Environment& environment_, Stack stack_);

	inline void PrintEnvironment(const Environment& environment_)
	{
		std::cout << "{";

		if(!environment_.empty()) std::cout << std::endl;

		int remaining = environment_.size();
		for (const auto& [k, v] : environment_)
		{
			--remaining;
			std::cout << "  \"R" << k << "\" : " << v;
			if (remaining > 0) std::cout << ",";
			std::cout << std::endl;
		}

		std::cout << "}" << std::endl;
	}

	inline void PrintError(const RunResult& result_)
	{
		std::cout << RED << "Error in instruction #" << (result_.ip + 1) << RESET << std::endl;

		switch (result_.status)
		{
		case EStatus::SourceRegistryDoesntExist:
			std::cout << "  " << RED << "Error: Source registry doesn't exist!" << RESET << std::endl;
			break;
		case EStatus::RequiresStackToNotBeEmpty:
			std::cout << "  " << RED << "Error: Requires stack to be non-empty!" << RESET << std::endl;
			break;
		case EStatus::InstructionPointerInvalid:
			std::cout << "  " << RED << "Error: Instruction pointer became invalid!" << RESET << std::endl;
			break;
		}
	}
};
