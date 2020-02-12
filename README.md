# Border Crossing Analysis

## Table of Contents
1. [Problem](README.md#problem)
1. [Approach and Time complexity](README.md#Approach-and-Time-complexity)
1. [Assumptions Made](README.md#Asumptions-made)
1. [Expected output](README.md#Expected-output)
1. [Pitfalls](README.md#Pitfalls)
1. [Unit Testing](README.md#Unit-testing)

## Problem
The Bureau of Transportation Statistics regularly makes available data on the number of vehicles, equipment, passengers and pedestrians crossing into the United States by land.

**For this challenge, we want to you to calculate the total number of times vehicles, equipment, passengers and pedestrians cross the U.S.-Canadian and U.S.-Mexican borders each month. We also want to know the running monthly average of total number of crossings for that type of crossing and border.**


## Approach and Time complexity 

**Read file into ordered map -- O(nlogn)**
The input file is read to parse the required columns data into an ordered map (Key = date,measure,border & Value = value,average).
Here both key and value pairs are of custom struct types and the operator(<) is overloaded to sort key in ascending order by date,measure and border.

**Calculate the moving average -- O(n)**
The ascending order of the map is necessary to calculate the monthly moving average by looping through the ordered map(entry) and use an unordered map(key_except_month) as an intermediate buffer to calculate the moving average. Equality operator '==' is overload to avoid collisions and hash function is written for the unordered_map with struct type key.

**Copy the map to a vector -- O(n)**
The ordered map is now converted into a vector pair to sort in descending order by date, value, measure and border.

**Sort the vector -- O(nlogn)**
Custom sort function is written and date is split and sorted by year,month,date. Therefore overall descending order of the result set must be in order (year->month->date->value->measure->border)

**Loop through the vector and Write to a file -- O(n)**
Output file is written by looping through the vector and combining the date fields, columns in neccesary format.

***Overall Time complexity: O(nlogn)


## Asumptions made
**Data is almost clean except for invalid dates or missing data.**
	  -Missing data is handled by throwing an exception(Corrupt file! for read)     
	  -Invalid dates are handled by throwing an exception(Invalid date)

**In date field, time and AM/PM is constant "12:00:00 AM". However this can also be handled while sorting the date field**


## Pitfalls
  -Sorting date field should not just be string comparision but should be by year,month and date in order.    
  -Month and day values can be single digits. However, two digit format should be written to the output file.
  
  
## Unit Testing    
Test cases were written by keeping in mind of the above pitfalls.     
  -Case 1 -> Already provided by insight, sample input.    
  -Case 2 -> Similar dates and border details input to check how data merges.        
  -Case 3 -> Different dates and measure deatils to check how data is sorted.     

***Microsoft Excel was used to validate the output when the run with the provided large dataset.
