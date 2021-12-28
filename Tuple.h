#ifndef CSVPARSER_TUPLEPRINTER_H
#define CSVPARSER_TUPLEPRINTER_H

using namespace std;

template<typename Type, unsigned N, unsigned Last>
struct tuple_printer {

	static void print(ostream& out, const Type& value) {

		out << get<N>(value) << ", ";
		tuple_printer<Type, N + 1, Last>::print(out, value);
	}
};

template<typename Type, unsigned N>

struct tuple_printer<Type, N, N> {

	static void print(ostream& out, const Type& value) {
		out << get<N>(value);
	}
};

template<typename... Types>

ostream& operator<<(ostream& out, const tuple<Types...>& value) {
	out << "(";
	tuple_printer<tuple<Types...>, 0, sizeof...(Types) - 1>::print(out, value);
	out << ")\n";
	return out;
}

#endif