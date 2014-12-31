/*
 * Copyright (C) 2011-2013 Daniel Scharrer
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author(s) be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

/*!
 * \file
 *
 * Terminal output functions: colors, progress bar.
 */
#ifndef INNOEXTRACT_UTIL_CONSOLE_HPP
#define INNOEXTRACT_UTIL_CONSOLE_HPP

#include <stddef.h>
#include <ostream>
#include <iomanip>
#include <sstream>

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/cstdint.hpp>

namespace color {

/*!
 * Object that can be written to the console to change the output color.
 */
struct shell_command {
	const char * command;
};

std::ostream & operator<<(std::ostream & os, const shell_command command);

//! Reset the output color to the original value.
extern shell_command reset;

extern shell_command black;
extern shell_command red;
extern shell_command green;
extern shell_command yellow;
extern shell_command blue;
extern shell_command magenta;
extern shell_command cyan;
extern shell_command white;

extern shell_command dim_black;
extern shell_command dim_red;
extern shell_command dim_green;
extern shell_command dim_yellow;
extern shell_command dim_blue;
extern shell_command dim_magenta;
extern shell_command dim_cyan;
extern shell_command dim_white;

//! The last set output color.
extern shell_command current;

inline std::ostream & operator<<(std::ostream & os, const shell_command command) {
	color::current = command;
	return os << command.command;
}

enum is_enabled {
	enable,
	disable,
	automatic
};

/*!
 * Initilize console output functions.
 *
 * \param color    Enable or disable color output.
 * \param progress Enable or disable progress bar output.
 */
void init(is_enabled color = automatic, is_enabled progress = automatic);

} // namespace color

//! A text-based progress bar for terminals.
class progress {
	
	boost::uint64_t max;
	boost::uint64_t value;
	bool show_rate;
	
	boost::posix_time::ptime start_time;
	
	float last_status;
	boost::uint64_t last_time;
	
	float last_rate;
	std::ostringstream label;
	
public:
	
	/*!
	 * \param max       Maximumum progress values.
	 *                  If this value is \c 0, the progress bar will be unbounded.
	 * \param show_rate Display the rate at which the progress changes.
	 */
	progress(boost::uint64_t max = 0, bool show_rate = true);
	progress(const progress & o)
		: max(o.max), value(o.value), show_rate(o.show_rate), start_time(o.start_time),
		  last_status(o.last_status), last_time(o.last_time),
		  last_rate(o.last_rate), label(o.label.str()) { }
	
	/*!
	 * Update the progress bar.
	 *
	 * \param delta Value to add to the progress. When the total progress value reaches the
	 *              maximum set in the constructor, the bar will be full.
	 * \param force Force updating the progress bar. Normally, the progress bar. Otherwise,
	 *              updates are rate-limited and small deltas are not displayed immediately.
	 *
	 * \return true if the progres bar was updated
	 */
	bool update(boost::uint64_t delta = 0, bool force = false);
	
	/*!
	 * Draw a bounded progress bar (with a maximum).
	 *
	 * \param value The progress value, between \c 0.f and \c 1.f.
	 * \param label A label to draw next to the progress bar.
	 */
	static void show(float value, const std::string & label = std::string());
	
	/*!
	 * Draw an unbounded progress bar (without a maximum).
	 *
	 * \param value The progress value, between \c 0.f and \c 1.f.
	 * \param label A label to draw next to the progress bar.
	 */
	static void show_unbounded(float value, const std::string & label = std::string());
	
	/*!
	 * Clear any progress bar to make way for other output.
	 *
	 * \param reset_only Only reset the cursor if cleaning the line is expensive.
	 *                   This should be used if the whole line will be written anyway.
	 */
	static void clear(bool reset_only = false);
	
	//! Enable or disable the progress bar.
	static void set_enabled(bool enable);
	
	static bool is_enabled();
	
};

#endif // INNOEXTRACT_UTIL_CONSOLE_HPP
