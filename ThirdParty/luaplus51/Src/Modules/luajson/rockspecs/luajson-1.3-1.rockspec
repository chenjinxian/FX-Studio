package = "luajson"
version = "1.3-1"
source = {
	url = "http://cloud.github.com/downloads/harningt/luajson/luajson-1.3.tar.gz",
	md5 = "18a59640119143ea69b3744b467e1a89"
}
description = {
	summary = "customizable JSON decoder/encoder",
	detailed = [[
		LuaJSON is a customizable JSON decoder/encoder using
		LPEG for parsing.
	]],
	homepage = "http://github.com/harningt/luajson",
	maintainer = "Thomas Harning <harningt@gmail.com>",
	license = "MIT/X11"
}
dependencies = {
	"lua >= 5.1",
	"lunit >= 0.4",
	"lpeg >= 0.8.1"
}
build = {
	type = "module",
	modules = {
		["json"] = "lua/json.lua",
		["json.decode"] = "lua/json/decode.lua",
		["json.decode.composite"] = "lua/json/decode/composite.lua",
		["json.decode.number"] = "lua/json/decode/number.lua",
		["json.decode.others"] = "lua/json/decode/others.lua",
		["json.decode.state"] = "lua/json/decode/state.lua",
		["json.decode.strings"] = "lua/json/decode/strings.lua",
		["json.decode.util"] = "lua/json/decode/util.lua",
		["json.encode"] = "lua/json/encode.lua",
		["json.encode.array"] = "lua/json/encode/array.lua",
		["json.encode.calls"] = "lua/json/encode/calls.lua",
		["json.encode.number"] = "lua/json/encode/number.lua",
		["json.encode.object"] = "lua/json/encode/object.lua",
		["json.encode.others"] = "lua/json/encode/others.lua",
		["json.encode.output"] = "lua/json/encode/output.lua",
		["json.encode.output_utility"] = "lua/json/encode/output_utility.lua",
		["json.encode.strings"] = "lua/json/encode/strings.lua",
		["json.util"] = "lua/json/util.lua"
	}
}

