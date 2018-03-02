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

#include "timeZoneCalculator.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time/local_time.hpp>

timeZoneCalculator::timeZoneCalculator() {
	try {
		boost::local_time::time_zone_ptr tz(new boost::local_time::posix_time_zone("GMT"));
		if (tz.get()) {
			m_TZ = tz;
		} else {
			ERROR("timeZoneCalculator::timeZoneCalculator() Error creating timezone");
		}
	} catch(boost::local_time::bad_offset) {
		ERROR("timeZoneCalculator::timeZoneCalculator() Bad offset exception");
	} catch(boost::local_time::bad_adjustment) {
		ERROR("timeZoneCalculator::timeZoneCalculator() Bad adjustment exception");
	} catch(...) {
		ERROR("timeZoneCalculator::timeZoneCalculator() Unknown exception");
	}
}

timeZoneCalculator::timeZoneCalculator(string strRegion, string strFilePath) {
	setTimeZone(strRegion, strFilePath);
}

int timeZoneCalculator::setTimeZone(string strRegion, string strFilePath) {
	int rv = -1;
	
	if (strFilePath.length() > 0 && strRegion.length() > 0) {
		try {
			boost::local_time::tz_database tzdb;
			tzdb.load_from_file(strFilePath);
			boost::local_time::time_zone_ptr tz = tzdb.time_zone_from_region(strRegion);
			if (tz.get()) {
				m_TZ = tz;
				rv = 0;
			} else {
				ERROR("timeZoneCalculator::setTimeZone(Region) Error retrieving time zone for region <" << strRegion << ">.");
			}
		} catch(boost::local_time::data_not_accessible) {
			ERROR("timeZoneCalculator::setTimeZone(Region) File not accessible exception for <" << strFilePath << ">.");
		} catch(boost::local_time::bad_field_count) {
			ERROR("timeZoneCalculator::setTimeZone(Region) Bad field count exception for <" << strFilePath << ">.");
		} catch(...) {
			ERROR("timeZoneCalculator::setTieZone(Region) Unknown exception");
		}
	} else {
		ERROR("timeZoneCalculator::setTimeZone(Region) Zero length region or file path.");
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
			boost::local_time::time_zone_ptr tz(new boost::local_time::posix_time_zone(strPosixTimeZone));
			if (tz.get()) {
				m_TZ = tz;
				rv = 0;
			} else {
				ERROR("timeZoneCalculator::setTimeZone(POSIX) Error creating timezone for <" << strPosixTimeZone << ">");
			}
		} catch(boost::local_time::bad_offset) {
			ERROR("timeZoneCalculator::setTimeZone(POSIX) Bad offset exception for <" << strPosixTimeZone << ">");
		} catch(boost::local_time::bad_adjustment) {
			ERROR("timeZoneCalculator::setTimeZone(POSIX) Bad adjustment exception for <" << strPosixTimeZone << ">");
		} catch(...) {
			ERROR("timeZoneCalculator::setTimeZone(POSIX) Unknown exception");
		}
	} else {
		ERROR("timeZoneCalculator::setTimeZone(POSIX) Zero length time zone string.");
	}
	
	return rv;
}

timeZoneCalculator::~timeZoneCalculator() {
}

boost::local_time::local_date_time timeZoneCalculator::calculateLocalTime(boost::posix_time::ptime pt) {
	try {
		boost::local_time::local_date_time rv = boost::local_time::local_date_time(pt, m_TZ);
		return rv;
	} catch(...) {
		ERROR("timeZoneCalculator::calculateLocalTime() Unknown exception");
	}
}

string timeZoneCalculator::getTimeZoneString() {
	string rv;

	try {
		rv = m_TZ->to_posix_string();
	} catch(...) {
		ERROR("timeZoneCalculator::getTimeZoneString() Unknown exception");
	}

	return rv;
}

boost::local_time::local_date_time timeZoneCalculator::createLocalTime(boost::gregorian::date d, boost::posix_time::time_duration td) {
	try {
		boost::local_time::local_date_time rv = boost::local_time::local_date_time(d, td, m_TZ, boost::local_time::local_date_time::NOT_DATE_TIME_ON_ERROR);
		return rv;
	} catch(...) {
		ERROR("timeZoneCalculator::createLocalTime() Unknown exception");
	}
}

boost::local_time::local_date_time timeZoneCalculator::createLocalTime(u_int8_t month, u_int8_t day, u_int16_t year, u_int8_t hour, u_int8_t min, u_int8_t sec) {
	try {
		boost::local_time::local_date_time rv = createLocalTime(boost::gregorian::date(year, month, day), boost::posix_time::time_duration(hour, min, sec));
		return rv;
	} catch(...) {
		ERROR("timeZoneCalculator::createLocalTime() Unknown exception");
	}
}

boost::local_time::local_date_time timeZoneCalculator::createLocalTime(string str, string strfmt) {
	boost::local_time::local_date_time ldt(boost::local_time::not_a_date_time, m_TZ);

	try {
		stringstream ss;
		boost::local_time::local_time_input_facet* input_facet = new boost::local_time::local_time_input_facet(strfmt + " %ZP");
		ss.imbue(locale(ss.getloc(), input_facet));
		ss << str + " " + getTimeZoneString();
		ss >> ldt;
	} catch(...) {
		ERROR("timeZoneCalculator::createLocalTime() Unknown exception");
	}

	return ldt;
}
