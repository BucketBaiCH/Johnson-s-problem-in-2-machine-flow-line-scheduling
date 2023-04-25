/*_____Johnson's Problem in 2-machine flow shop problem_____*/
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using std::vector;
using std::cout;

constexpr double M = DBL_MAX;        // The maximum number can be calculated in double
const char* file_name = "data_example.txt";  // The file that will be open and geu data
//const char* file_name = "test.txt";

/*****************This 2 Fuctions is to get the Makespan*************************/
double Larger(double x1, double x2)   // This microfunction is used for MAKESPAN CALCULATING
{
	return ((x1 >= x2) ? x1 : x2);
}
double Makespan(vector<vector<double>> select)
{
	vector<vector<double>> makespan = select;
	makespan[0][0] = select[0][0];
	for (decltype(select.size()) i = 1; i < select[0].size(); ++i) {      // Get the row 1
		makespan[0][i] = makespan[0][i - 1] + select[0][i];
	}
	for (decltype(select.size()) j = 1; j < select.size(); ++j) {         // Get the column 1
		makespan[j][0] = makespan[j - 1][0] + select[j][0];
	}
	for (decltype(select.size()) i = 1; i < select.size(); ++i) {         // Get another elements
		for (decltype(select.size()) j = 1; j < select[0].size(); ++j) {
			makespan[i][j] = Larger(makespan[i - 1][j], makespan[i][j - 1]) + select[i][j];
		}
	}
	double rslt = makespan[makespan.size() - 1][makespan[0].size() - 1];      // Total makespan is the last element
	return rslt;
}

/*______________MAIN FUNCTION_________________*/
int main()
{
	/*
	* *************Step 1 : Define a 2 x n matrix
	* 2 machines and n jobs
	* t_ij means operation_j in machine_i's process time 
	*/
	std::cout << "Open file : " << file_name << endl;
	ifstream inFile(file_name);    // Open file

	const unsigned machine = 2;    // Johnson's Algorithm is onle for 2_machine flow line
	unsigned jobs = 0;             // Define the number of jobs
	vector<vector<double>> process_time;   // Define a vector contain the time matrix

	if (inFile.is_open()) {        // Get the number of jobs
		inFile >> jobs;
		process_time.resize(machine, vector<double>(jobs, 0));
	}

	// Get the matrix
	if (inFile.is_open()) {        // Defeine the elements in matrix
		for (unsigned row = 0; row < machine; ++row) {
			for (unsigned col = 0; col < jobs; ++col) {
				inFile >> process_time[row][col];
			}
		}
	}
	
	inFile.close();             // Close file
	

	vector<vector<double>> judge{ process_time };        // Definr a matrix eqaul to process time use for judge if select one swap the big M

	vector<unsigned> sequence(process_time[0].size());   // sequece is for saving the job's new sequence
	  
	vector<vector<double>> new_time(                     // new_time is for saving the new sequance of time matrix
		process_time.size() , vector<double>(process_time[0].size())
	);
	

	/*
	* ******************Step 2&3****************
	*/
	unsigned position_machine1 = 0;                      // First position if min_time in machine1
	unsigned position_machine2 = sequence.size() - 1;    // Last position if min_time in machine2

	for (unsigned itr = 0; itr < sequence.size(); ++itr) {    // Get the element from small to large
		double min_value = judge[0][0];     // Initialize that first element is minmum and corresponding location
		unsigned min_col = 0;
		unsigned min_row = 0;

		for (unsigned j = 0; j < judge[0].size(); ++j) {
			for (unsigned i = 0; i < judge.size(); ++i) {
				if (judge[i][j] < min_value) {
					min_value = judge[i][j];
					min_col = j;
					min_row = i;
				}
			}
		}

		if (min_row == 0) {           // If the min_time is in machine_1 posite it front
			sequence[position_machine1] = min_col;

			new_time[0][position_machine1] = process_time[min_row][min_col];     // insert the column front
			new_time[1][position_machine1] = process_time[1][min_col];

			++position_machine1;      // move back the inserting position
			judge[0][min_col] = M;
			judge[1][min_col] = M;
		}
		else {                        // If the min_time is in machine_2 posite it behind
			sequence[position_machine2] = min_col;

			new_time[0][position_machine2] = process_time[0][min_col];           // insert the column behind
			new_time[1][position_machine2] = process_time[min_row][min_col];

			--position_machine2;      // move forward the inserting position
			judge[1][min_col] = M;
			judge[0][min_col] = M;
		}
	}

	/*
	* Last step : Output the result
	* new time matix before sequence and after squence x2
	* The job sequence x1
	* corresponding makepan x1
	* 4 outputs totally
	*/
	std::cout << "The original time matrix: " << endl;
	for (vector<double> vec : process_time) {
		for (double j : vec) {
			cout << j << " ";
		}
		cout << endl;
	}

	cout << "The new time matrix: " << endl;
	for (vector<double> vec : new_time) {
		for (double j : vec) {
			cout << j << " ";
		}
		cout << endl;
	}
	cout << endl;

	cout << "*************************************************"
		 << endl << endl;

	cout << "The Job sequence is: ";
	for (unsigned i = 0; i < sequence.size() - 1; ++i) {
		cout << sequence[i] +1 << " - ";
	}
	cout << sequence[sequence.size() - 1] + 1 ;

	cout << endl;
	cout << "The corresponding makespan is: "
		 << Makespan(new_time) << endl << endl;

	cout << "*************************************************"
		 << endl << endl;

	return 0;
}