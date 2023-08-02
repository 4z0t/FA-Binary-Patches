#define CSTRINGCAST(addr) reinterpret_cast<const char*>(addr) 

#define ExpectedButGot CSTRINGCAST(0x00E0A220) // "%s\n  expected %d args, but got %d"
