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



struct my_action {
	my_action(uint32_t line) :_line(line) {}
	void operator()(const char * first, const char * last)const
	{
		std::string str(first, last);
		std::cout << "my_action:" << str << ",source line:" << _line << endl;
	}
private:
	uint32_t _line;
};

namespace client1
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	///////////////////////////////////////////////////////////////////////////////
	//  Our calculator grammar
	///////////////////////////////////////////////////////////////////////////////
	template <typename Iterator>
	struct calculator : qi::grammar<Iterator, ascii::space_type>
	{
		calculator() : calculator::base_type(expression)
		{
			qi::uint_type uint_;

			expression =
				term
				>> *(('+' >> term)
					| ('-' >> term)
					)
				;

			term =
				factor
				>> *(('*' >> factor)
					| ('/' >> factor)
					)
				;

			factor =
				uint_
				| '(' >> expression >> ')'
				| ('-' >> factor)
				| ('+' >> factor)
				;
		}

		qi::rule<Iterator, ascii::space_type> expression, term, factor;
	};
}

namespace client {


	enum ERR_CODE{
		ERR_COLON = 1000,
		ERR_SEMI_COLON,
		ERR_KEY,
		ERR_RIGHT_PAREN,
		ERR_RSQUO,
	};
	


	

		

	

	
// 	struct ColorSymbols :qi::symbols<char, uint32_t> {
// 		ColorSymbols() {
// 			add("red", RGB2INT(255, 0, 0))
// 				("green", RGB2INT(0, 255, 0))
// 				("blue", RGB2INT(0, 0, 255))
// 				("black", RGB2INT(0, 0, 0))
// 				("white", RGB2INT(255, 255, 255))
// 				("gray", RGB2INT(128, 128, 128))
// 				("yellow", RGB2INT(255, 255, 0))
// 				("cyan", RGB2INT(0, 255, 255))
// 				("magenta", RGB2INT(255, 0, 255))
// 				("brown", RGB2INT(128, 0, 0))
// 				;
// 		}
// 	};
// 	struct LineTypeSymbols :qi::symbols<char, uint8_t> {
// 		LineTypeSymbols() {
// 			add("stick", 0)
// 				("colorstick", 1)
// 				("volstick",2)
// 				("linestick",3)
// 				("crossdot",4)
// 				("circledot",5)
// 				("pointdot",6)
// 				("stick3d",7)
// 				("color3d",8)
// 				("dotline",9)
// 				("dashline",10)
// 				;
// 		}
// 	};

// 	struct error_report_parser {
// 		error_report_parser(uint32_t err) : _err(err) {}
//  		typedef nil_t result_t;
// 
// 		template <typename ScannerT>
// 		int operator()(ScannerT const& scan, result_t& /*result*/) const
// 		{
// 			cerr << _err << endl;
// 			return 0;
// 		}
// 
// 	private:
// 		uint32_t _err;
// 		uint32_t _line;
// 	};

// 	struct error_handler
// 	{
// 		error_handler(uint32_t line) :_line(line) {}
// 		template <typename ScannerT, typename ErrorT>
// 		error_status<>
// 			operator()(ScannerT const& scan, ErrorT const& error) const
// 		{
// 			cout << "exception caught, where:" << error.where - scan.first << ",code:" << error.descriptor
// 				<< ",source line:" << _line << endl;
// 			return error_status<>(error_status<>::rethrow);
// 		}
// 	private:
// 		uint32_t _line;
// 	};

