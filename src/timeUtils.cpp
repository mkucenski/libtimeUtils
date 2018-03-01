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

// #define _DEBUG_
#include "misc/debugMsgs.h"
#include "misc/errMsgs.h"

#include "timeUtils.h"

#include <sstream>
using namespace std;

#include "misc/debugMsgs.h"

string getLocalDateTimeString(const local_time::local_date_time& ldt, const string& strfmt) {
	DEBUG("getLocalDateTimeString()");

	stringstream ss;
	local_time::local_time_facet* output_facet = new local_time::local_time_facet(strfmt.c_str());
	ss.imbue(locale(ss.getloc(), output_facet));
	ss << ldt;
	return ss.str();
}

int32_t getUnix32FromPTime(const posix_time::ptime& pt) {
	DEBUG("getUnix32FromPTime()");

	int32_t rv = 0;

	posix_time::time_duration td_fromUnixEpoch = pt - posix_time::ptime(gregorian::date(1970, 1, 1), posix_time::time_duration(0, 0, 0));
	if (!td_fromUnixEpoch.is_negative()) {
		rv = td_fromUnixEpoch.total_seconds();	//Return the number of seconds in the duration between the given time and the UNIX epoch (1/1/1970)
	}

	DEBUG("getUnix32FromPTime() = " << rv);
	return rv;
}

posix_time::ptime getFromWindows64(int64_t t) {
	DEBUG("getFromWindows64()");

	int64_t t_totalsecs = t/10000000;			//Max value for t is 0x7FFF FFFF FFFF FFFF, divided by 10,000,000 requires 40 bits
	int32_t t_hours = t_totalsecs/3600;			//Dividing again by 3600, brings the max bits required for hours to less than 32.
	int32_t t_mins = (t_totalsecs%3600)/60;	//The means that for 64-bit Windows time values (nanoseconds since 1/1/1601), 	
	int32_t t_secs = (t_totalsecs%3600)%60;	//I can safely use this method to build a ptime value.
	DEBUG("totalsec: " << t_totalsecs << " hours: " << t_hours << " mins: " << t_mins << " secs: " << t_secs);
	return posix_time::ptime(gregorian::date(1601, 1, 1), posix_time::time_duration(t_hours, t_mins, t_secs));
};
