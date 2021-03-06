#pragma once


template <typename Iterator>
struct ErrorGrammarHandler
{
	template <typename, typename, typename>
	struct result { typedef void type; };

	ErrorGrammarHandler(Iterator f, Iterator l)
		: first(f), last(l) {}

	template <typename Message, typename What>
	void operator()(
		Message const& message,
		What const& what,
		Iterator err_pos) const
	{
		int line;
		Iterator line_start = getPos(err_pos, line);
		if (err_pos != last)
		{
			std::cout << message << what << " line " << line << ':' << std::endl;
			std::cout << getLine(line_start) << std::endl;
			for (; line_start != err_pos; ++line_start)
				std::cout << ' ';
			std::cout << '^' << std::endl;
		}
		else
		{
			std::cout << "Unexpected end of file. ";
			std::cout << message << what << " line " << line << std::endl;
		}
	}

	Iterator getPos(Iterator err_pos, int& line) const
	{
		line = 1;
		Iterator i = first;
		Iterator line_start = first;
		while (i != err_pos)
		{
			bool eol = false;
			if (i != err_pos && *i == '\r') // CR
			{
				eol = true;
				line_start = ++i;
			}
			if (i != err_pos && *i == '\n') // LF
			{
				eol = true;
				line_start = ++i;
			}
			if (eol)
				++line;
			else
				++i;
		}
		return line_start;
	}

	std::string getLine(Iterator err_pos) const
	{
		Iterator i = err_pos;
		// position i to the next EOL
		while (i != last && (*i != '\r' && *i != '\n'))
			++i;
		return std::string(err_pos, i);
	}

	Iterator first;
	Iterator last;
	std::vector<Iterator> iters;
};

