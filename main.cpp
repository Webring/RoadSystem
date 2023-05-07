#include <iostream>
#include "CityRoadGraph.h"

#define INPUT_FILE_NAME "input.roads"
#define OUTPUT_FILE_NAME "output.ways"

using namespace std;


int main() {
    CityRoadGraph roadmap(3);

    ifstream *input_file = new ifstream (INPUT_FILE_NAME);
    ofstream *output_file = new ofstream (OUTPUT_FILE_NAME);

    roadmap.import_roads_from_file(input_file);

    CityRoadGraph shortest_ways = roadmap.get_shortest_ways();

    shortest_ways.export_ways_to_file(output_file);
    return 0;
}


