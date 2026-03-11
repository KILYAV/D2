#include <iostream>

constexpr unsigned in = 0;

constexpr unsigned expr(const unsigned in) {
	return in;
}

using tuple = std::tuple<int>;

template<unsigned temp>
struct test {
	static constexpr unsigned value = expr(temp);
	static constexpr unsigned Value(const unsigned in) {
		if (std::is_constant_evaluated())
			return Size<0>();
		else
			return in;
	}
	template<unsigned index>
	static constexpr unsigned Size() {
		return sizeof(std::tuple_element_t<index, tuple>);
	}
};

int main() {
	std::cout << test<in>::value << std::endl;
	std::cout << test<in>::Value(in) << std::endl;
}