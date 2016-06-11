#include <cstdint>
#include <vector>
#include <iterator>
#include <cassert>
#include <iostream>
#include <algorithm>

struct var_uint32
{

static void encode(std::uint32_t value, std::vector<std::uint8_t>& output)
{
	while (value >= 0x80)
	{
		output.push_back(static_cast<std::uint8_t>(value | 0x80));
		value >>= 7;
	}
	output.push_back(static_cast<std::uint8_t>(value));
}

static std::uint32_t decode(std::uint8_t*& in)
{
	std::uint32_t result = 0;
	std::uint32_t tmp = 0;
	for (int i=0; i<4; ++i)
	{
		tmp = *in++;
		result += (tmp << i * 7);
		if (not (tmp & 0x80)) break;
		result -= (0x80 << i * 7);
	}
	return result;
}

};

int main()
{
	std::vector<std::uint32_t> data {100, 200, 300, 400, 500};
	
	std::vector<std::uint8_t> encoded;

	std::for_each(data.begin(), data.end(), 
		[&encoded](auto v) { var_uint32::encode(v, encoded); }
	);

	//std::copy(encoded.begin(), encoded.end(), std::ostream_iterator<int>(std::cout, ", "));

	auto p = encoded.data();
	std::vector<std::uint32_t> decoded(data.size());
	for (int i=0; i<decoded.size(); ++i)
	{
		auto x = var_uint32::decode(p);
		decoded[i] = x;
		assert(x == data[i]);
	}
	
	//std::cout << std::endl;
	//std::copy(decoded.begin(), decoded.end(), std::ostream_iterator<int>(std::cout, ", "));

	return 0;
}