	template<typename Iterator>
	struct Formular : public qi::grammar<Iterator, ascii::space_type>
	{
// 		
// 		template <typename ScannerT>
// 		struct definition
// 		{
// #define EXPECT(err) assertion<int>(err)
// 			
// 			guard<int>      my_guard;
// 			fun_sym fun_sym_p;
// 			color_sym color_sym_p;
// 			line_type_sym line_type_sym_p;
// 
// 			typedef rule<ScannerT> rule_type;
// 			rule_type string_p, word_p;
// 			rule_type factor, term, op_add_subtract, op_compare1, op_compare2, op_bit_and, op_bit_e, op_bit_or, op_and, op_or;
// 			rule_type var_name, comment, stmt_end, paren_end;
// 			rule_type rlStart;
// 			
// 			rule_type input_stmt, input_param_data, input_param_def, input_param_list;
// 			rule_type var_stmt, var_declare, var_def, var_list;
// 			rule_type main_stmt, simple_stmt, assign_stmt, out_stmt;
// 			rule_type func_stmt, func_id, option_list;
// 			rule_type exp_stmt, exp_list;
// 			rule_type attr_modifier, color_attr, thick_attr, layer_attr, precis_attr, align_attr, valign_attr, line_attr, move_attr;
// 
// 			const rule_type& start() const { return rlStart; }
// 			
// 			definition(calculator const& self)
// 			{
// 				color_attr = lexeme_d[as_lower_d["color"] >> (as_lower_d[color_sym_p] | repeat_p(6)[as_lower_d[chset_p("0-9a-f")]])];
// 				thick_attr = lexeme_d[as_lower_d["linethick"] >> chset_p("0-7")];
// 				layer_attr = lexeme_d[as_lower_d["layer"] >>chset_p("0-7")];
// 				precis_attr = lexeme_d[as_lower_d["precis"] >> chset_p("0-7")];
// 				align_attr = lexeme_d[as_lower_d["align"] >> chset_p("0-2")];
// 				valign_attr = lexeme_d[as_lower_d["valign"] >> chset_p("0-2")];
// 				line_attr = lexeme_d[as_lower_d["hideval"]];
// 				move_attr = lexeme_d[as_lower_d["move"] >> +chset_p("0-9")];
// 
// 				attr_modifier = color_attr
// 					| thick_attr
// 					| layer_attr
// 					| precis_attr
// 					| align_attr
// 					| valign_attr
// 					| line_attr
// 					| move_attr
// 					| line_type_sym_p
// 					;
// 
// 				word_p = lexeme_d[as_lower_d[(chset_p("a-z\x80-\xff")) >> *(chset_p("a-z0-9\x80-\xff_"))]];
// 				//字符串解析，单引号
// 				string_p = my_guard(ch_p('\'') >> *(~chset_p("\'\n"))>>EXPECT(ERR_RSQUO)(ch_p('\'')))[error_handler(__LINE__)];
// 
// 				//预定义函数
// 				func_id = word_p & fun_sym_p;
// 
// 				//变量名，不区分大小写，除去关键字，支持中文
// 				var_name = my_guard(EXPECT(ERR_KEY)(word_p - fun_sym_p))[error_handler(__LINE__)];
// 					//语句结束 分号结束
//  					stmt_end = EXPECT(ERR_SEMI_COLON)(ch_p(';'));
// 					//右括号
//  					paren_end = EXPECT(ERR_RIGHT_PAREN)(ch_p(')'));
// 
// 					//程序开始，INPUT语句，定义输入参数
// 					input_param_data = real_p%',';
// 					input_param_def = my_guard(var_name >> '(' >> input_param_data >> paren_end)[error_handler(__LINE__)];
// 					input_param_list = input_param_def % ',';
// 					input_stmt = my_guard(as_lower_d["input"] >> ':' >> input_param_list >> stmt_end)[error_handler(__LINE__)]
// 						| my_guard(as_lower_d["input"] >> EXPECT(ERR_COLON)(ch_p(':')))[error_handler(__LINE__)];
// 					
// 					//变量列表
// 					var_def = var_name >> '=' >> real_p
// 						| var_name >> '=' >> string_p;
// 					var_list = var_def%',';
// 					var_declare = as_lower_d["variable"] >> ':' >> var_list >> stmt_end;
// 					var_stmt = +var_declare;
// 					
// 					//注释
// 					comment = comment_p("/*", "*/")
// 						| comment_p("//")
// 						| comment_p("{", "}");
// 
// 					exp_list = list_p(exp_stmt, ch_p(','));
// 
// 					option_list =confix_p('(', !exp_list, ')');
// 
// 					func_stmt =	func_id[my_action(__LINE__)] >> option_list  >> stmt_end;					
// 
// 					factor = longest_d[real_p						
// 						| (func_stmt[my_action(__LINE__)])
// 						| (var_name[my_action(__LINE__)])
// 						
// //  						| '(' >> exp_stmt >> ')'
// // 						| '~'>>factor
// // 						| !(ch_p('+') | '-') >> factor	
// 						];
// 
// 					term = factor >>
// 						*(('*' >> factor) | ('/' >> factor) | ('%' >> factor));
// 					op_add_subtract = term >>
// 						*(('+' >> term) | ('-' >> term));
// 
// 					op_compare1 = op_add_subtract >> *((ch_p('>') >> op_add_subtract)
// 						| (str_p(">=") >> op_add_subtract)
// 						| (ch_p('<') >> op_add_subtract)
// 						| (str_p("<=") >> op_add_subtract)
// 						);
// 					op_compare2 = op_compare1 >> *((str_p("==") >> op_compare1) | (str_p("!=") >> op_compare1));
// 					op_bit_and = op_compare2 >> *(ch_p('&') >> op_compare2);
// 					op_bit_e = op_bit_and >> *(ch_p('^') >> op_bit_and);
// 					op_bit_or = op_bit_e >> *(ch_p('|') >> op_bit_e);
// 					op_and = op_bit_or >> *(as_lower_d["and"] >> op_bit_or);
// 					op_or = op_and >> *(as_lower_d["or"] >> op_and);
// 					exp_stmt = op_or[my_action(__LINE__)];
// 
// 					assign_stmt = var_name >> str_p(":=") >> exp_stmt>>stmt_end;
// 					out_stmt = var_name >> ch_p(':') >> exp_stmt >> stmt_end
// 						| var_name >> ch_p(':') >> exp_stmt >> ',' >> attr_modifier%',' >> stmt_end
// 						;
// 
// 					simple_stmt =
// 						out_stmt
// 						| assign_stmt
// 						| ch_p(';')
// 						;
// 					rlStart = 
// 						func_stmt
// // 						comment
// //  						| input_stmt
// // 						| option_list
// //   						| var_stmt
// // 						| simple_stmt
// 						| end_p
// 						;
// 			}
// 		};
// 		
		Formular():
			Formular::base_type(startRule)
		{
			

			using ascii::char_;
			using ascii::no_case;
			using qi::repeat;
			using qi::lexeme;
			using qi::lit;
			using qi::alnum;
			using qi::alpha;
			using qi::raw;
			using boost::lambda::_1;
			using qi::float_;


		
// 		
// 			colorAttr = lexeme[no_case["color"] >> (no_case[colorSymbols] | repeat(6)[no_case[char_("0-9a-f")]])];
// 			thickAttr = lexeme[no_case["linethick"] >> char_("0-7")];
// 			layerAttr = lexeme[no_case["layer"] >> char_("0-7")];
// 			preciseAttr = lexeme[no_case["precise"] >> char_("0-7")];
// 			alignAttr = lexeme[no_case["align"] >> char_("0-2")];
// 			valignAttr = lexeme[no_case["valign"] >> char_("0-2")];
// 			lineAttr = lexeme[no_case["hideval"]];
// 			moveAttr = lexeme[no_case["move"] >> +char_("0-9")];

			
// 			stringRule = char_('\'') >> *(lit("\\\'") | char_ - char_("\'")) >> char_('\'');
// 			quoteRule = char_('\"') >> *(lit("\\\"") | char_ - char_("\"")) >> char_('\"');

			expr = logicalOrExpr.alias();

			

			startRule = expr;
		}
		qi::symbols<char> keywords;
		typedef qi::rule<Iterator, ascii::space_type> rule_type;

