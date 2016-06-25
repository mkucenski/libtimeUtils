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

#include "timeZoneCalculator.h"

#include "misc/debugMsgs.h"

timeZoneCalculator::timeZoneCalculator() {
	try {
		local_time::time_zone_ptr tz(new local_time::posix_time_zone("GMT"));
		if (tz.get()) {
			m_TZ = tz;
		} else {
			DEBUG_ERROR("timeZoneCalculator::timeZoneCalculator() Error creating timezone");
		}
	} catch(local_time::bad_offset) {
		DEBUG_ERROR("timeZoneCalculator::timeZoneCalculator() Bad offset exception");
	} catch(local_time::bad_adjustment) {
		DEBUG_ERROR("timeZoneCalculator::timeZoneCalculator() Bad adjustment exception");
	}
}

timeZoneCalculator::timeZoneCalculator(string strRegion, string strFilePath) {
	setTimeZone(strRegion, strFilePath);
}

int timeZoneCalculator::setTimeZone(string strRegion, string strFilePath) {
	int rv = -1;
	
	if (strFilePath.length() > 0 && strRegion.length() > 0) {
		try {
			local_time::tz_database tzdb;
			tzdb.load_from_file(strFilePath);
			local_time::time_zone_ptr tz = tzdb.time_zone_from_region(strRegion);
			if (tz.get()) {
				m_TZ = tz;
				rv = 0;
			} else {
				DEBUG_ERROR("timeZoneCalculator::setTimeZone(Region) Error retrieving time zone for region <" << strRegion << ">.");
			}
		} catch(local_time::data_not_accessible) {
			DEBUG_ERROR("timeZoneCalculator::setTimeZone(Region) File not accessible exception for <" << strFilePath << ">.");
		} catch(local_time::bad_field_count) {
			DEBUG_ERROR("timeZoneCalculator::setTimeZone(Region) Bad field count exception for <" << strFilePath << ">.");
		}
	} else {
		DEBUG_ERROR("timeZoneCalculator::setTimeZone(Region) Zero length region or file path.");
	}
	
	return rv;
}

timeZoneCalculator::timeZoneCalculator(string strPosixTimeZone) {
	setTimeZone(strPosixTimeZone);
}

int timeZoneCalculator::setTimeZone(string strPosixTimeZone) {
	int rv = -1;
	
	if (strPosixTimeZone.length() > 0) {
		try {
			local_time::time_zone_ptr tz(new local_time::posix_time_zone(strPosixTimeZone));
			if (tz.get()) {
				m_TZ = tz;
				rv = 0;
			} else {
				DEBUG_ERROR("timeZoneCalculator::setTimeZone(POSIX) Error creating timezone for <" << strPosixTimeZone << ">");
			}
		} catch(local_time::bad_offset) {
			DEBUG_ERROR("timeZoneCalculator::setTimeZone(POSIX) Bad offset exception for <" << strPosixTimeZone << ">");
		} catch(local_time::bad_adjustment) {
			DEBUG_ERROR("timeZoneCalculator::setTimeZone(POSIX) Bad adjustment exception for <" << strPosixTimeZone << ">");
		}
	} else {
		DEBUG_ERROR("timeZoneCalculator::setTimeZone(POSIX) Zero length time zone string.");
	}
	
	return rv;
}

timeZoneCalculator::~timeZoneCalculator() {
}

local_time::local_date_time timeZoneCalculator::calculateLocalTime(posix_time::ptime pt) {
	return local_time::local_date_time(pt, m_TZ);
}

string timeZoneCalculator::getTimeZoneString() {
	return m_TZ->to_posix_string();
}

local_time::local_date_time timeZoneCalculator::createLocalTime(gregorian::date d, posix_time::time_duration td) {
	return local_time::local_date_time(d, td, m_TZ, local_time::local_date_time::NOT_DATE_TIME_ON_ERROR);
}

local_time::local_date_time timeZoneCalculator::createLocalTime(u_int8_t month, u_int8_t day, u_int16_t year, u_int8_t hour, u_int8_t min, u_int8_t sec) {
	return createLocalTime(gregorian::date(year, month, day), posix_time::time_duration(hour, min, sec));
}

local_time::local_date_time timeZoneCalculator::createLocalTime(string str, string strfmt) {
	local_time::local_date_time ldt(local_time::not_a_date_time, m_TZ);

	stringstream ss;
	local_time::local_time_input_facet* input_facet = new local_time::local_time_input_facet(strfmt + " %ZP");
	ss.imbue(locale(ss.getloc(), input_facet));
	ss << str + " " + getTimeZoneString();
	ss >> ldt;
	
	return ldt;
}
