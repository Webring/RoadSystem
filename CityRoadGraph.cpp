#include "CityRoadGraph.h"

using namespace std;


void CityRoadGraph::init(int n) {
    if (n < 2) throw invalid_argument(INVALID_NUMBER_OF_CITIES_ERROR);

    number_of_cities = n;

    int length = distance_matrix_in_vector_form_length();

    distance_matrix_in_vector_form = new float[length];
    city_names = new char *[number_of_cities];

    for (int i = 0; i < length; i++) {
        distance_matrix_in_vector_form[i] = INFINITY;
    }

    for (int i = 0; i < number_of_cities; i++) {
        city_names[i] = new char[20];
        sprintf(city_names[i], START_CITY_NAME_PATTERN, i + 1);
    }
};

CityRoadGraph::CityRoadGraph(int n) {
    init(n);
};

CityRoadGraph::CityRoadGraph(const CityRoadGraph &road_graph) {
    init(road_graph.number_of_cities);

    int length = distance_matrix_in_vector_form_length();

    for (int i = 0; i < length; i++) {
        distance_matrix_in_vector_form[i] = road_graph.distance_matrix_in_vector_form[i];
    }

    for (int i = 0; i < number_of_cities; i++) {
        strcpy(city_names[i], road_graph.city_names[i]);
    }
};

int CityRoadGraph::distance_matrix_in_vector_form_length() {
    return (number_of_cities * (number_of_cities - 1)) / 2;
}

int CityRoadGraph::calc_linear_index(int x, int y) { // private
    if (x >= number_of_cities or y >= number_of_cities) throw out_of_range(INDEX_OUT_OF_RANGE_ERROR_TEXT);

    return (2 * number_of_cities - x - 1) * x / 2 + (y - x) - 1; // (2 * number_of_cities - x) * (x - 1) / 2 + (y - x) - 1
}

bool CityRoadGraph::is_equal(CityRoadGraph &graph) {
    if (number_of_cities == graph.number_of_cities) {
        int length = distance_matrix_in_vector_form_length();
        int number_of_matching = 0;
        for (int i = 0; i < length; i++) {
            number_of_matching += (distance_matrix_in_vector_form[i] == graph.distance_matrix_in_vector_form[i]);
        }
        if (number_of_matching == length) {
            number_of_matching = 0;
            for (int i = 0; i < number_of_cities; i++) {
                number_of_matching += (strcmp(city_names[i], graph.city_names[i]) == 0);
            }
            if (number_of_matching == number_of_cities) return true;
        }
    }
    return false;
}

int CityRoadGraph::matrix_index_to_linear(int x, int y) {
    if (x == y) throw invalid_argument(UNDEFINDED_INDEX_ERROR_TEXT);

    if (x < y) {
        return calc_linear_index(x, y);
    }
    return calc_linear_index(y, x);
}

float CityRoadGraph::get(int x, int y) {
    if (x == y) return 0;

    int index = matrix_index_to_linear(x, y);
    return distance_matrix_in_vector_form[index];
}

void CityRoadGraph::set(int x, int y, float value) {
    if (x == y) throw invalid_argument(SELF_WIDTH_SET_ERROR_TEXT);

    int index = matrix_index_to_linear(x, y);
    distance_matrix_in_vector_form[index] = value;
}

bool CityRoadGraph::safety_set(int x, int y, float value) {
    if (x == y) return 0;

    set(x, y, value);
    return 1;
}

void CityRoadGraph::print_distance_matrix() {
    for (int y = 0; y < number_of_cities; y++) {
        for (int x = 0; x < number_of_cities; x++) {
            cout << get(x, y) << "\t";
        }
        cout << endl;
    }
}

void CityRoadGraph::add_road(int x, int y, float distance) {
    float old_distance = get(x, y);
    if (distance < old_distance) {
        set(x, y, distance);
    }
}

CityRoadGraph CityRoadGraph::get_shortest_ways() {
    CityRoadGraph new_graph(*this);
    for (int departure = 0; departure < number_of_cities; departure++) {
        for (int intermediate = 0; intermediate < number_of_cities; intermediate++) {
            for (int destination = 0; destination < number_of_cities; destination++) {

                float departure_to_intermediate_width = new_graph.get(departure, intermediate);
                float intermediate_to_destination_width = new_graph.get(intermediate, destination);
                float sum_intermediate_widths = departure_to_intermediate_width + intermediate_to_destination_width;

                if (departure_to_intermediate_width == INFINITY or intermediate_to_destination_width == INFINITY) {
                    sum_intermediate_widths = INFINITY;
                }

                float value = min(new_graph.get(departure, destination), sum_intermediate_widths);

                new_graph.safety_set(departure, destination, value);
            }
        }
    }
    return new_graph;
}


