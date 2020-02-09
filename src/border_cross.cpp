#include <iostream>
#include <bits/stdc++.h>
#include <iomanip>
#include<math.h>
using namespace std;

//Constructors and Comparators
struct timestamp{
    int month;
    int day;
    int year;
    string time;
    string amrpm;
};

struct record_key{
    timestamp t;
    string measure;
    string border;
};

struct record_value{
    int val;
    int average;
};

bool operator<(const record_key &t1, const record_key &t2) 
{ 
    if(t1.t.month==t2.t.month){
        if(t1.measure==t2.measure){
            if(t1.border==t2.border){
                t1.border==t2.border;
            }
            return t1.border<t2.border;
        }
        return t1.measure<t2.measure;
    } 
    return t1.t.month<t2.t.month;
}

bool cmp(const pair<record_key,record_value> &p1,const pair<record_key,record_value> &p2){
    if(p1.first.t.month==p2.first.t.month){
        if(p1.second.val==p2.second.val){
            if(p1.first.measure==p2.first.measure){
                return p1.first.measure>p2.first.measure;
            }
            return p1.first.measure>p2.first.measure;
        }
        return p1.second.val>p2.second.val;
    }
    return p1.first.t.month>p2.first.t.month;
}

//Variables
map<record_key,record_value> entry;

// Functions
struct timestamp process_date(string dt){
    timestamp t;
    stringstream dtstr(dt);
    vector<string> parts;
    string part,date,time,amrpm;
    while(getline(dtstr,part,' ')){
        parts.push_back(part);
    }
    date = parts[0];
    t.time = parts[1];
    t.amrpm = parts[2];
    stringstream s(date);
    parts={};
    while(getline(s,part,'/')){
        parts.push_back(part);
    }
    t.month = stoi(parts[0]);
    t.day = stoi(parts[1]);
    t.year = stoi(parts[2]);
    return t;
}

void readFile(string p_filepath){
    int l_headerline=1;
    record_key key;
    record_value value;
    string m_row,m_col,month;
    try{
        ifstream fin (p_filepath,ios::in);
        if(!fin) throw "Error opening file! for read" ;
        while(getline(fin,m_row)){
        vector<string> dataitems;
        if(!l_headerline){
            stringstream s(m_row);
            while(getline(s,m_col,',')){
                dataitems.push_back(m_col);
            } 
            key.t = process_date(dataitems[4]);
            key.measure = dataitems[5];
            key.border = dataitems[3];
            entry[key].val+=stoi(dataitems[6]);
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

void writeFile(string p_filepath, map<record_key,record_value> p_entry, vector<pair<record_key,record_value>> &p_vec){
    try{
        ofstream fout;
        fout.open(p_filepath,ios::out);
        if(!fout) throw "Error opening file! for write" ;
        fout<<"Border,Date,Measure,Value,Average\n";
        vector<pair<record_key,record_value>> vec(entry.begin(),entry.end());
        for(auto it=vec.begin();it!=vec.end();it++){
            record_key temp1,temp2;
            temp1 = it->first;
            temp2 = it->first;
            temp1.t.month-=1;
            temp2.t.month-=2;
            if(it->first.t.month-2>0){ 
                it->second.average = round((float)(entry[temp1].val + entry[temp2].val)/(it->first.t.month-1));
            }
            else if(it->first.t.month-1>0){
                it->second.average = round((float)(entry[temp1].val)/(it->first.t.month-1));
            }
            else{
                it->second.average = 0;
            }
        }
        sort(vec.begin(),vec.end(),cmp);
        for(auto it=vec.begin();it!=vec.end();it++){
        string combine = "/"+to_string(it->first.t.year)+" "+it->first.t.time+" "+it->first.t.amrpm;
        fout<<it->first.border<<","<<std::setfill('0')<<std::setw(2)<<it->first.t.month<<"/"<<std::setfill('0')<<std::setw(2)<<it->first.t.day<<combine<<","<<it->first.measure<<","<<it->second.val<<","<<it->second.average<<"\n";
		}
        fout.close();
    }
    catch( const char* cstr )
    {
        std::cerr << cstr << '\n' ;
    }
}


int main(){

    readFile("./input/Border_Crossing_Entry_Data.csv");

    vector<pair<record_key,record_value>> l_vec_copy(entry.begin(),entry.end());
    
    writeFile("./output/report.csv",entry,l_vec_copy);
    
    return 0;
}