		rule_type startRule, expr;

		rule_type varNameRule, wordRule;

// 		LineTypeSymbols lineTypeSymbols;
// 		ColorSymbols colorSymbols;
		rule_type colorAttr, thickAttr, layerAttr, preciseAttr, alignAttr, valignAttr, lineAttr, moveAttr;
		rule_type stringRule;
		rule_type quoteRule;

		rule_type unaryExpr, multiExpr, additiveExpr, relationalExpr, equalityExpr, bitAndExpr, bitXorExpr,bitOrExpr;
		rule_type logicalAndExpr, logicalOrExpr;

		rule_type mainExpr;

		
	

// 		fun_sym _fun;


		
		
		
	};

	
}


///////////////////////////////////////////////////////////////////////////////
//  Main program
///////////////////////////////////////////////////////////////////////////////
// #include <boost/spirit/include/classic.hpp>
// #include <boost/spirit/include/classic_parser.hpp>
// using namespace boost::spirit::classic;

#include "StatementGrammar.hpp"
#include "ExpressionGrammar.hpp"
#include "ErrorGrammarHandler.hpp"
#include "ast.hpp"

string mainPath;

struct compiler :boost::static_visitor<> {
	void operator()(ast::Nil) const{
		cout << "nil\n" << endl;
		//assert(0);
	}
	void operator()(double x)const {
		cout << "double:" << x << endl;
	}
	void operator()(const string& x)const {
		cout << "string:"<< x << endl;
	}
	void operator()(uint32_t id) const{
		cout << "uint32_t:" << id << endl;
	}
	void operator()(SFI id) const {
		cout << "SFI:" << id << endl;
	}
	void operator()(char id) const {
		cout << "char:" << id << endl;
	}
	void operator()(ast::ColorModifer const& modifer)const {
		cout << "type:ColorModifer"
// 			<<" "
// 			<<color.value
			<<"\n";
		boost::apply_visitor(*this, modifer.operand);
	}
	void operator()(ast::TypeModifer const& typeModifer) {
		cout << "type:typeModifer:" << typeModifer.type << " value:"<<typeModifer.value<<"\n";
		
	}
	void operator()(ast::InputStmt const& inputStmt)const {
		cout << "type:InputStmt\n"
			<< "inputId:" << inputStmt.name
			<< "params:" << inputStmt.params.size()
			<< endl;
	}

