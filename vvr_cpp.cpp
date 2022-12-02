#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;





map<string,map<string,string>> list_snack_versions {
	{"A",{{"A1", "MM"},
	     {"A2", "BBQ"}}},
	{"B",{{"A1", "BBQ"},
	     {"A2","MM" }}},
	{"C",{{"A1", "TT"},
	     {"A2", "BBQ"}}},
	{"D",{{"A1", "BBQ"},
	     {"A2", "TT"}}},
	{"E",{{"A1", "TT"},
	     {"A2", "MM"}}},
	{"F",{{"A1", "MM"},
	     {"A2", "TT"}}},
	{"G",{{"A1", "MM"},
	     {"A2", "BBQ"}}},
	{"H",{{"A1", "BBQ"},
	     {"A2", "MM"}}},
	{"I",{{"A1", "TT"},
	     {"A2", "BBQ"}}},
	{"J",{{"A1", "BBQ"},
	     {"A2", "TT"}}},
	{"K",{{"A1", "MM"},
	     {"A2", "TT"}}},
	{"L",{{"A1", "TT"},
	     {"A2", "MM"}}} 
};



map<string, map<string, string>> degrad_pattern_versions {
	{"d1", {{"A1","deg"},
		{"A2","int"}}},
	{"d2", {{"A1","int"},
		{"A2","deg"}}},
	{"\"d0\"", {{"A1","int"},
		{"A2","int"}}} 
};




map<string,string> create_pattern_dict(){
	map<string, string> pattern;
	ifstream input;
	input.open("degrad_pattern.csv");
	if(input.fail()){
		cout << "Error with degradation pattern file!\n";
	}
	string PIN, pat, line;
	while(getline(input, line)){
		stringstream input_string(line);
		getline(input_string, PIN, ',');
		getline(input_string, pat, ',');
		if(pattern.find(PIN) != pattern.end()){
			cout << PIN << " " << "has a duplicate" << "\n";
		} else {
			pattern.insert({PIN, pat});
		}
	}
	return pattern;
}

int main(){
	map<string, string>pattern_map;
	pattern_map = create_pattern_dict();
	ifstream inputFile;
	inputFile.open("VVR1_clean.csv");
	if(inputFile.fail()){
		cout << "Error with opening the file!\n";
	}
	string file = "VVR1_processed.csv";
	ofstream output(file);
	string PIN,time, timestamp, stage, version, interval,block, 
	       event_raw, event_type, event_converted, snack, type, eventO1, eventO2, atype1, atype2, line;
	while(getline(inputFile, line)){
		stringstream inputString(line);
		getline(inputString, PIN, ',');
		getline(inputString, time, ',');
		getline(inputString, timestamp, ',');
		getline(inputString, stage, ',');
		getline(inputString,version, ',');
		getline(inputString, interval, ',');
		getline(inputString, block, ',');
		getline(inputString, event_raw, ',');
		getline(inputString, event_type, ',');
		getline(inputString, event_converted, ',');

		if(event_raw == "\"left tilt\"" || event_raw == "\"right tilt\"" || event_raw == "\"key release\"" || event_raw == "\"text appears\""){
			continue;
		}


		if(event_raw == "\"image appears\""){
			if(event_converted == "\"blank vending machine\""){
				snack = "blank";
				type = "no action";
			} else {
				string snack_token = event_converted;
				size_t pos = 0;
				string string_delim = " ";
				pos = snack_token.find(string_delim);
				snack_token = snack_token.substr(1,pos-1);
				snack = snack_token;
				version.erase(remove( version.begin(), version.end(), '\"'), version.end() );
				type = "no action";
			}
		} else if (event_raw == "\"key press\"") {
			if(event_type == "\"39\""){
				type = "A2";
				snack = "no image";
			} else if (event_type == "\"37\""){
				type = "A1";
				snack = "no image";
			} else {
				type = "no action";
				snack = "no image";
			}
		} else {
			type = "no action";
			snack = "no image";
		}
		version.erase(remove( version.begin(), version.end(), '\"'), version.end() );
		//cout << event_type << " " << snack << " " << version << " " <<  list_snack_versions[version]["A1"]  <<  "\n";
		if(event_raw == "\"image appears\"" && list_snack_versions[version]["A1"] == snack){
			eventO1 = "1";
			eventO2 = "0";
		} else if (event_raw == "\"image appears\"" && list_snack_versions[version]["A2"] == snack){
			eventO2 = "1";
			eventO1 = "0";
		} else {
			eventO2 = "0";
			eventO1 = "0";
		}
		//cout << version << "\n";
		//cout << list_snack_versions[version][type] << "\n";
		auto it = pattern_map.find(PIN);
		if (it == pattern_map.end()){
			cout << "PIN: "<<PIN<<" is not found\n";
			atype1 = "not found";
			atype2 = "not found";
		} else {
			atype1 = it->second;
			if(type == "A1" || type == "A2"){
				atype2 = degrad_pattern_versions[atype1][type];
			} else {
				atype2 = "no action";
			}
		}
		//cout << snack << " " << type << " " << eventO1 << " " << eventO2<< "\n";
		output << PIN << ',' << time << ',' <<  timestamp << ',' << stage << ',' << version << ',' << interval << ',' << block << ',' << 
			event_raw << ',' << event_type << ',' << 
			event_converted << ',' << snack << ',' << type << ',' <<  eventO1 << ',' << eventO2 << ',' << atype1 << ',' << atype2 << "\n";
	}
}

