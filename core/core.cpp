// core.cpp : Parsowanie argumentow wywolania i wykonanie odpowiedniego algorytmu.
//

#include "stdafx.h"
#include "algorithms.h"

// * ZAINCLUDUJ PLIK Z ALGORYTMEM *
//
#include "Greedy.h"
#include "Naive.h"
#include "BruteForce.h"
#include "Bellman.h"
#include "ACS.h"


using namespace std;

void print_usage(char argv0[]) {
	cout << "Usage:" << endl;
	cout << "\t" << argv0 << " <algorithm name> file <file name>" << endl;
	cout << "\t\t" << "Gets graph from file." << endl;
	cout << "\t" << argv0 << " <algorithm name> generate <no. of nodes> <min. weight> <max. weight>" << endl;
	cout << "\t\t" << "Generates graph with random edge weights." << endl;
	cout << "\t" << argv0 << " <algorithm name>" << endl;
	cout << "\t\t" << "Gets the graph from stdin (same format as file)." << endl << endl;
	cout << "Options:" << endl;
	cout << "\t" << "verbose" << endl;
	cout << "\t\t" << "Debug output." << endl;
	cout << "\t" << "savegraph <file name>" << endl;
	cout << "\t\t" << "Saves input graph as a file." << endl;
	cout << "\t" << "printgraph" << endl;
	cout << "\t\t" << "Before performing calculations prints the graph." << endl;
	cout << "\t" << "saveresult <file name>" << endl;
	cout << "\t\t" << "Saves non-verbose output to file." << endl;
	cout << "\t" << "printresult" << endl;
	cout << "\t\t" << "Prints algorithm's result." << endl;
}

// * DODAJ STALA NAZWE ALGORYTMU *
//
#define A_GREEDY ("greedy")
#define A_N ("naive")
#define A_BF ("bruteforce")
#define A_BM ("bellman")
#define A_ACS ("acs")

int main(int argc, char* argv[])
{
	// --- PARSING ---
	enum Source {FILE, GENERATION, STDIN};
	bool verbose = false;
	bool save_graph_to_file = false;
	bool save_result_to_file = false;
	bool print_graph = false;
	bool print_result = false;

	Source source = STDIN;
	int gargv[3] = {0, 0, 0};

	char *algorithm_name = NULL;
	char *input_file_name = NULL;
	char *output_graph_file_name = NULL;
	char *output_result_file_name = NULL;
		
	bool correct_parsing = true;
	for (int n = 1; n < argc; n++) {
		if (strcmp(argv[n], "verbose") == 0)
			verbose = true;
		else if (strcmp(argv[n], "savegraph") == 0) {
			if (n+1 < argc) {
				n++;
				save_graph_to_file = true;
				output_graph_file_name = argv[n];
			}
			else {
				correct_parsing = false;
				break;
			}
		}
		// * DODAJ NAZWE DO WARUNKU *
		//
		// === PARSING ALGORITHM NAME ===
		else if (strcmp(argv[n], A_GREEDY) == 0 ||
				 strcmp(argv[n], A_N) == 0 ||
				 strcmp(argv[n], A_BF) == 0 ||
				 strcmp(argv[n], A_BM) == 0 ||
				 strcmp(argv[n], A_ACS) == 0)
			algorithm_name = argv[n];
		// ==============================
		else if (strcmp(argv[n], "file") == 0) {
			if (n+1 < argc) {
				n++;
				source = FILE;
				input_file_name = argv[n];
			}
			else
			{
				correct_parsing = false;
				break;
			}
		}
		else if (strcmp(argv[n], "generate") == 0) {
			if (n+3 < argc) {
				source = GENERATION;
				n++;
				gargv[0] = atoi(argv[n]);
				n++;
				gargv[1] = atoi(argv[n]);
				n++;
				gargv[2] = atoi(argv[n]);
			}
			else {
				correct_parsing = false;
				break;
			}
		}
		else if (strcmp(argv[n], "printgraph") == 0)
			print_graph = true;
		else if (strcmp(argv[n], "savegraph") == 0) {
			if (n+1 < argc) {
				save_graph_to_file = true;
				n++;
				output_graph_file_name = argv[n];
			}
			else {
				correct_parsing = false;
				break;
			}
		}
		else if (strcmp(argv[n], "saveresult") == 0) {
			if (n+1 < argc) {
				save_result_to_file = true;
				n++;
				output_result_file_name = argv[n];
			}
			else {
				correct_parsing = false;
				break;
			}
		}
		else if (strcmp(argv[n], "printresult") == 0)
			print_result = true;
		else {
			correct_parsing = false;
			break;
		}
	}
	// ---------------
	if (correct_parsing == true)
	{
		if (verbose) {
				cout << "tryb gadatliwy" << endl << "algorytm: ";
				cout << (algorithm_name ? algorithm_name : "placeholder");
				cout << endl << "zapis grafu do pliku: " << save_graph_to_file;
				cout << " " << (output_graph_file_name ? output_graph_file_name : "");
				cout << endl << "zrodlo grafu: ";
				cout << (source==FILE ? "FILE" : 
					(source==GENERATION ? "GENERATION" : 
					(source==STDIN ? "STDIN" : "nie znane")));
				cout << " " << (input_file_name ? input_file_name : "");
				cout << endl << "argumenty generacji: ";
				cout << gargv[0] << " " << gargv[1];
				cout << " " << gargv[2] << endl;
				cout << "drukuj graf: " << print_graph << endl << endl;
		}

		Graph *graph;
		if (source==STDIN)
			graph = new Graph(&cin);
		else if (source==FILE) {
			istream *input_file = new ifstream(input_file_name);
			graph = new Graph(input_file);
			delete input_file;
		}
		else if (source==GENERATION) {
			graph = new Graph(gargv[0], gargv[1], gargv[2]);
		}

		
		Cycle *output;
		// * WYKONAJ ALGORYTM *
		//
		// === CREATING ALGORITHM OBJECT ===
		if (algorithm_name == NULL)
			output = PlaceholderAlgorithm::perform_calculations(*graph);
		else if (strcmp(algorithm_name, A_GREEDY) == 0)
			output = GreedyAlgorithm::perform_calculations(*graph);
		else if (strcmp(algorithm_name, A_N) == 0)
			output = NaiveAlgorithm::perform_calculations(*graph);
		else if (strcmp(algorithm_name, A_BF) == 0)
			output = BruteForceAlgorithm::perform_calculations(*graph);
		else if (strcmp(algorithm_name, A_BM) == 0)
			output = BellmanAlgorithm::perform_calculations(*graph);
		else if (strcmp(algorithm_name, A_ACS) == 0)
			output = ACSAlgorithm::perform_calculations(*graph);
		// =================================

		if (print_graph)
			graph->print_graph(&cout, verbose);

		if (save_graph_to_file) {
			ofstream* output_file = new ofstream(output_graph_file_name);
			graph->print_graph(output_file, false);
			delete output_file;
		}

		if (print_result)
			output->print_cycle(&cout, verbose);

		if (save_result_to_file) {
			ofstream* output_file = new ofstream(output_result_file_name);
			output->print_cycle(output_file, false);
		}

		delete output; 
	}
	else
		print_usage(argv[0]);
		return -1;
	return 0;
}

