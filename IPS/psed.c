#include <iostream>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <regex>

char *line;

int quit = 0;

std::vector<std::mutex *> line_ready;
std::vector<std::mutex *> print_ok;
std::vector<std::mutex *> line_processed;

char *to_cstr(std::string a) {
	// prevede retezec v c++ do retezce v "c" (char *)
	char *str;
	str = (char *) malloc(sizeof(char)*(a.length()+1));
	strcpy(str, a.c_str());
	return str;
}

char *read_line(int *res) {
	std::string line;
	char *str;
	if (std::getline(std::cin, line)) {
		str = to_cstr(line);
		*res = 1;
		return str;
	} else {
		*res = 0;
		return NULL;
	}

}


void f(int thread_id, std::regex regex, char *replacement) {
	while (1) {
		line_ready[thread_id]->lock();
		if (quit) break;
		std::string s = std::regex_replace(line, regex, replacement);
		if (thread_id != 0) {
		//	printf("before lock print_ok[%d]\n", thread_id - 1);
			print_ok[thread_id - 1]->lock();
		//	printf("after lock print_ok[%d]\n", thread_id - 1);
		}
		puts(s.c_str());
		print_ok[thread_id]->unlock();
		//printf("after unlock print_ok[%d]\n", thread_id);
		line_processed[thread_id]->unlock();
	}
}


int main(int argc, char *argv[]) {
	if ((argc - 1) % 2 != 0) {
		fprintf(stderr, "Spatny pocet argumentu\n");
		return 1;
	}

	std::vector<std::thread *> threads;
	int thread_num = 0;
	for (int i = 1; i < argc; i += 2) {
		std::regex regex(argv[i]);
		char *replacement = argv[i + 1];
		std::mutex *line_ready_m = new std::mutex();
		line_ready_m->lock();
		std::mutex *print_ok_m = new std::mutex();
		std::mutex *line_processed_m = new std::mutex();
		print_ok_m->lock();
		line_processed_m->lock();
		line_ready.push_back(line_ready_m);
		print_ok.push_back(print_ok_m);
		line_processed.push_back(line_processed_m);
		std::thread *thread = new std::thread(f, thread_num, regex, replacement);
		threads.push_back(thread);
		thread_num += 1;
	}

	int res;
	line = read_line(&res);
	while (res) {
		for (int i = 0; i < thread_num; i++) {
			line_ready[i]->unlock();
		}
		for (int i = 0; i < thread_num; i++) {
			line_processed[i]->lock();
		}
		free(line); /* uvolnim pamet */
		line = read_line(&res);

	}

	//free(line);

	quit = 1;

	for (int i = 0; i < thread_num; i++) {
		line_ready[i]->unlock();
		threads[i]->join();
		delete threads[i];
		delete line_ready[i];
		delete print_ok[i];
		delete line_processed[i];
	}

	return 0;
}