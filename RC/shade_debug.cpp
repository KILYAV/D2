#include <set>

#include "domain.h"

auto LoadFile(const std::filesystem::path& path) {
	std::string source;
	std::ifstream file{ path, std::ios::binary };

	auto size{ std::filesystem::file_size(path) };
	source.resize(size);
	file.read(source.data(), size);
	return source;
}
auto LoadSource(const std::filesystem::path& path) {
	std::map<std::string, std::string> source;
	for (const auto& file : std::filesystem::directory_iterator(path)) {
		source.emplace(file.path().filename().string(), LoadFile(file));
	}
	return source;
}
auto SpliteToken(std::string_view line) {
	std::vector<std::string_view> splite;
	auto begin = line.begin(), end = line.end();

	for (;;) {
		begin = std::find_if(begin, line.end(), [](const char& ch) {
			return false == std::isspace(ch);
			});
		if (line.end() == begin)
			break;

		end = std::find_if(begin, line.end(), [](const char& ch) {
			return std::isspace(ch);
			});

		if (2 < (end - begin)) {
			std::string_view front{ begin, begin + 2 }, back{ end - 2, end };
			if ("//" == front || "/*" == front || "*/" == front)
				end = begin + 2;
			if ("//" == back || "/*" == back || "*/" == back)
				end -= 2;
		}

		splite.emplace_back(std::string_view{ begin, end });

		if (line.end() == end)
			break;
		else
			begin = end;
	}
	return splite;
}
auto SpliteString(const std::string& input) {
	std::string::size_type start = 0, find = 0, offset = 0;
	std::vector<std::string_view> splite;

	for (;;) {
		find = input.find('\n', find);
		offset = std::invoke([&]() {
			auto pos = std::string::npos == find ? input.size() - 1 : find - 1;
			return '\r' == input[pos] ? 1 : 0;
			});
		if (std::string::npos != find) {
			if (start != (find - offset))
				splite.emplace_back(input.begin() + start, input.begin() + find - offset);
			start = ++find;
		}
		else {
			if (start != (find - offset))
				splite.emplace_back(input.begin() + start, input.end() - offset);
			break;
		}
	};
	return splite;
}
auto SkipCommment(std::vector<std::string_view> input, bool& skip) {
	int index = 0;
	std::vector<std::string_view> output;

	if (skip) {
		for (; index < input.size(); ++index) {
			if ("*/" == input[index]) {
				skip = false;
			}
		}
	}

	if (false == skip) {
		for (; index < input.size(); ++index) {
			if ("//" == input[index]) {
				break;
			}
			else if ("/*" == input[index]) {
				skip = true;
				for (++index; index < input.size(); ++index) {
					if ("*/" == input[index]) {
						skip = false;
						break;
					}
				}
			}
			else
				output.push_back(input[index]);
		}
	}
	return output;
}
auto SpliteShade(std::vector<std::string_view> shade) {
	std::vector<std::vector<std::string_view>> result;
	bool skip = false;
	for (const auto& line : shade) {
		auto token{ SkipCommment(SpliteToken(line), skip) };
		if (false == token.empty())
			result.push_back(std::move(token));
	}
	return result;
}
auto SpliteShade(const std::map<std::string, std::string>& shades) {
	std::map<std::string_view, std::vector<std::vector<std::string_view>>> output;
	for (const auto& [label, shade] : shades) {
		output.emplace(label, SpliteShade(SpliteString(shade)));
	}
	return output;
}
auto IsInclude(const std::vector<std::string_view>& line, std::string_view& name) {
	if (1 < line.size()) {
		if ("#include" == line[0]) {
			name = { line[1].begin() + 1, line[1].end() - 1 };
			return true;
		}
	}
	else
		return false;
}
std::vector<std::vector<std::string_view>> InsertInclude(
	const std::map<std::string_view, std::vector<std::vector<std::string_view>>>& shades,
	const std::vector<std::vector<std::string_view>>& shade, std::set<std::string_view>& ignore) {

	std::vector<std::vector<std::string_view>> output;
	for (auto iter = shade.begin(); iter != shade.end(); ++iter) {
		std::string_view name;
		if (IsInclude(*iter, name)) {
			if (ignore.contains(name)) {
				output.emplace_back(*iter);
			}
			else {
				ignore.insert(name);
				auto temp{ InsertInclude(shades, shades.at(name), ignore) };
				for (const auto& line : temp) {
					output.emplace_back(line);
				}
			}
		}
		else
			output.emplace_back(*iter);
	}
	return output;
}
auto MergeString(const std::vector<std::vector<std::string_view>> shade) {
	std::string result;
	if (shade.size()) {
		if (shade.front().size())
			result += shade.front().front();
		for (int index = 1; index < shade.front().size(); ++index) {
			result += " ";
			result += shade.front().at(index);
		}
	}

	for (int line = 1; line < shade.size(); ++line) {
		result += "\n";
		if (shade.at(line).size())
			result += shade.at(line).at(0);
		for (int token = 1; token < shade.at(line).size(); ++token) {
			result += " ";
			result += shade.at(line).at(token);
		}
	}
	return result;
}
auto InsertInclude(const std::map<std::string_view, std::vector<std::vector<std::string_view>>>& shades) {
	std::map<std::string, std::map<std::string, std::string>> output;

	for (const auto& [label, shade] : shades) {
		std::filesystem::path path{ label };
		auto ext{ path.extension().string() };
		auto stem{ path.filename().stem().string() };
		if (".glsl" != ext) {
			std::set<std::string_view> ignore;
			output[stem].emplace(ext, MergeString(InsertInclude(shades, shade, ignore)));
		}
	}
	return output;
}
std::map<std::string, std::map<std::string, std::string>> LoadShade(const std::filesystem::path& path) {
	auto source{ LoadSource(path) };
	auto shades{ SpliteShade(source) };
	return InsertInclude(shades);
}