char *CityRoadGraph::get_city_name(int city_id) {
    if (city_id < 0 or city_id >= number_of_cities) throw out_of_range(CITY_FIND_ERROR_TEXT);
    return city_names[city_id];
};

void CityRoadGraph::set_city_name(int city_id, char *new_name) {
    if (city_id < 0 or city_id >= number_of_cities) throw out_of_range(CITY_FIND_ERROR_TEXT);
    strcpy(city_names[city_id], new_name);
};

int CityRoadGraph::get_city_index_by_name(char *city_name) {
    for (int i = 0; i < number_of_cities; i++) {
        if (strcmp(get_city_name(i), city_name) == 0) return i;
    }
    return -1;
};

void CityRoadGraph::export_graph_to_file(ofstream *file) {

    *file << number_of_cities << " ";

    int length = distance_matrix_in_vector_form_length();

    for (int i = 0; i < length; i++) {
        *file << distance_matrix_in_vector_form[i];
        if (i + 1 != length) {
            *file << " ";
        }
    }

    for (int i = 0; i < number_of_cities; i++) {
        *file << endl << city_names[i];
    }

    file->close();
}

void CityRoadGraph::import_graph_from_file(ifstream *file) {
    if (not file->is_open()) {
        throw invalid_argument(FILE_OPEN_ERROR_TEXT);
    }

    int new_number_of_cities;

    *file >> new_number_of_cities;

    init(new_number_of_cities);

    for (int i = 0; i < distance_matrix_in_vector_form_length(); i++) {
        char *value_in_char_pointer = new char[20];
        float value;
        *file >> value_in_char_pointer;
        if (strcmp(value_in_char_pointer, "inf") == 0) {
            value = INFINITY;
        } else {
            sscanf(value_in_char_pointer, "%f", &value);
        }
        distance_matrix_in_vector_form[i] = value;
    }

    for (int i = 0; i < number_of_cities; i++) {
        *file >> city_names[i];
    }

    file->close();
}

void CityRoadGraph::import_roads_from_file(ifstream *file) {
    if (not file->is_open()) {
        throw invalid_argument(FILE_OPEN_ERROR_TEXT);
    }

    int new_number_of_cities;

    *file >> new_number_of_cities;

    init(new_number_of_cities);

    for (int i = 0; i < number_of_cities; i++) {
        *file >> city_names[i];
    }

    int numbers_of_roads;

    *file >> numbers_of_roads;

    float value;
    for (int i = 0; i < numbers_of_roads; i++) {
        char *value_in_char_pointer = new char[20];
        char *dep_city_name = new char[20];
        char *dest_city_name = new char[20];
        *file >> dep_city_name >> dest_city_name >> value_in_char_pointer;
        int dep = get_city_index_by_name(dep_city_name);
        int dest = get_city_index_by_name(dest_city_name);
        if (dep != -1 and dest != -1) {
            if (strcmp(value_in_char_pointer, "inf") == 0) {
                value = INFINITY;
            } else {
                sscanf(value_in_char_pointer, "%f", &value);
            }
            add_road(dep, dest, value);
        } else throw out_of_range(CITY_DOES_NOT_EXIST_ERROR_TEXT);
    }

    file->close();
}

void CityRoadGraph::export_distance_matrix_to_file(ofstream *file, char separator, bool replace_dot_to_comma) {
    for (int y = 0; y < number_of_cities; y++) {
        for (int x = 0; x < number_of_cities; x++) {
            *file << get(x, y);
            if (x != number_of_cities - 1) {
                *file << separator;
            }
        }
        if (y != number_of_cities - 1) {
            *file << endl;
        }
    }
    file->close();
}

void CityRoadGraph::export_ways_to_file(ofstream *file, bool export_infinity_ways) {
    for (int y = 0; y < number_of_cities; y++) {
        for (int x = y + 1; x < number_of_cities; x++) {
            float distance = get(x, y);
            if (distance != INFINITY or export_infinity_ways) {
                *file << get_city_name(y) << " (" << y + 1 << ") <-> " << get_city_name(x) << " (" << x + 1 << ") = " << distance;
                if (not(y == number_of_cities - 2 and x == number_of_cities - 1)) {
                    *file << endl;
                }
            }
        }
    }
    file->close();
}