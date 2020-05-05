/*------------------------------------------------------------------------------------------
    File Name: main.cpp

    Description: Main Driver file, opens a two files one for input and one for output

    Usage:   make main
             ./main

    compiler: g++ 5.4

    Notes: includes BinarNOde and BinarySearchTree

    Modification History:
    Author        Date        Modifications
    ---------------------------------------------------------------------------
    Andre Travis  5/2/2020    Created Program and Graph
    Andre Travis  5/3/2020    Added in getPathLength
    Matthew Santos 5/3/2020   Derive distance between each city
    Andre Travis  5/4/2020    Added in getPath and Brute Force
    Matthew Santos 5/4/2020   Change to main function, slight edits to bruteforce and pathLength
                              Added functionality with display and cityName
                              Exported data to .txt
----------------------------------------------------------------------------------------------*/

#include <iostream>
#include <utility>
#include <string>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 

typedef boost::property<boost::edge_weight_t, double> EdgeWeight;
typedef boost::property<boost::vertex_index_t, double> verIndex;
typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, verIndex, EdgeWeight> Graph;
typedef boost::graph_traits<Graph>::edge_iterator edge_iterator;

using namespace boost;

//function prototypes
void bruteForce( Graph g);
void secondLetter(Graph g);
void thirdLetter(char [], Graph g, int, int);
int getPathLength(char path[], const int SIZE, Graph g);
void getPath(char [],int index);
void display(int num1, int num2);
void cityName(int num);

/*----------------------------------------------------------------------------------------------
Function name: main
Purpose:  drives the program
Returns: nothing
Notes: Creates graph and calls brute force
-------------------------------------------------------------------------------------------*/
int main(int, char*[])
{
	enum{Reno, SanFrancisco, SaltLakeCity, Seattle, LasVegas};
	const int num_vertices = 5;
	
	typedef std::pair<int, int> Edge;
	//sets edge to be used in graph
	Edge edge_array[] = {
		Edge(Reno, SanFrancisco), Edge(Reno, SaltLakeCity), Edge(Reno,Seattle),
		Edge(Reno, LasVegas), Edge(SanFrancisco, Seattle), Edge(SanFrancisco, LasVegas),
		Edge(SaltLakeCity, Seattle), Edge(SaltLakeCity, LasVegas), Edge(Seattle, LasVegas)};
	int weight_array[] = {218, 518, 704, 443, 808, 569, 829, 421, 1115};
	//sets the number edges based on edge_array
	const int num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

	Graph g(edge_array, edge_array + sizeof(edge_array)/ sizeof(Edge), weight_array, num_vertices);
	//prints to terminal distance between each city
	std::cout << "City #1\t\tCity #2\t\tMiles Between" << std::endl
	          << "Reno\t\tSanFrancisco\t218\n\t\tSaltLakeCity\t518\n\t\tSeattle\t\t704\n\t\tLasVegas\t443\n"
		  << "SanFrancisco\tSaltLakeCity\t736(Through Reno)\n\t\tSeattle\t\t808\n\t\tLasVegas\t569\n"
		  << "SaltLakeCity\tSeattle\t\t829\n\t\tLasVegas\t421\n"
		  << "Seattle\t\tLasVegas\t1115\n\n";
//Gets the path and the length trough a brute force method
	bruteForce(g);
	

}

/*----------------------------------------------------------------------------------------------
Function name: bruteForce
Purpose:  get the shortest path through the graph
Returns: nothing
Notes: Gets path by trying all the possible variations of the path
-------------------------------------------------------------------------------------------*/
void bruteForce(Graph g)
{
    char currentPath[6];
    char shortestPath[6];
    int length;
    int shortestLength = 10000000;

    //Runs through all 24 variations of the path
    for(int index = 0 ; index < 24; index++)
    {
        //gets the path
        getPath(currentPath, index);
        //gets the length of the path
        length = getPathLength(currentPath, sizeof(currentPath), g);

        //stores the shortest path and length
        if(length < shortestLength)
        {
            shortestLength = length;
            for(int i = 0; i < 6; ++i)
            {
                shortestPath[i] = currentPath[i];
            }
        }

    }
	
    std::cout << std::endl <<"The shortest path is " << shortestLength << " miles" << std::endl;
}

