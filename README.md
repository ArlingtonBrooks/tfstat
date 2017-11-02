# tfstat
Traffic Usage Monitor (pre-alpha state)

# Purpose
tfstat is meant to overcome some of the shortcomings of current UNIX-based traffic monitors.  tfstat continuously stores data (at specified intervals) such that a user can recall historical network bandwidth usage.  

# Usage
Currently tfstat is in a pre-alpha phase.  It has the basic functionality to work on Linux, however lacks the ability to grab traffic data on bsd-based systems.  Furthermore, most command line inputs are ignored and the program simply runs per the specified values in src/defaults.hpp.  

For these reasons, I do not reccommend using tfstat in its current state beyond testing functionality.

# Storage Methods
tfstat uses a database-type method of storing historical data.  When an interface's network stats are being saved, tfstat generates both a database file and a table file.  The database file is never loaded into memory in full because it may cause issues depending on the size.  While the table file is only about 50% smaller, it is easier to manage in memory.  

# Future Plans
In the future, I plan to make tfstat a functioning traffic monitoring package such that a user can have it run indefinitely in the background and recall traffic usage statistics for any period in time.  By default, tfstat saves network statistics in 5 minute intervals.  Given the 40-byte data structure being saved, if tfstat were left running for 5 years, the database file would only reach about 21MB of disk usage.  There will be an option to overwrite old database entries beyond a specified period.
