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

#ifndef _TIMEUTILS_H_
#define _TIMEUTILS_H_

#include <string>
using namespace std;

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/local_time/local_time.hpp"
using namespace boost;

#include "Misc/windowsTypes.h"
#include "Misc/endianSwitch.h"

string getLocalDateTimeString(const local_time::local_date_time& ldt, const string& strfmt);
inline string getDateString(const local_time::local_date_time& ldt) { return getLocalDateTimeString(ldt, "%m/%d/%Y"); };
inline string getTimeString(const local_time::local_date_time& ldt) { return getLocalDateTimeString(ldt, "%T"); };
inline string getDateTimeString(const local_time::local_date_time& ldt) { return getLocalDateTimeString(ldt, "%a %b %d %Y %T"); };

posix_time::ptime getFromWindows64(int64_t t);
inline posix_time::ptime getFromWindows64Big(int64_t t) { return getFromWindows64(bigToHost64(t)); };
inline posix_time::ptime getFromWindows64Little(int64_t t) { return getFromWindows64(littleToHost64(t)); };
inline posix_time::ptime getFromFILETIME(const FILETIME& ft) { return getFromWindows64( ((int64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime); };
inline posix_time::ptime getFromUnix32(int32_t t) { return posix_time::from_time_t(t); };

int32_t getUnix32FromPTime(const posix_time::ptime& pt);
inline int32_t getUnix32FromWindows64(int64_t t) { return getUnix32FromPTime(getFromWindows64(t)); };
inline int32_t getUnix32FromLocalTime(const local_time::local_date_time& ldt) { return getUnix32FromPTime(ldt.utc_time()); };

#endif //_TIMEUTILS_H_
