package = "Coxpcall"

version = "1.11.0-1"

description = {
  summary = "Coroutine safe xpcall and pcall",
  detailed = [[
 Encapsulates the protected calls with a coroutine based loop, so errors can
 be dealed without the usual Lua 5.x pcall/xpcall issues with coroutines
 yielding inside the call to pcall or xpcall.
  ]],
  license = "MIT/X11",
  homepage = "http://coxpcall.luaforge.net"
}

dependencies = { }

source = {
  url = "http://luaforge.net/frs/download.php/3047/coxpcall-1.11.0.tar.gz",
}

build = {
   type = "make",
   build_pass = false,
   install_target = "install-rocks",
   install_variables = {
     PREFIX  = "$(PREFIX)",
     LUA_DIR = "$(LUADIR)",
     BIN_DIR = "$(BINDIR)"
   }
}
