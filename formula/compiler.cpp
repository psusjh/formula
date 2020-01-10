#include "stdafx.h"
#include "compiler.h"
#include "Code.h"

void align_print(int depth) {
	while (depth-- > 0) {
		cout << "\t";
	}
}

compiler::compiler(shared_ptr<Code>& code)
{
	this->codeptr = code;
	depth = 0;
}

compiler::compiler(shared_ptr<Code>&code, int depth)
{
	this->codeptr = code;
	this->depth = depth;
}

compiler::~compiler()
{

}





