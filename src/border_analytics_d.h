#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <math.h>
using namespace std;

//Structs//

struct timestamp{
    int month;
    int day;
    int year;
    string time;
    string amrpm;
};

struct record_key{
    struct timestamp t;
    string measure;
    string border;
};

struct key_except_month{
    int day;
    int year;
    string measure;
    string border;

    // Need to overload equality operator to handle hash collisions.
    // https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key/17017281#17017281
    bool operator==(const key_except_month& other) const
    { 
        return (day == other.day && year == other.year && measure == other.measure && border == other.border);
    }
};

struct record_value{
    int val;
    int average;
};

//Comparators and custom hash function//

bool operator<(const record_key &t1, const record_key &t2) 
{   /**
    Need operator(<) overloading (for ordered_map with struct key) in ascending order.
    **/
    if(t1.t.year==t2.t.year){
        if(t1.t.month==t2.t.month){
            if(t1.t.day==t2.t.day){
                if(t1.measure==t2.measure){
                    return t1.border<t2.border;
                }
                return t1.measure<t2.measure;
            }
            return t1.t.day<t2.t.day;
        }
        return t1.t.month<t2.t.month;
    } 
    return t1.t.year<t2.t.year;
}

namespace std
{   /**
    Need hash function(for unordered_map with struct key). 
    **/
    template <>
    struct hash<key_except_month>
    {
        size_t operator()( const key_except_month& k ) const
        {
            // Compute individual hash values
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<int>()( k.day );
            res = res * 31 + hash<int>()( k.year );
            res = res * 31 + hash<string>()( k.measure );
            res = res * 31 + hash<string>()( k.border );
            return res;
        }
    };
}

bool cmp(const pair<record_key,record_value> &p1,const pair<record_key,record_value> &p2){
    /**
    Comparator function for sorting the vector in descending order by date, value, measure and border.
     **/
    if(p1.first.t.year==p2.first.t.year){
        if(p1.first.t.month==p2.first.t.month){
            if(p1.first.t.day==p2.first.t.day){
                if(p1.second.val==p2.second.val){
                    if(p1.first.measure==p2.first.measure){
                        return p1.first.border>p2.first.border;
                    }
                    return p1.first.measure>p2.first.measure;
                }
                return p1.second.val>p2.second.val;
            }
            return p1.first.t.day>p2.first.t.day;
        }
        return p1.first.t.month>p2.first.t.month;
    }
    return p1.first.t.year>p2.first.t.year;
}


// Functions //

struct timestamp split_date(string dt){
    /**
    Parse the date column to more granular fields - month,day,year,time and AM/PM.
    **/
    timestamp t;
    stringstream l_dtstr(dt);
    vector<string> l_parts;
    string l_part,l_date,l_time,l_amrpm;
    while(getline(l_dtstr,l_part,' ')){
        l_parts.push_back(l_part);
    }
    l_date = l_parts[0];
    t.time = l_parts[1];
    t.amrpm = l_parts[2];
    stringstream s(l_date);
    l_parts={};
    while(getline(s,l_part,'/')){
        l_parts.push_back(l_part);
    }
    t.month = stoi(l_parts[0]);
    t.day = stoi(l_parts[1]);
    t.year = stoi(l_parts[2]);
    return t;
}

void readFile(string p_filepath,map<record_key,record_value> &p_entry){
    /**
    Read the required columns data into a map and sum the total number of 
    crossings of each type that crossed the border that month.
    **/
    int l_headerline=1;
    record_key key;
    record_value value;
    string l_row,l_col,month;
    try{
        ifstream fin (p_filepath,ios::in);
        if(!fin) throw "Error opening file! for read" ;
        while(getline(fin,l_row)){
        vector<string> dataitems;
        if(!l_headerline){
            stringstream s(l_row);
            while(getline(s,l_col,',')){
                if(l_col.empty()) throw "Corrupt file! for read" ;
                dataitems.push_back(l_col);
            } 
            key.t = split_date(dataitems[4]);
            if(key.t.month>12||key.t.month<1||key.t.day>31||key.t.day<1||key.t.year<1900) throw "Invalid date!" ;
            key.measure = dataitems[5];
            key.border = dataitems[3];
            p_entry[key].val+=stoi(dataitems[6]);
        }
        l_headerline=0;
    }
    fin.close();
    }
    catch( const char* cstr )
    {
        std::cerr << cstr << '\n' ;
    }

}

void writeFile(string p_filepath, vector<pair<record_key,record_value>> &p_vec){
    /**
    Write the sorted vector data onto a file in required format and order. 
    **/
    
    try{
        ofstream fout;
        string l_combinedate;
        fout.open(p_filepath,ios::out);
        if(!fout) throw "Error opening file! for write" ;
        fout<<"Border,Date,Measure,Value,Average\n";
        for(auto it=p_vec.begin();it!=p_vec.end();it++){
            l_combinedate = "/"+to_string(it->first.t.year)+" "+it->first.t.time+" "+it->first.t.amrpm;
            fout<<it->first.border<<","<<std::setfill('0')<<std::setw(2)<<it->first.t.month<<"/";
            fout<<std::setfill('0')<<std::setw(2)<<it->first.t.day;
            fout<<l_combinedate<<","<<it->first.measure<<","<<it->second.val<<","<<it->second.average<<"\n";
        }
        fout.close();
    }
    catch( const char* cstr )
    {
        std::cerr << cstr << '\n' ;
    }
}

void calculate_moving_average(map<record_key,record_value> &p_entry){
    /**
    Since the data in map is now sorted in ascending order by date, measure and border. 
    Moving average can be calculated by looping through it in sequence. 
    **/

    //To keep track of monthly moving average
    struct key_except_month temp; 

    //Dictionary used in moving average calculations
    unordered_map<key_except_month,int> l_avg;
    for(auto it=p_entry.begin();it!=p_entry.end();it++){
        temp.measure = it->first.measure;
        temp.border = it->first.border;
        temp.day = it->first.t.day;
        temp.year = it->first.t.year;
        if(l_avg.find(temp)==l_avg.end()){
            it->second.average = 0;
        }
        else{
            if(it->first.t.month<=1){
                it->second.average = 0;    
            }
            else{
                it->second.average = round((float)(l_avg[temp])/(it->first.t.month-1));
            }
        }
        l_avg[temp]+=it->second.val;
    }

}
