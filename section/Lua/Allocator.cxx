#include "Allocator.h"
#include "MemoryPool.h"

void *__cdecl MP_ReallocFunction(
    void *ptr,
    unsigned int oldsize,
    unsigned int size,
    void *data,
    const char *allocName,
    unsigned int flags)
{
    // LogF("%u->%u", oldsize, size);
    MemoryPool *pool = (MemoryPool *)data;

    return pool->Reallocate(ptr, size);
}

void __cdecl MP_FreeFunction(void *ptr, unsigned int oldsize, void *data)
{
    MemoryPool *pool = (MemoryPool *)data;
    pool->Free(ptr);
}

size_t distrib[1024]{};

/*
0: 0
1: 0
2: 0
3: 0
4: 2605
5: 0
6: 0
7: 0
8: 107554
9: 0
10: 0
11: 0
12: 2553
13: 0
14: 0
15: 0
16: 106528
17: 0
18: 0
19: 0
20: 47980
21: 1
22: 91
23: 503
24: 4876
25: 847
26: 912
27: 1761
28: 5651
29: 1712
30: 1579
31: 2043
32: 107938
33: 3283
34: 2494
35: 1360
36: 510709
37: 1609
38: 1259
39: 1178
40: 332648
41: 1093
42: 1491
43: 861
44: 2650
45: 735
46: 814
47: 714
48: 11750
49: 696
50: 785
51: 1229
52: 2131
53: 632
54: 2884
55: 615
56: 4013
57: 1092
58: 642
59: 1043
60: 1525
61: 1004
62: 439
63: 409
64: 48530
65: 529
66: 488
67: 478
68: 1000
69: 1183
70: 704
71: 511
72: 1417
73: 460
74: 433
75: 488
76: 892
77: 502
78: 428
79: 384
80: 285861
81: 410
82: 449
83: 377
84: 816
85: 447
86: 396
87: 408
88: 826
89: 298
90: 255
91: 206
92: 435
93: 205
94: 196
95: 164
96: 2149
97: 189
98: 154
99: 172
100: 342
101: 142
102: 61
103: 70
104: 273
105: 79
106: 51
107: 48
108: 324
109: 75
110: 58
111: 36
112: 6905
113: 44
114: 49
115: 39
116: 266
117: 33
118: 39
119: 39
120: 315
121: 27
122: 31
123: 37
124: 145
125: 27
126: 33
127: 21
128: 13728
129: 25
130: 29
131: 27
132: 177
133: 19
134: 22
135: 18
136: 174
137: 22
138: 11
139: 13
140: 114
141: 14
142: 10
143: 13
144: 309
145: 19
146: 17
147: 21
148: 194
149: 24
150: 20
151: 19
152: 131
153: 15
154: 12
155: 16
156: 147
157: 12
158: 19
159: 7
160: 114016
161: 10
162: 19
163: 11
164: 98
165: 9
166: 20
167: 9
168: 178
169: 23
170: 7
171: 12
172: 121
173: 12
174: 12
175: 8
176: 122
177: 9
178: 7
179: 7
180: 82
181: 7
182: 7
183: 11
184: 148
185: 10
186: 6
187: 9
188: 49
189: 3
190: 3
191: 13
192: 701
193: 3
194: 17
195: 8
196: 50
197: 7
198: 3
199: 1
200: 148
201: 2
202: 11
203: 8
204: 69
205: 1
206: 5
207: 7
208: 95
209: 3
210: 2
211: 1
212: 39
213: 4
214: 5
215: 4
216: 95
217: 5
218: 7
219: 6
220: 55
221: 3
222: 7
223: 5
224: 67
225: 7
226: 1
227: 6
228: 82
229: 5
230: 1
231: 2
232: 60
233: 0
234: 0
235: 3
236: 23
237: 0
238: 2
239: 0
240: 42
241: 1
242: 1
243: 2
244: 31
245: 2
246: 2
247: 5
248: 45
249: 2
250: 1
251: 0
252: 47
253: 2
254: 3
255: 2
256: 7204
*/

/*
40, 80, 160, 320 - table
not mult of 4/8 - string
8 - userdata
32 - function
*/

void *__cdecl my_ReallocFunction(
    void *ptr,
    unsigned int oldsize,
    unsigned int size,
    void *data,
    const char *allocName,
    unsigned int flags)
{
    // if (size < sizeof(distrib) / sizeof(distrib[0]))
    // {
    //     distrib[size]++;
    // }
    if (oldsize && size && size % 12 == 0)
    {
        string result = Moho__GetCallStack();
        LogF("%s", result.data());
    }
    return realloc(ptr, size);
}

void __cdecl my_FreeFunction(void *ptr, unsigned int oldsize, void *data)
{
    free(ptr);
}

// class exception
// {
// private:
//     char *msg;
//     int complete;

// public:
//     exception() : msg(nullptr), complete(0) {}
//     exception(const char *msg) : exception{}
//     {
//         if (msg != nullptr)
//         {
//             this->msg = new char[strlen(msg) + 1];
//             _strcpy(this->msg, msg);
//         }
//     }

//     virtual const char *what() const { return msg; }
//     virtual ~exception()
//     {
//         delete[] msg;
//     }
// };

SHARED LuaState *__thiscall UI_StateCreate(LuaState *_this, StandardLibraries libs)
{
    LogF("UI_StateCreate: %p", _this);
    // string result = Moho__GetCallStack();
    // LogF("%s", result.data());
    // throw std::exception("UI_StateCreate hi");
    // catch(const stdexception& e)
    // {
    //     WarningF("Catch: %s", e.what());
    // }

    MemoryPool *pool = new (std::nothrow) MemoryPool(0x4000000, 0x1000000);

    lua_setdefaultmemoryfunctions(my_ReallocFunction, my_FreeFunction, pool);
    // lua_setdefaultmemoryfunctions(MP_ReallocFunction, MP_FreeFunction, pool);

    new (_this) LuaState(libs);

    lua_setdefaultmemoryfunctions(nullptr, nullptr, nullptr);

    return _this;
}

SHARED void __thiscall UI_StateDestroy(LuaState *_this)
{
    LogF("UI_StateDestroy: %p", _this);
    void *memData = lua_getMemData(_this->m_state);
    _this->~LuaState();
    if (memData)
    {
        delete (MemoryPool *)memData;
    }

    LogF("Distribution:");
    for (int i = 0; i < sizeof(distrib) / sizeof(distrib[0]); i++)
    {
        LogF("%d: %d", i, distrib[i]);
    }
}

SHARED LuaState *__thiscall SIM_StateCreate(LuaState *_this, StandardLibraries libs)
{
    LogF("SIM_StateCreate: %p", _this);

    MemoryPool *pool = new (std::nothrow) MemoryPool(0x10000000);

    // lua_setdefaultmemoryfunctions(my_ReallocFunction, my_FreeFunction, pool);

    new (_this) LuaState(libs);

    // lua_setdefaultmemoryfunctions(nullptr, nullptr, nullptr);

    return _this;
}

SHARED void __thiscall SIM_StateDestroy(LuaState *_this)
{
    LogF("SIM_StateDestroy: %p", _this);
    void *memData = lua_getMemData(_this->m_state);
    _this->~LuaState();
    if (memData)
    {
        delete (MemoryPool *)memData;
    }
}

/*
    UI_Lua local x,y,z,w,v  LOG(debug.allocatedsize(function() return x,y,z,w,v end))
*/