	void operator()(ast::VariableStmt const& variantStmt)const {
		cout << "type:VariableStmt"
			<< variantStmt.name
			<< endl;
		for (auto e : variantStmt.operands) {
			boost::apply_visitor(*this, e);
		}
	}

	void operator()(ast::Unary const& unary)const {
		cout << "type:Unary\n"
			<< "sfi:" << unary.sfi << "\n"
			;
		boost::apply_visitor(*this, unary.operand);
	}
	void operator()(ast::Operation const& operation)const {
		cout << "type:Operation\n"
			<< "sfi:" << operation.sfi << "\n"
			;
		boost::apply_visitor(*this, operation.operand);
	}
	void operator()(ast::FunctionCall const& functionCall)const {
		cout << "type:FunctionCall\n"
			<< "name:" << functionCall.name << "\n"
			;
		//boost::apply_visitor(*this, operation.operand);
	}
	void operator()(ast::Expression const& expression)const {
		cout << "type:Expression\n"
			/*<< "sfi:" << operation.sfi << "\n"*/
			;
		boost::apply_visitor(*this, expression.first);
		cout << "\n";
		for (auto e : expression.operationLst) {
			cout << "sub:Operation:"  << "\n";
			boost::apply_visitor(*this, e);
		}
	}

// 	void operator()(ast::ModiferStmtLst const& modiferStmtList) {
// 		cout << "ast::ModiferStmtLst:"
// 			<< modiferStmtList.operands.size()
// 			<< "\n";
// 		for (auto e : modiferStmtList.operands) {
// 			boost::apply_visitor(*this, e);
// 			cout << "\n";
// 		}
// 	}

};

int
main(int argc, char* argv[])
{
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

	

	auto parseFormula = [](const string& text) {
		try {


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
			//StatementGrammar<string::const_iterator> program(errorHandler);
			ExpressionGrammar<string::const_iterator> program;
			
			//ast::ModiferStmtLst astResult;
			//vector<ast::CalcOperand> astResult;
			ast::Expression astResult;
			
// 			SkipperGrammar<string::const_iterator> skipper;

			
 			ascii::space_type space;

			bool info = qi::phrase_parse(iter, end, program, space, astResult);

			if (info && iter == end)
			{
				compiler compiler_;

// 				for (auto e : v) {
// 					compiler_(e);
// 
// 					cout << "\n";
// 					
// 					
// 				}
 				compiler_(astResult);
//				astResult.apply_visitor(compiler_);

				cout << "-------------------------\n";
				cout << "Parsing succeeded\n";
				cout << "-------------------------\n";
			}
			else
			{

				cout << "-------------------------\n";
				cout << "Parsing failed ret: " << info << "\n";
				cout << "stopped at: \": " << string(iter, end) << "\"\n";
				cout << "-------------------------\n";
			}
		}
		catch (std::exception& e) {
			cout << "std::exception:" << e.what() << endl;
		}
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

