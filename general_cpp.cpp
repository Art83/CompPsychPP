#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;



main(){
	string file = "for_general_eval_output.csv";
	ofstream output(file);
	string PIN,pre_PIN, pre_complete, complete, pre_location, location, pre_timestamp, timestamp, pre_version, version, pre_event_type, event_type, line;
	int general_counter = 0;
	int init_timestamp, final_timestamp;
	int actions = 0;

	ifstream input_file;
	input_file.open("for_general_eval.csv");
	if(input_file.fail()){
		cout << "Error opening the input file\n";
	}
	while(getline(input_file, line)){
		stringstream inputString(line);
		getline(inputString, PIN, ',' );
		getline(inputString, complete, ',');
		getline(inputString, timestamp, ',');
		getline(inputString, location, ',');
		getline(inputString, version, ',');
		getline(inputString, event_type, ',');
		//cout << PIN << " " << complete << " " << location <<" " << timestamp << " " << version << " " << event_type << "\n";  
		if(general_counter == 0){
			output << "PIN" << ',' << "version" << ',' << "location" << ',' << "total_time" << ',' << "num_acts" << "\n";
		} else if (general_counter == 1){
			pre_PIN = PIN;
			pre_complete = complete;
			pre_location = location;
			pre_timestamp = timestamp;
			pre_version = version;
			pre_event_type = event_type;
			init_timestamp = stoi(timestamp);
		} else {
			if(pre_PIN != PIN){
				final_timestamp = stoi(pre_timestamp);
				//cout << init_timestamp << " " << final_timestamp << " " << final_timestamp - init_timestamp << "\n";
				int delta = final_timestamp - init_timestamp;
				output << pre_PIN << ',' << pre_version << ',' << pre_location << ',' << delta << ',' << actions << ',' << pre_complete << "\n";
				actions = 0;
				pre_PIN = PIN;
				pre_complete = complete;
				pre_location = location;
				pre_timestamp = timestamp;
				pre_version = version;
				pre_event_type = event_type;
				init_timestamp = stoi(timestamp);
			} else if (event_type == "\"key press\""){
				actions++;
			} else {
				pre_timestamp = timestamp;
			}
		}
		general_counter++;
	}

}
