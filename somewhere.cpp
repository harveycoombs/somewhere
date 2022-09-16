//Written by Harvey Coombs (Original: 2018, Revised: 2022)
#include <iostream>
#include <stdlib.h>
#include <filesystem>

namespace filesys = std::filesystem;

struct listed_dir {
	int index;
	std::string path;
};

std::string collect_in() {
	std::string final = "";

	char c;
	while (((c = fgetc(stdin)) != EOF) && (c != '\n')) {
		final += c;
	}

	return final;
}

void open_directory(std::string path, std::vector<listed_dir>* ctx) {
	filesys::path subject(path);

	if (filesys::exists(subject)) {
		ctx->clear();

		auto itr = std::filesystem::directory_iterator(subject);
		int x = 0;

		for (auto const& file : itr) {
			++x;
			std::string cur_path = file.path().string();

			std::cout << x;
			if (!filesys::is_directory(cur_path)) {
				std::cout << ".\t\u001b[36m" << cur_path
					<< "\u001b[0m\t["
					<< file.file_size()
					<< "B]\n";
			}
			else {
				std::cout << ".\t\u001b[33m" << cur_path
					<< "\u001b[0m\n";
			}

			struct listed_dir current {};
			current.index = x;
			current.path = file.path().string();

			ctx->push_back(current);
		}
	}
}

void open_dir_from_list(int target, std::vector<listed_dir>* ctx) {
	for (int l = 0; l < ctx->size(); l++) {
		if (ctx->at(l).index == target) {
			open_directory(ctx->at(l).path, ctx);
			break;
		}
	}
}

void prompter(std::vector<listed_dir>* ctx) {
	std::cout << "\n>";
	std::string choice = collect_in();

	open_dir_from_list(std::atoi(choice.c_str()), ctx);
	prompter(ctx);
}

int main(void) {
	std::vector<listed_dir> cur_list;

	open_directory("/", &cur_list);
	prompter(&cur_list);
}