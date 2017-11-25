// GERT, Monte Carlo analysis
// Author: Alex Battiston
// Web: https://www.linkedin.com/in/battistonalex/

// If you want to run this code, you can just copy paste it in http://cpp.sh and click RUN.
// If you want to change the structure of the model, change only the variables and parameters in delimited area of MODEL SETUP PHASE

// Do not evaluate the code, because I am already tell you that the quality of the code is extremely low! ;-)

/******************* DO NOT CHANGE THIS CODE, start *******************/ 
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
 
struct iteration { int time; int cost; };
struct stats { int time; int count; int cost; };
struct link { int id; int node_id_start; int node_id_end; float probability; float probability_range_min; float probability_range_max; float time; int cost_fix; int cost_variable; };
struct node { int id; std::string name; std::vector < link > links; bool end; };
 
// Sort statistics by count function
bool sortByCount (const stats & lhs, const stats & rhs) { return lhs.count > rhs.count;}

int main () 
{
std::vector < node > nodes;
std::vector < link > links;
std::vector < iteration > monte_carlo_results;
/******************* DO NOT CHANGE THIS CODE, end *******************/  
  
/******************* START CHANGE PARAMETERS FROM HERE *******************/
// 'MODEL SETUP PHASE' START
int monte_carlo_runs = 100000;   // number of iteration in the Monte Carlo simulation
  
 // Initialize links
       //ID, NODE_ID_START, NODE_ID_DESTINATION, PROBABILITY, PROBABILITY_RANGE_START, PROBABILITY_RANGE_END, DAYS, COST FIX, COST VARIABLE
link link_1_2 { 1 , 1, 2, 1.0 , 0.0 , 1.0 , 5 , 0,  20};

link link_2_3 { 2 , 2, 3, 1.0 , 0.0 , 1.0 , 15, 0,  15};

link link_3_4 { 3 , 3, 4, 0.9 , 0.0 , 0.9 , 10, 10, 10};
link link_3_2 { 4 , 3, 2, 0.1 , 0.9 , 1.0 , 0 , 0,  0};

link link_4_5 { 5 , 4, 5, 0.7 , 0.0 , 0.7 , 20, 30, 10};
link link_4_3 { 6 , 4, 3, 0.3 , 0.7 , 1.0 , 0 , 0,  0};

link link_5_6 { 7 , 5, 6, 0.7 , 0.0 , 0.7 , 4 , 15, 5};
link link_5_3 { 8 , 5, 3, 0.05, 0.7 , 0.75, 0 , 0,  0};
link link_5_4 { 9 , 5, 4, 0.25, 0.75, 1.0 , 0 , 0,  0};

link link_6_7 { 10, 6, 7, 0.95, 0.0 , 0.95, 1 , 0,  0};
link link_6_3 { 11, 6, 3, 0.05, 0.95, 1.0 , 0 , 0,  0};

// Initialize nodes
//node_ID = { NODE_ID, "Name of the node", {}, false}; the last parameters is true only when the node is the last one
node node_1 = { 1, "Project Start", {}, false};
node node_2 = { 2, "Objective defined", {}, false};
node node_3 = { 3, "Research completed", {}, false};
node node_4 = { 4, "Prototype designed", {}, false};
node node_5 = { 5, "Prototype developed", {}, false};
node node_6 = { 6, "Prototype tested", {}, false};
node node_7 = { 7, "Project End", {}, true};

// associate nodes and links
node_1.links.push_back (link_1_2);

node_2.links.push_back (link_2_3);

node_3.links.push_back (link_3_4);
node_3.links.push_back (link_3_2);

node_4.links.push_back (link_4_5);
node_4.links.push_back (link_4_3);

node_5.links.push_back (link_5_6);
node_5.links.push_back (link_5_4);
node_5.links.push_back (link_5_3);

node_6.links.push_back (link_6_7);
node_6.links.push_back (link_6_3);
 
// fill nodes
nodes.push_back (node_1);
nodes.push_back (node_2);
nodes.push_back (node_3);
nodes.push_back (node_4);
nodes.push_back (node_5);
nodes.push_back (node_6);
nodes.push_back (node_7); 
// 'MODEL SETUP PHASE' END

/******************* END PARAMETERS *******************/

// From here it starts the code, which must not be changed 
 
//parse the model and execute Monte Carlo analysis
int count_runs = 0;
do{  // monte_carlo_runs
    node node_temp = node_1;    // starting node
    int total_time = 0;    // sum the time per each branch
    int total_cost = 0;    // sum the cost per each branch
    do{  // till reach the last node
        float random_probability = (float) (std::rand () % 100) / 100;  // just a random number
        int next_node = -1;   // index of the next node
	    //std::cout << "Node name: " << temp.name << std::endl << " - p: " << random_probability << std::endl;
        for (link l : node_temp.links){
			if (node_temp.id == l.node_id_start 
                && l.probability_range_min <= random_probability 
                && random_probability < l.probability_range_max)
            {
                next_node = l.node_id_end;
                total_time += l.time;
                total_cost += l.cost_fix + l.cost_variable * l.time;
            }
        }
        node_temp = nodes.at(next_node-1);  // change 
    }
    while (node_temp.end == false);

    monte_carlo_results.push_back({total_time, total_cost});
    count_runs++;
	//std::cout << " " << node_temp.name << std::endl << "************************" << std::endl;
    }
while (count_runs != monte_carlo_runs);
  
// generate histogram
std::vector < stats > statistics;
for (iteration monte_carlo_result:monte_carlo_results){
    bool found = false;
    for (int i = 0; i < statistics.size (); i++){
        if (monte_carlo_result.time == statistics[i].time){
            statistics[i].count++;
            found = true;
        }
    }
    if (found == false)
        statistics.push_back ({monte_carlo_result.time, 1, monte_carlo_result.cost});
}
  
// show histogram values
std::cout << "GERT, Monte Carlo analysis: " << monte_carlo_runs << " iterations" << std::endl;
sort (statistics.begin (), statistics.end (), sortByCount);
for (int i = 0; i < statistics.size (); i++)
    std::cout << "Time: " << statistics[i].time << " - Cost: " << statistics[i].cost << " - Frequency: " << statistics[i].count << " - Probability - %" << (float) ((float) statistics[i].count * 100 /(float)monte_carlo_runs) <<std::endl;


return 0;
}



