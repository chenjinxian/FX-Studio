local json = require("json")
local lunit = require("lunit")

-- Test module for handling the simple decoding that behaves more like expected
if not module then
    _ENV = lunit.module("lunit-simple-decode", 'seeall')
else
    module("lunit-simple-decode", lunit.testcase, package.seeall)
end

function test_decode_simple_undefined()
	assert_nil(json.decode('undefined', json.decode.simple))
end
function test_decode_default_undefined()
	assert_equal(json.util.undefined, json.decode('undefined'))
end

function test_decode_simple_null()
	assert_nil(json.decode('null', json.decode.simple))
end

function test_decode_default_null()
	assert_equal(json.util.null, json.decode('null'))
end

function test_decode_array_simple_with_only_null()
	local result = assert(json.decode('[null]', json.decode.simple))
	assert_nil(result[1])
	assert_equal(1, result.n)
end

function test_decode_array_default_with_only_null()
	local result = assert(json.decode('[null]'))
	assert_equal(json.util.null, result[1])
	assert_equal(1, #result)
end

function test_decode_array_simple_with_null()
	local result = assert(json.decode('[1, null, 3]', json.decode.simple))
	assert_equal(1, result[1])
	assert_nil(result[2])
	assert_equal(3, result[3])
	assert_equal(3, result.n)
end

function test_decode_array_default_with_null()
	local result = assert(json.decode('[1, null, 3]'))
	assert_equal(1, result[1])
	assert_equal(json.util.null, result[2])
	assert_equal(3, result[3])
	assert_equal(3, #result)
end

function test_decode_small_array_simple_with_trailing_null()
	local result = assert(json.decode('[1, null]', json.decode.simple))
	assert_equal(1, result[1])
	assert_nil(result[2])
	assert_equal(2, result.n)
end

function test_decode_small_array_default_with_trailing_null()
	local result = assert(json.decode('[1, null]'))
	assert_equal(1, result[1])
	assert_equal(json.util.null, result[2])
	assert_equal(2, #result)
end


function test_decode_small_array_simple_with_trailing_null()
	local result = assert(json.decode('[1, null]', json.decode.simple))
	assert_equal(1, result[1])
	assert_nil(result[2])
	assert_equal(2, result.n)
end

function test_decode_small_array_default_with_trailing_null()
	local result = assert(json.decode('[1, null]'))
	assert_equal(1, result[1])
	assert_equal(json.util.null, result[2])
	assert_equal(2, #result)
end


function test_decode_array_simple_with_trailing_null()
	local result = assert(json.decode('[1, null, 3, null]', json.decode.simple))
	assert_equal(1, result[1])
	assert_nil(result[2])
	assert_equal(3, result[3])
	assert_nil(result[4])
	assert_equal(4, result.n)
end

function test_decode_array_default_with_trailing_null()
	local result = assert(json.decode('[1, null, 3, null]'))
	assert_equal(1, result[1])
	assert_equal(json.util.null, result[2])
	assert_equal(3, result[3])
	assert_equal(json.util.null, result[4])
	assert_equal(4, #result)
end


function test_decode_object_simple_with_null()
	local result = assert(json.decode('{x: null}', json.decode.simple))
	assert_nil(result.x)
	assert_nil(next(result))
end

function test_decode_object_default_with_null()
	local result = assert(json.decode('{x: null}'))
	assert_equal(json.util.null, result.x)
	assert_not_nil(next(result))
end

function test_decode_object_with_stringized_numeric_keys_default()
	local result = assert(json.decode('{"1": "one"}'))
	assert_equal("one", result["1"])
	assert_equal(nil, result[1])
end

function test_decode_object_with_stringized_numeric_keys_force_numeric()
	local result = assert(
			json.decode(
					'{"1": "one"}',
					{ object = { setObjectKey = assert(json.decode.util.setObjectKeyForceNumber) } }
				)
		)
	assert_equal(nil, result["1"])
	assert_equal("one", result[1])
end
