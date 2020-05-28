// formula.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>


#include <boost/spirit/include/qi.hpp>
#include <boost/lambda/lambda.hpp>

using namespace std;

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;


#include "StatementGrammar.hpp"
#include "ExpressionGrammar.hpp"
#include "ErrorGrammarHandler.hpp"

#include "compiler.h"
#include "Code.h"
#include "InternalFunction.h"
string mainPath;



int
main(int argc, char* argv[])
{
	int i = 1 + -1;
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "\t\tformula Parser\n\n";
	std::cout << "/////////////////////////////////////////////////////////\n\n";
	std::cout << "Type a char ...\n\n[q or Q] to quit\n[e or E] load .\\test.txt to exec Formula Parse\n\n";
	
	for (int i = strlen(argv[0]) - 1; i >= 0; i--) {
		if (argv[0][i] == '\\') {
			mainPath.insert(0, argv[0], i+1);
			break;
		}
	}
	InternalFunction::initCatalog();
	InternalFunction::initInternalFunction();
	

	auto parseFormula = [](const string& text) {
		//try {


			//client::Formular<string::const_iterator> program;
			//client1::calculator<string::const_iterator> program;
			cout << "formula text:\n"
				<< "-------------------------\n\n"
				<< text
				<< "\n\n";

			string::const_iterator iter = text.begin();
			string::const_iterator end = text.end();
			std::string result;
			ErrorGrammarHandler<string::const_iterator> errorHandler(iter, end);
			StatementGrammar<string::const_iterator> program(errorHandler);
//			ExpressionGrammar<string::const_iterator> program;
			
			//ast::ModiferStmtLst astResult;
			//vector<ast::CalcOperand> astResult;
//			list<ast::Expression> astResult;
 			ast::StatementList astResult;
//			string astResult;
			
 			SkipperGrammar<string::const_iterator> skipper;

			
//  			ascii::space_type skipper;

			bool info = qi::phrase_parse(iter, end, program, skipper, astResult);

			if (info && iter == end)
			{
 				
// 				compiler_(astResult);
//				astResult.apply_visitor(compiler_);

				cout << "-------------------------\n";
				cout << "Parsing succeeded\n";
				cout << "-------------------------\n";
				compiler compiler_(make_shared<Code>());
				for (auto e : astResult) {
					e.apply_visitor(compiler_);
				}
				compiler_.printCode();
			}
			else
			{

				cout << "-------------------------\n";
				cout << "Parsing failed ret: " << info << "\n";
				cout << "stopped at: \"" << string(iter, end) << "\"\n";
				cout << "-------------------------\n";
			}
// 		}
// 		catch (std::exception& e) {
// 			cout << "std::exception:" << e.what() << endl;
// 		}
	};
	
	string a = "\x80-\xff";
	string str;
	while (true)
	{
		char c = getchar();
		if (c == 'q' || c == 'Q') {
			
			break;
		}
		if (c == 'e' || c == 'E') {
			string filePath = mainPath + "test.txt";
			FILE* f = fopen(filePath.c_str(), "r");
			if (nullptr == f) {
				cout << "-------------------------\n";
				cout << "load ./test.txt failed\n";
				continue;
			}
			fseek(f, 0, SEEK_END);
			long fileSize = ftell(f);
			char* buffer = new char[fileSize+1];
			memset(buffer, 0, fileSize+1);
			fseek(f, 0, SEEK_SET);
			fread(buffer, fileSize, 1, f);
			parseFormula(buffer);
			delete[]buffer;
			fclose(f);
			
			cout << "\n-------------------------\n";
			cout << "Please type [q or Q] to quit\n[e or E] load .\\test.txt to exec Formula Parse\n";
			getchar();
			continue;
		}

		str.append(&c, 1);		
	}

	

	std::cout << "Bye... :-) \n\n";
	system("pause");
	return 0;
}

