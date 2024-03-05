#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cmath>

// Custom comparison function to compare pairs based on the second value
bool compareSecond(const std::pair<int, double>& a, const std::pair<int, double>& b) {
	return a.second > b.second;
}

// Function to sort the map according 
// to value in a (key-value) pairs 
std::multimap<double, int> sort(std::map<int, double>& P) {


	// Declare a multimap
	std::multimap<double, int> MM;

	// Insert every (key-value) pairs from
	// map M to multimap MM as (value-key)
	// pairs
	for (auto& it : P) {
		MM.insert({ it.second, it.first });
	}

	return MM;
}

// Print the first thousand entries of the multimap
void topThousandPrinter(std::multimap<double, int> MM) {
	int count = 0;
	for (auto& it : MM) {
		++count;
		if (count > 1000) {
			break;
		}
		std::cout << it.second << ' ' << it.first << std::endl;
	}
}

// Function to load file
std::fstream fileLoader(char fn[]) {

	//Try to open the file
	std::fstream file(fn);

	// If file does not exist, Create new file
	if (!file.is_open())
	{
		file.open(fn, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

	//If file exists, clear file
	else
	{
		file.open(fn, std::ofstream::out | std::ofstream::trunc);
	}

	return file;
}

// Function to close file
void fileCloser(std::fstream& f) {
	f.close();
	std::cout << "\n";
}

// Function to write pagerank to file
void pageRankWriter(std::multimap<double, int> MM) {

	char filename[] = "pagerank_out.txt";
	std::fstream currentFile;

	currentFile = fileLoader(filename);

	
	//std::cout << "Opened file successfully" << std::endl;

	fileCloser(currentFile);

}


int main() {
	// ifstream file;
	// file.open("web-Stanford.txt");

	std::ifstream file("web-Stanford.txt"); // Change "edges.txt" to your file name
	if (!file.is_open()) {
		std::cerr << "Unable to open file" << std::endl;
		return 1;
	}

	std::cout << "Opened file successfully" << std::endl;

	std::string line;
	// Skip the first 4 lines
	for (int i = 0; i < 4; ++i) {
		if (!std::getline(file, line)) {
			std::cerr << "Failed to skip initial lines or file too short" << std::endl;
			return 1;
		}
	}

	std::map<int, std::vector<int> > edges;
	std::map<int, double> pagerank, previous_pagerank;
	std::set<int> nodes;
	std::map<int, int> out_degree;

	int from_id, to_id;
	while (file >> from_id >> to_id) {
		// Process the edge here
		edges[to_id].push_back(from_id);
		out_degree[from_id]++;
		nodes.insert(from_id);
		nodes.insert(to_id);
	}

	file.close();

	double epsilon = 0.15, one_minus_epsilon = 1 - epsilon, avg_error = 0.00001;
	int n = nodes.size();

	int num_iterations = (int)(std::log(avg_error) / std::log(one_minus_epsilon));
	double avg_epsilon = epsilon / n;
	// Initialize pagerank
	for (int node : nodes) {
		pagerank[node] = 1.0 / n;
	}

	//A
	// Implement the pagerank algorithm here
	for (int t = 1; t < num_iterations; t++)
	{
		previous_pagerank = pagerank;
		for (int node : nodes)
		{
			double sum = 0;
			for (int from_node : edges[node])
			{
				sum += previous_pagerank[from_node] / out_degree[from_node];
			}
			pagerank[node] = one_minus_epsilon * sum + avg_epsilon;
		}

	}

	std::multimap<double, int>  sorted = sort(pagerank);

	// Print the pagerank of the top 1000 nodes
	topThousandPrinter(sorted);


	// Output all the nodes and their pageranks in a file
	pageRankWriter(sorted);

	//Top 5 pagerank tracker here
	// 


	return 0;
}
