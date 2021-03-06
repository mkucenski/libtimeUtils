// Copyright 2007 Matthew A. Kucenski
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef TIMEZONECALCULATOR_H_
#define TIMEZONECALCULATOR_H_

#include <string>
using namespace std;

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

class timeZoneCalculator {
	public:
		timeZoneCalculator();													//Default to GMT
		timeZoneCalculator(string strPosixTimeZone);			//Select time zone from a POSIX time zone string
		timeZoneCalculator(string strRegion, string strFilePath);	//Select time zone by region name from a time zone database
		~timeZoneCalculator();

		int setTimeZone(string strPosixTimeZone);			//Select time zone from a POSIX time zone string
		int setTimeZone(string strRegion, string strFilePath);		//Select time zone by region name from a time zone database
		boost::local_time::time_zone_ptr getTimeZone() { return m_TZ; };	//Retrieve a copy of the local_time::time_zone_ptr member
		
		boost::local_time::local_date_time calculateLocalTime(boost::posix_time::ptime pt);	//Calculate local time from a UTC ptime
											//value based on the loaded time zone.
											
		bool createLocalTime(	boost::gregorian::date d, 		//Create a local_date_time object from local
								boost::posix_time::time_duration td,
								boost::local_time::local_date_time* p_local);	//date and time values that are already in 
																										//this time zone.
																											
		bool createLocalTime(	u_int8_t month, //Create a local_date_time object from month,
								u_int8_t day, 	//day, year, hour, min, sec values that are 
								u_int16_t year, //already in this time zone.
								u_int8_t hour, 
								u_int8_t min, 
								u_int8_t sec,
								boost::local_time::local_date_time* p_local);
																	
		bool createLocalTime(string str, string strfmt,
							 boost::local_time::local_date_time* p_local);	//Create a local_date_time object from a 
											//date/time string that is already in this 
											//time zone.
		
		string getTimeZoneString();	//Return the POSIX string representation of the currently 
						//loaded time zone.
		
	private:
		boost::local_time::time_zone_ptr m_TZ;
};

#endif /*TIMEZONECALCULATOR_H_*/
