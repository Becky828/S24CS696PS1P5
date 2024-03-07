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


//Function to gather sink nodes
std::vector<int> sinkFinder(std::set<int> nodes, std::map<int, int> o, std::map<int, int> i) {

	std::vector<int> sinks;


	for (int node : nodes) {

		for (int node : nodes) {

			if (i.contains(node) && !o.contains(node)) {

				int cnt = std::count(sinks.begin(), sinks.end(), node);

				// Check if the target value was found 
				if (cnt == 0) {
					sinks.push_back(node);
				}
			}
		}
	}

	return sinks;

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

	//Initialize the counter
	int count = 0;
	for (auto& it : V) {
		++count;
		if (count > 1000) {
			break;
		}
		std::cout << it.second << ' ' << it.first << std::endl;
	}
}

//Gets the ids of the top 5 pagerank nodes
std::vector<int> topFiveIDGetter(std::vector<std::pair<int, double> >  V) {

	// Initialize the counter
	int count = 0;

	// Declare vector of pairs 
	std::vector<int>  B;

	// copy key-value pairs from the map to the vector
	std::vector<std::pair<int, double> >::iterator iter = V.begin();
	for (iter; iter != V.end(); iter++)
	{
		++count;
		if (count > 5) {
			break;
		}
		B.push_back(iter->first);
	}
	return B;
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
	std::fstream file = fileLoader(filename);

	for (std::pair<int, double> v : V) {

		file << v.second << ' ' << v.first << std::endl;
	}

	//file.close();
	fileCloser(file);
}

void topFivePageRankWriter(std::vector<double> V1, std::vector<double> V2, std::vector<double> V3,
	std::vector<double> V4, std::vector<double> V5) {

	char filename_top_first[] = "top_first_out.txt";
	char filename_top_second[] = "top_second_out.txt";
	char filename_top_third[] = "top_third_out.txt";
	char filename_top_fourth[] = "top_fourth_out.txt";
	char filename_top_fifth[] = "top_fifth_out.txt";

	std::fstream file1 = fileLoader(filename_top_first);
	std::fstream file2 = fileLoader(filename_top_second);
	std::fstream file3 = fileLoader(filename_top_third);
	std::fstream file4 = fileLoader(filename_top_fourth);
	std::fstream file5 = fileLoader(filename_top_fifth);

	for (double v1 : V1) {

		file1 << v1 << std::endl;
	}

	for (double v2 : V2) {

		file2 << v2 << std::endl;
	}

	for (double v3 : V3) {

		file3 << v3 << std::endl;
	}

	for (double v4 : V4) {

		file4 << v4 << std::endl;
	}

	for (double v5 : V5) {

		file5 << v5 << std::endl;
	}

	//file.close();
	fileCloser(file1);
	fileCloser(file2);
	fileCloser(file3);
	fileCloser(file4);
	fileCloser(file5);
}


int main() {
	using namespace matplot;

	// ifstream file;
		// file.open("web-Stanford.txt");

	//std::ifstream file("web-Stanford.txt"); // Change "edges.txt" to your file name

	std::ifstream file("web-Stanford_very_abridged.txt"); // Change "edges.txt" to your file name


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

	std::map<int, std::vector<int>> edges;
	std::map<int, double> pagerank, previous_pagerank;
	std::map<int, double> top_five_pagerank, top_five_previous_pagerank;
	std::set<int> nodes;
	std::map<int, int> out_degree, in_degree;

	int from_id, to_id;
	while (file >> from_id >> to_id) {
		// Process the edge here
		edges[to_id].push_back(from_id);
		out_degree[from_id]++;
		in_degree[to_id]++;
		nodes.insert(from_id);
		nodes.insert(to_id);
	}


	file.close();

	std::vector<int> sink_nodes = sinkFinder(nodes, out_degree, in_degree);


	//std::map<int, int> ::iterator it4;

	for (int s : sink_nodes) {
		out_degree[s] += (nodes.size() - 1);
		
		for (auto i = in_degree.begin(); i != in_degree.end(); i++){

		//for (int node : nodes) {
			if (i->first != s) {
				in_degree[s]++;
			}
		}
		//for (it4 = out_degree.begin(); it4 != out_degree.end(); it4++) {

			//int current_node = it4->first;

			// Count the occurrences of the target value in the 
			// vector 
			//int cnt = std::count(sink_nodes.begin(), sink_nodes.end(), current_node);

			// Check if the target value was found 
			/*if (cnt > 0) {
				out_degree[current_node] += (nodes.size() - 1);
				for (int node : nodes) {
					if (node != current_node) {
						in_degree[node]++;
					}
				}
			}*/
			//}

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
		for (int t = 1; t < num_iterations + 1; t++)
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

		std::vector<std::pair<int, double> >  sorted = sort(pagerank);

		// Print the pagerank of the top 1000 nodes
		topThousandPrinter(sorted);


		// Output all the nodes and their pageranks in a file
		pageRankWriter(sorted);


		//B
		//Top 5 pagerank tracker here
		// 	

		// Initialize the vectors for file output
		std::vector<double> y_1;
		std::vector<double> y_2;
		std::vector<double> y_3;
		std::vector<double> y_4;
		std::vector<double> y_5;

		std::vector<int> topFiveIds = topFiveIDGetter(sorted);

		// Initialize plotted pagerank
		for (int node : nodes) {
			top_five_pagerank[node] = 1.0 / n;
		}

		std::map<int, double> ::iterator it3;

		// Plot the latest pageranks of the top 5 nodes

		int top_first = topFiveIds[0];
		int top_second = topFiveIds[1];
		int top_third = topFiveIds[2];
		int top_fourth = topFiveIds[3];
		int top_fifth = topFiveIds[4];

		std::vector<int> t_index;

		for (int t = 1; t < num_iterations + 1; t++)
		{
			t_index.push_back(t);

			top_five_previous_pagerank = top_five_pagerank;

			for (it3 = top_five_previous_pagerank.begin(); it3 != top_five_previous_pagerank.end(); it3++) {

				int current_node = it3->first;
				double current_pagerank = it3->second;

				// Count the occurrences of the target value in the 
				// vector 
				int cnt = std::count(topFiveIds.begin(), topFiveIds.end(), current_node);

				// Check if the target value was found 
				if (cnt > 0) {

					if (current_node == top_first) {
						y_1.push_back(current_pagerank);
					}
					else if (current_node == top_second) {
						y_2.push_back(current_pagerank);
					}
					else if (current_node == top_third) {
						y_3.push_back(current_pagerank);
					}
					else if (current_node == top_fourth) {
						y_4.push_back(current_pagerank);
					}
					else if (current_node == top_fifth) {
						y_5.push_back(current_pagerank);
					}

				}
			}

			for (int node : nodes)
			{
				double sum = 0;
				for (int from_node : edges[node])
				{
					sum += top_five_previous_pagerank[from_node] / out_degree[from_node];
				}
				top_five_pagerank[node] = one_minus_epsilon * sum + avg_epsilon;
			}

		}

		//plot(t_index, y_1)->line_width(2).color("red");
		//plot(t_index, y_1)->color({ 0.f, 0.7f, 0.9f });
		/*plot(x_2, y_2)->line_width(2).color("blue");
		plot(x_3, y_3)->line_width(2).color("green");
		plot(x_4, y_4)->line_width(2).color("yellow");
		plot(x_5, y_5)->line_width(2).color("black");*/

		//show();
		topFivePageRankWriter(y_1, y_2, y_3, y_4, y_5);
		return 0;
	}
}