/*----------------------------------------------------------------------------------------------
Function name: getPathLength
Purpose:  gets the length of the entered path, displays paths to terminal through loops
Returns: int
Notes:  uses the boost libray to get the weight of an edge
-------------------------------------------------------------------------------------------*/
int getPathLength(char path[], const int SIZE, Graph g)
{

    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);

    graph_traits<Graph>::edge_iterator ei, ei_end;
    property_map<Graph, edge_weight_t>::type EdgeWeightMap = get(boost::edge_weight, g);


    int pathLength = 0, num1, num2;

    //Runs throught the entire c -string and gets the weight of each path
    for(int i = 0; i < SIZE; ++i)
    {
        num1 = path[i] - 65;
        num2 = path[i + 1] - 65;
        // Runs throught the graph checking the starting and ending edge values
        for(tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
        {
            //checks for the starting and ending value of the current edge
	    //displays direction by comparing the start and end values.
            if(index[source(*ei, g)] == num1 && index[target(*ei, g)] == num2)
            {
                //if correct add the weight to the pathLength
		//display to terminal the start city to the next.
		display(index[source(*ei, g)], index[target(*ei,g)]);
                pathLength += EdgeWeightMap[*ei];
            }
	    else if(index[source(*ei, g)] == num2 && index[target(*ei, g)] == num1){
		//if correct add the weight to the pathLength
		//display to terminal the start city to the next.
		display(index[target(*ei,g)], index[source(*ei, g)]);
                pathLength += EdgeWeightMap[*ei];
	    }
        }
        //if going from San Francisco to Salt Lake city or vice versa
	//if else statement to orientate statement in proper order 
        if(num1 == 1 && num2 == 2)
        {
	    	std::cout << "San Francisco to Salt Lake City";
            	pathLength += 736;
        }
	else if(num1 == 2 && num2 == 1){
	   	std::cout << "Salt Lake City to San Francisco";
	    	pathLength += 736;
	}
	std::cout << std::endl;
    }
	std::cout << "Total Miles = " << pathLength << std::endl << "**********************" <<std::endl;
    return pathLength;
}

/*----------------------------------------------------------------------------------------------
Function name: getPath
Purpose:  returns the path at the current requested index
Returns: nothing
Notes: 
-------------------------------------------------------------------------------------------*/
void getPath(char path[], int index)
{
//  A = Reno
//  B = San Francisco
//  C = Salt Lake City
//  D = Seattle
//  E = Las Vegas
    char allPaths[][6] = {{'A','B', 'C', 'D', 'E', 'A'},{'A','B', 'C', 'E', 'D', 'A'},
                        {'A','B', 'D', 'C', 'E', 'A'},{'A','B', 'D', 'E', 'C', 'A'},
                        {'A','B', 'E', 'C', 'D', 'A'},{'A','B', 'E', 'D', 'C', 'A'},
                        {'A','C', 'B', 'D', 'E', 'A'},{'A','C', 'B', 'E', 'D', 'A'},
                        {'A','C', 'D', 'B', 'E', 'A'},{'A','C', 'D', 'E', 'B', 'A'},
                        {'A','C', 'E', 'B', 'D', 'A'},{'A','C', 'E', 'D', 'B', 'A'},
                        {'A','D', 'B', 'C', 'E', 'A'},{'A','D', 'B', 'E', 'C', 'A'},
                        {'A','D', 'C', 'B', 'E', 'A'},{'A','D', 'C', 'E', 'B', 'A'},
                        {'A','D', 'E', 'B', 'C', 'A'},{'A','D', 'E', 'C', 'B', 'A'},
                        {'A','E', 'B', 'C', 'D', 'A'},{'A','E', 'B', 'D', 'C', 'A'},
                        {'A','E', 'C', 'B', 'D', 'A'},{'A','E', 'C', 'D', 'B', 'A'},
                        {'A','E', 'D', 'B', 'C', 'A'},{'A','E', 'D', 'C', 'B', 'A'} };

    //reads in the requested index from allPaths to the passed in path
    for(int i = 0; i < 6; ++i)
    {
        path[i] = allPaths[index][i];
    } 

    

}
/*----------------------------------------------------------------------------------------------
Function name: display
Purpose:  displays the current state of the travler to terminal
Returns: nothing
Notes: 
-------------------------------------------------------------------------------------------*/
void display(int num1, int num2){
	//prints out "city#1 to city#2"
	cityName(num1);
	std::cout << " to ";
	cityName(num2);
}

/*----------------------------------------------------------------------------------------------
Function name: cityName
Purpose:  utilizes switch to convert current iteration to names of the city
Returns: nothing
Notes: 
-------------------------------------------------------------------------------------------*/
void cityName(int num){
	//switch statement that correlates num with a city
	switch (num){
		case 0:{ std::cout << "Reno";
			break;
		}
		case 1:{ std::cout << "San Francisco";
			break;
		}
		case 2:{ std::cout << "Salt Lake City";
			break;
		}
		case 3:{ std::cout << "Seatlle";
			break;
		}
		case 4:{ std::cout << "Las Vegas";
			break;
		}
		default:{ std::cout << "error"; }
	}
}