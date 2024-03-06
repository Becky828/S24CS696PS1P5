//#include "matplotlibcpp.h"
#include <matplot/matplot.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <cmath>

// Custom comparison function to compare pairs based on the second value
//bool compareSecond(std::pair<int, double>& a, std::pair<int, double>& b) {

bool compareSecond(const std::pair<int, double>& a, const std::pair<int, double>& b) {
	return a.second > b.second;
}

// Function to sort the map according 
// to value in a (key-value) pairs 
std::vector<std::pair<int, double> > sort(std::map<int, double> M)
{
	// Declare vector of pairs 
	std::vector<std::pair<int, double> > A;

	// copy key-value pairs from the map to the vector
	std::map<int, double> ::iterator it2;
	for (it2 = M.begin(); it2 != M.end(); it2++)
	{
		A.push_back(std::make_pair(it2->first, it2->second));
	}

	// Sort using comparator function 
	sort(A.begin(), A.end(), compareSecond);

	return A;
}

// Print the first thousand entries of the multimap
void topThousandPrinter(std::vector<std::pair<int, double> >  V) {
	int count = 0;
	for (auto& it : V) {
		++count;
		if (count > 1000) {
			break;
		}
		std::cout << it.second << ' ' << it.first << std::endl;
	}
}

//Gets the top 5 pagerank nodes
std::vector<std::pair<int, double>> topFiveGetter(std::vector<std::pair<int, double> >  V) {
	int count = 0;

	// Declare vector of pairs 
	std::vector<std::pair<int, double> > A;

	// copy key-value pairs from the map to the vector
	std::vector<std::pair<int, double> >::iterator iter = V.begin();
	for (iter; iter != V.end(); iter++)
	{
		++count;
		if (count > 5) {
			break;
		}
		A.push_back(std::make_pair(iter->first, iter->second));
	}
	return A;
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

//Writes each node and its pagerank to a text file line line by line in decreasing order of pagerank
//void pageRankWriter(std::map<int, double>   V) {

void pageRankWriter(std::vector<std::pair<int, double> > V) {

	char filename[] = "pagerank_out.txt";
	//std::fstream currentFile;

	//currentFile = fileLoader(filename);
	std::fstream file(filename);

	// If file does not exist, Create new file
	if (!file.is_open())
	{
		file.open(filename, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}

	//If file exists, clear file
	else
	{
		file.open(filename, std::ofstream::out | std::ofstream::trunc);
	}

	int count = 0;

	for (std::pair<int, double>  v : V) {
		//++count;
		//if (count > 10) {
			//break;
		//}
		file << v.second << ' ' << v.first << std::endl;

	}
		
	file.close();
	//fileCloser(currentFile);

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
	std::map<int, double> plotted_pagerank, plotted_previous_pagerank;

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


	//pageRankWriter(pagerank);


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

	std::vector<std::pair<int, double> >   sorted = sort(pagerank);

	// Print the pagerank of the top 1000 nodes
	topThousandPrinter(sorted);


	// Output all the nodes and their pageranks in a file
	pageRankWriter(sorted);

	
	//B
	//Top 5 pagerank tracker here
	// 
	topFiveGetter(sorted);

	return 0;
}
