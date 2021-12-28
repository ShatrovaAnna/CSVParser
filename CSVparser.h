#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include "EmptyStringException.h"
#include "UnknownTypeException.h"
#include "LineOutOfRangeException.h"

using namespace std;

template<typename... Types>

class CSVparser {

	ifstream& in;
	string line;
	int lastLineIdx;
	int column = 0;
	int row = 0;
	char columnSeparator;
	char rowSeparator;
	char shieldingSymbol;
	bool meetShieldingSymbol;
	long beginningOfFile;

public:

	CSVparser(ifstream& _in, int countOfSkips, char _columnSeparator, char _rowSeparator, char _shieldingSymbol)
		: in(_in), columnSeparator(_columnSeparator), rowSeparator(_rowSeparator), shieldingSymbol(_shieldingSymbol) {

		skipFirstLines(countOfSkips);
		beginningOfFile = in.tellg();
	}

private:

	void skipFirstLines(int countOfSkips) {
		for (int i = 0; i < countOfSkips; i++) {
			string _line;
			getline(in, _line);
		}
	}

private:

	class Iterator {

		typedef typename tuple<Types...> value;


	public:
		Iterator(CSVparser<Types...>* _parser, long _pos) : parser(_parser), pos(_pos) {
			if (_pos == -1) {
				return;
			}
			if (parser->beginningOfFile == 0) {
				parser->in.clear();
				parser->in.seekg(parser->beginningOfFile, ios::beg);
			}
			else {
				parser->in.clear();
				parser->in.seekg(parser->beginningOfFile - 1, ios::beg);
			}
			try {
				parseCell<0>(tuple);
			}
			catch (LineOutOfRangeException& exp) {
				parser->row++;
				parser->column = 0;
			}
			pos = parser->in.tellg();

		};

		bool operator!=(Iterator const& other) const {
			return pos != other.pos;
		}
		bool operator==(Iterator const& other) const {
			return pos == other.pos;
		}
		value& operator*() {
			return tuple;
		}

		Iterator& operator++() {
			pos = parser->in.tellg();
			try {
				parseCell<0>(tuple);
			}
			catch (LineOutOfRangeException& exp) {
				parser->row++;
				parser->column = 0;
			}
			return *this;
		}

	private:
		CSVparser<Types...>* parser;
		value tuple;
		long pos;

	private:
		template<size_t N>
		typename enable_if<(N < sizeof...(Types))>::type parseCell(tuple<Types...>& cells) {
			string cell;
			try {
				cell = readCell();
			}
			catch (exception& ex) {
				return;
			}
			pushValue<N>(cells, cell);
			parser->column++;
			parseCell<N + 1>(cells);

		}

		template<size_t n>
		[[noreturn]] typename enable_if<(n >= sizeof...(Types))>::type parseCell(tuple<Types...>& cells) {
			throw LineOutOfRangeException("column: " + to_string(parser->column) +
				", row: " + to_string(parser->row) + " - line out of range");
		}

		template<size_t N>
		void pushValue(tuple<Types...>& cells, const string& str) {
			try {
				istringstream stream(str);
				stream.exceptions(istringstream::failbit);
				stream >> get<N>(cells);
			}
			catch (istringstream::failure& ex) {
				throw UnknownTypeException("column: " + to_string(parser->column)
					+ ", row: " + to_string(parser->row) + " - UnknownType");
			}
		}

		string readCell() {
			if (parser->line.empty()) {
				getline(parser->in, parser->line, parser->rowSeparator);
				parser->lastLineIdx = 0;
				if (parser->line.empty()) { 
					throw EmptyStringException("line is empty"); 
				}
			}
			string cell;
			parser->meetShieldingSymbol = false;
			for (int i = parser->lastLineIdx; i <= parser->line.size(); i++) {
				if (i == parser->line.size()) {
					parser->line = "";
					return cell;
				}
				else if (parser->line[i] == parser->columnSeparator) {
					if (parser->meetShieldingSymbol) {
						cell.push_back(parser->line[i]);
					}
					else {
						parser->lastLineIdx = i + 1;
						return cell;
					}
				}
				else if (parser->line[i] == parser->shieldingSymbol && parser->meetShieldingSymbol) {
					parser->meetShieldingSymbol = false;

				}
				else if (parser->line[i] == parser->shieldingSymbol && !parser->meetShieldingSymbol) {
					parser->meetShieldingSymbol = true;

				}
				else {
					cell.push_back(parser->line[i]);
				}
			}
		}
	};

public:
	Iterator begin() {
		return Iterator(this, 0);
	}

	Iterator end() {
		return Iterator(this, -1);
	}

	friend class Iterator;
};

template<class charT, class trT, class T>
typename enable_if<(is_same_v<T, string>), typename basic_istringstream<charT, trT>&>::type
operator>>(basic_istringstream<charT, trT>& st, T& t) {
	t = st.str();
	return st;
}

#endif