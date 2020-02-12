#include "border_analytics_d.h"

int main(){

    /** Steps Followed:
            Read file into ordered map -- O(nlogn)
            Calculate the moving average -- O(n)
            Copy the map to a vector -- O(n)
            Sort the vector -- O(nlogn)
            Loop through the vector and Write to a file -- O(n)

        Overall Time complexity: O(nlogn)
    **/
   
    string l_inputfilepath = "./input/Border_Crossing_Entry_Data.csv";
    string l_outputfilepath = "./output/report.csv";

    //Ordered Dictionary used to parse the file and write to file.
    map<record_key,record_value> entry; 

    readFile(l_inputfilepath,entry);

    // Calculating moving average
    calculate_moving_average(entry);

    // Copying map to a vector
    vector<pair<record_key,record_value>> l_vec_copy(entry.begin(),entry.end());
        
    // Sort in descending order by date,value,measure,border
    sort(l_vec_copy.begin(),l_vec_copy.end(),cmp);
        
    writeFile(l_outputfilepath,l_vec_copy);

    return 0;
}