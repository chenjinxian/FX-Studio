--[[--
Front end for LuaMacro, a Lua macro preprocessor.

The default action is to preprocess and run a Lua file. To just dump
the preprocessed output, use the `-d` flag. Like `lua`, the `-l` flag can
be used to load a library first, but you need to explicitly say `-i` to
get an interactive prompt.

The package loader is modified so that `require 'mod'` will preprocess `mod` if it is found as `mod.m.lua`.

Dumping is the only action available when preprocessing C code with `-C`.

##Usage

    LuaMacro 2.3.0, a Lua macro preprocessor and runner
        -l  require a library
        -e  statement to be executed
        -c  error context to be shown (default 2)
        -d  dump preprocessed output to stdout
        -C  C lexer
        -N  No #line directives when generating C
        -i  interactive prompt
        <input>    Lua source file

@script luam
]]

-- adjust the path so that this script can see the macro package
local path = arg[0]:gsub('[^/\\]+$','')
package.path = path .. '?.lua;'..path .. 'macro/?.lua;' .. package.path
local macro = require 'macro'
require 'macro.builtin'

--- Using luam.
-- @usage follows
local usage = [[
LuaMacro 2.3.0, a Lua macro preprocessor and runner
    -l  require a library
    -e  statement to be executed
    -c  error context to be shown (default 2)
    -d  dump preprocessed output to stdout
    -C  C lexer
    -N  No #line directives when generating C
    -i  interactive prompt
    <input>    Lua source file
]]

-- parsing the args, the hard way:
local takes_value = {l = '', e = '', c = 2}

local args = {}
local idx,i = 1,1
while i <= #arg do
    local a = arg[i]
    local flag = a:match '^%-(.+)'
    local val
    if flag then
        if #flag > 1 then -- allow for -lmod, like Lua
            val = flag:sub(2)
            flag = flag:sub(1,1)
        end
        -- grab the next argument if we need a value
        if takes_value[flag] and not val then
            i = i + 1
            val = arg[i]
        end
        -- convert the argument, if required
        local def = takes_value[flag]
        if type(def) == 'number' then
            val = tonumber(val)
        end
        args[flag] = val or true
    else
        args[idx] = a
        idx = idx + 1
    end
    i = i + 1
end

if not args[1] and not args.i then
    print(usage)
    os.exit()
elseif args[1] then
    args.input_name = args[1]
    args.input,err = io.open(args[1],'r')
    if err then return print(err) end
    table.remove(args,1)
end
-- set defaults, if flags not specified
for k,v in pairs(takes_value) do
    if not args[k] then
        args[k] = v
    end
end

---------- compiling and running the output ------
-- the tricky bit here is presenting the errors so that they refer to the
-- original line numbers. In addition, we also present a few lines of context
-- in the output.

local function lookup_line (lno,li)
    for i = 1,#li-1 do
        --print(li[i].il,li[i].ol)
        if lno < li[i+1].ol then
            return li[i].il + (lno - li[i].ol) - 1
        end
    end
    return li[#li].il + lno - 1
end

-- iterating over all lines in a string can be awkward;
-- gmatch doesn't handle the empty-line cases properly.
local function split_nl (t)
    local k1 = 1
    local k2 = t:find ('[\r\n]',k1)
    return function()
        if not k2 then return nil end
        local res = t:sub(k1,k2-1)
        k1 = k2+1
        k2 = t:find('[\r\n]',k1)
        return res
    end
end

local function fix_error_trace (err,li)
    local strname,lno = err:match '%[string "(%S+)"%]:(%d+)'
    --print(strname,lno)
    local ino
    if strname then
        lno = tonumber(lno)
        if li then
            ino = lookup_line(lno,li)
            err = err:gsub('%[string "%S+"%]:'..(lno or '?')..':',strname..':'..(ino or '?'))
        end
    end
    return err,lno,ino
end

local function runstring (code,name,li,...)
    local res,err = loadstring(code,name)
    local lno,ok
    if not res then
        err,lno,ino = fix_error_trace(err,li)
        if ino then
            print 'preprocessed context of error:'
            local l1,l2 = lno-args.c,lno+args.c
            local l = 1
            for line in split_nl(code) do
                if l >= l1 and l <= l2 then
                    if l == lno then io.write('*') else io.write(' ') end
                    print(l,line)
                end
                l = l + 1
            end
        end
        io.stderr:write(err,'\n')
        os.exit(1)
    end
    ok,err = xpcall(function(...) return res(...) end, debug.traceback)
    if not ok then
        err = err:gsub("%[C%]: in function 'xpcall'.+",'')
        if li then
            repeat
                err,lno = fix_error_trace(err,li)
            until not lno
        end
        io.stderr:write(err,'\n')
    end
    return ok
end

local function subst (ins,name)
    local C
    if args.C then
        C = args.N and true or 'line'
    end
    return macro.substitute_tostring(ins,name,C)
end

local function subst_runstring (ins,name,...)
    local buf,li = subst(ins,name)
    if not buf then
        io.stderr:write(li,'\n')
        os.exit(1)
    end
    if args.d or args.C then
        print(buf)
    else
        return runstring(buf,name,li,...)
    end
end

-- Lua 5.1/5.2 compatibility
local pack = table.pack
if not pack then
    function pack(...)
        return {n=select('#',...),...}
    end
end
if not unpack then unpack = table.unpack end

local function eval(code)
    local status,val,f,err,rcnt
    code,rcnt = code:gsub('^%s*=','return ')
    f,err = loadstring(code,'TMP')
    if f then
        res = pack(pcall(f))
        if not res[1] then err = res[2]
        else
            return res
        end
    end
    if err then
        err = tostring(err):gsub('^%[string "TMP"%]:1:','')
        return {nil,err}
    end
end

local function interactive_loop ()
    os.execute(arg[-1]..' -v') -- for the Lua copyright
    print 'Lua Macro 2.3.0 Copyright (C) 2007-2011 Steve Donovan'

    local function readline()
        io.write(_PROMPT or '> ')
        return io.read()
    end

    require 'macro.all'
    _G.macro = macro
    macro.define 'quit os.exit()'
    macro._interactive = true

    local line = readline()
    while line do
        local s,err = subst(line..'\n')
        if not s then
            err = err:gsub('.-:%d+:','')
            print('macro error: '..err)
        elseif not s:match '^%s*$' then
            if args.d then print(s) end
            local res = eval(s)
            if not res[1] then
                print('expanded: '..s)
                print('error: '..res[2])
            elseif res[2] ~= nil then
                print(unpack(res,2))
           end
        end
        line = readline()
    end
end

macro.set_package_loader()

if args.l ~= '' then require(args.l) end

if args.e ~= '' then
    subst_runstring(args.e,"<temp>")
else
    if args.input then
        arg = args
        arg[0] = args.input_name
        arg[-1] = 'luam'
        subst_runstring(args.input,args.input_name,unpack(args))
    elseif args.i then
        interactive_loop()
    end
end
