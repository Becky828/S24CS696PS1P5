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
    std::map<int, double> pagerank;
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

    double epsilon = 0.15, one_minus_epsilon = 1-epsilon, avg_error = 0.00001;
    int n = nodes.size();

    int num_iterations = (int) (std::log(avg_error) / std::log(one_minus_epsilon));
    double avg_epsilon = epsilon / n;
    // Initialize pagerank
    for (int node : nodes) {
        pagerank[node] = 1.0 / n;
    }

    // Implement the pagerank algorithm here
    for (int t = 1; t < num_iterations; t++)
    {
        for (int node : nodes)
        {
		   double sum = 0;
           for (int from_node : edges[node])
           {
			   sum += pagerank[from_node] / out_degree[from_node];
		   }
           pagerank[node] = one_minus_epsilon * sum + avg_epsilon;
	   }

    }
    // Print the pagerank of the top 1000 nodes

    // Output all the nodes and their pageranks in a file


    return 0;
}
