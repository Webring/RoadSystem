#include <iostream>
#include "fstream"
#include "string.h"
#include "limits"

#define INFINITY numeric_limits<float>::infinity()
#define START_CITY_NAME_PATTERN "City #%i"
#define INVALID_NUMBER_OF_CITIES_ERROR "It's impossible to create a road system from such a number of roads"
#define INDEX_OUT_OF_RANGE_ERROR_TEXT "Index out of range"
#define UNDEFINDED_INDEX_ERROR_TEXT "Can't return undefined index, because x = y"
#define CITY_FIND_ERROR_TEXT "Can't find city with this id"
#define CITY_DOES_NOT_EXIST_ERROR_TEXT "The city does not exist"
#define FILE_OPEN_ERROR_TEXT "File path not valid"
#define SELF_WIDTH_SET_ERROR_TEXT "Can't set self width"


using namespace std;


struct CityRoadGraph {
    int number_of_cities;
    float *distance_matrix_in_vector_form;
    char **city_names;

    CityRoadGraph(int n); // Конструктор для инициализации

    CityRoadGraph(const CityRoadGraph &road_graph); // Конструктор копирования

    void init(int n);

    int distance_matrix_in_vector_form_length();

    int matrix_index_to_linear(int x, int y);

    bool is_equal(CityRoadGraph &graph);

    float get(int x, int y);

    void set(int x, int y, float value);

    bool safety_set(int x, int y, float value);

    void print_distance_matrix();

    void add_road(int x, int y, float width);

    CityRoadGraph get_shortest_ways();

    char *get_city_name(int city_id);

    void set_city_name(int city_id, char *new_name);

    int get_city_index_by_name(char *city_name);

    void export_graph_to_file(ofstream *file);

    void import_graph_from_file(ifstream *file);

    void import_roads_from_file(ifstream *file);

    void export_distance_matrix_to_file(ofstream *file, char separator = ' ', bool replace_dot_to_comma = false);

    void export_ways_to_file(ofstream *file, bool export_infinity_ways = false);

private:
    int calc_linear_index(int x, int y);
};

