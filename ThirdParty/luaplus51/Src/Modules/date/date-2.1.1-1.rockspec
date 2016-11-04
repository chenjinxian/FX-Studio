package = "date"
version = "2.1.1-1"

description = {
   summary = "Date & Time module for Lua 5.x",
   detailed = [[
      Pure Lua Date & Time module for Lua 5.x featuring date and Time string
      parsing, time addition & subtraction, time span calculation, support for
      ISO 8601 Dates, local time support, strftime-like formatting.
   ]],
   license = "MIT",
   homepage = "https://github.com/Tieske/date",
}

dependencies = {
   "lua >= 5.0, < 5.3 "
}

source = {
   url = "https://github.com/Tieske/date/archive/version_2.1.1.tar.gz",
   dir = "date-version_2.1.1",
}

build = {
   type = "builtin",
   modules = {
      date = "date.lua"
   }
}

