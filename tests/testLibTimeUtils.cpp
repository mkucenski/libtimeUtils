// Copyright 2016 Matthew A. Kucenski
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

#include "libtimeUtils/src/timeUtils.h"
#include "libtimeUtils/src/timeZoneCalculator.h"

#include <iostream>
#include <vector>
using namespace std;

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time/local_time.hpp"
using namespace boost;

int main() {
	timeZoneCalculator tzcalc;
	
	posix_time::ptime t(posix_time::second_clock::universal_time());
	cout << "Universal Time: " << t << "\n";
	
	cout << getDateTimeString(tzcalc.calculateLocalTime(t)) << "\n";			
	cout << getDateString(tzcalc.calculateLocalTime(t)) << "\n";			
	cout << getTimeString(tzcalc.calculateLocalTime(t)) << "\n";
	
	local_time::time_zone_ptr zone(new local_time::posix_time_zone("EST-05EDT,M4.1.0,M10.1.0"));
	local_time::local_date_time estldt(t, zone);
	cout << "EST Local Time: " << estldt << "\n";

	local_time::time_zone_ptr zone1(new local_time::posix_time_zone("MST-07"));
	local_time::local_date_time azldt(t, zone1);
	cout << "AZ Local Time: " << azldt << "\n";
	
	local_time::local_time_facet* output_facet = new local_time::local_time_facet();
	output_facet->format("%a %b %d, %H:%M %z");
	stringstream ss;
	ss.imbue(locale(locale::classic(), output_facet));
	ss << azldt;
	cout << "Formatted AZ Local Time: " << ss.str() << endl;
	
	local_time::tz_database tz_db;
	try {
		tz_db.load_from_file("zonespec.csv");
		vector<string> regions = tz_db.region_list();
		for (vector<string>::iterator it = regions.begin(); it != regions.end(); it++) {
		}
	} catch(...) {
		cout << "Exception thrown loading time zone database.\n";
	}
	
	try {
		local_time::time_zone_ptr zone2 = tz_db.time_zone_from_region("America/New_York");
		local_time::local_date_time nyldt(t, zone2);
		cout << "NY Local Time: " << nyldt << "\n";
		
		local_time::time_zone_ptr zone3 = tz_db.time_zone_from_region("America/Los_Angeles");
		local_time::local_date_time laldt(t, zone3);
		cout << "LA Local Time: " << laldt << "\n";
		
		local_time::time_zone_ptr zone4 = tz_db.time_zone_from_region("America/Chicago");
		local_time::local_date_time chgldt(t, zone4);
		cout << "Chicago Local Time: " << chgldt << "\n";
	} catch(...) {
		cout << "Exception thrown using time zone database.\n";
	}
	
	cout << endl << "Testing timeZoneCalculator class" << endl;
	
	timeZoneCalculator tzc1;
	timeZoneCalculator tzc2("America/Chicago", "./zonespec.csv");
	timeZoneCalculator tzc2a("America/New_York", "./zonespec.csv");
	timeZoneCalculator tzc3("PST-8PDT,M4.1.0,M10.1.0");
	timeZoneCalculator tzc4("GMT-5");

	cout << "GMT\t\t\t" << tzc1.calculateLocalTime(t) << "\t" << tzc1.getTimeZoneString() << endl;
	cout << "Chicago\t\t\t" << tzc2.calculateLocalTime(t) << "\t" << tzc2.getTimeZoneString() << endl;
	cout << "New_York\t\t" << tzc2a.calculateLocalTime(t) << "\t" << tzc2a.getTimeZoneString() << endl;
	cout << "PST-8PDT,M4.1.0,M10.1.0\t" << tzc3.calculateLocalTime(t) << "\t" << tzc3.getTimeZoneString() << endl;
	cout << "GMT-5\t\t\t" << tzc4.calculateLocalTime(t) << "\t" << tzc4.getTimeZoneString() << endl;
	
	cout << endl << "Testing input facets" << endl;

	cout << tzc2a.createLocalTime(string("2006 ") + "Sep 14 08:08:24.140 ns httpd[11092]:", "%Y %b %d %H:%M:%S%F") << endl;
	cout << tzc2a.createLocalTime(string("2006 ") + "Sep 14 08:08:24.140", "%Y %b %d %H:%M:%S%F") << endl;
	cout << tzc2a.getTimeZoneString() << endl;
	
	return 0;
}
