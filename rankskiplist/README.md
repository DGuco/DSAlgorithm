# 排行榜模板类
- 一个快速高效的排行榜模板类，可以快速的查找排名和查找范围排名排行榜的需求
# 实现原理
- 根据redis zset数据结构实现,redis zset算是skiplist的变种，在skiplist基础上做了一些优化，可以快速的查找指定排名的数据.
- [什么是skiplist](https://blog.csdn.net/D_Guco/article/details/78004991)
- [基于skiplist实现高效的排行算法](https://blog.csdn.net/D_Guco/article/details/103950188)
# 测试用例
- 经典的成绩排名问题,先比总分，如果总分相等，则语文成绩好的排名靠前，如果语文也相等则比数学......
- [源码test.cpp](https://github.com/DGuco/rankskiplist/blob/master/test.cpp)
- 测试结果：
```
rank = 1 id = 102,sum = 300,chinese = 100,math = 100,english = 100
rank = 2 id = 1,sum = 299,chinese = 100,math = 99,english = 100
rank = 3 id = 56080,sum = 298,chinese = 100,math = 98,english = 100
rank = 4 id = 7,sum = 298,chinese = 99,math = 99,english = 100
rank = 5 id = 8,sum = 297,chinese = 99,math = 99,english = 99
rank = 6 id = 5,sum = 297,chinese = 99,math = 99,english = 99
rank = 7 id = 2,sum = 297,chinese = 99,math = 99,english = 99
rank = 8 id = 6,sum = 296,chinese = 100,math = 98,english = 98
rank = 9 id = 31384,sum = 296,chinese = 100,math = 96,english = 100
rank = 10 id = 91781,sum = 295,chinese = 99,math = 97,english = 99
rank = 11 id = 98714,sum = 295,chinese = 96,math = 99,english = 100
rank = 12 id = 26255,sum = 294,chinese = 100,math = 99,english = 95
rank = 13 id = 7479,sum = 294,chinese = 97,math = 99,english = 98
rank = 14 id = 121,sum = 293,chinese = 100,math = 98,english = 95
rank = 15 id = 3047,sum = 293,chinese = 98,math = 95,english = 100
rank = 16 id = 27290,sum = 293,chinese = 97,math = 96,english = 100
rank = 17 id = 61818,sum = 293,chinese = 95,math = 99,english = 99
rank = 18 id = 90104,sum = 293,chinese = 95,math = 98,english = 100
rank = 19 id = 71132,sum = 293,chinese = 93,math = 100,english = 100
rank = 20 id = 96104,sum = 292,chinese = 98,math = 99,english = 95
rank = 21 id = 85784,sum = 292,chinese = 97,math = 100,english = 95
rank = 22 id = 66898,sum = 292,chinese = 97,math = 100,english = 95
rank = 23 id = 64704,sum = 292,chinese = 96,math = 96,english = 100
rank = 24 id = 64927,sum = 291,chinese = 100,math = 100,english = 91
rank = 25 id = 33759,sum = 291,chinese = 100,math = 96,english = 95
rank = 26 id = 8048,sum = 291,chinese = 99,math = 99,english = 93
rank = 27 id = 32766,sum = 291,chinese = 99,math = 94,english = 98
rank = 28 id = 11465,sum = 291,chinese = 97,math = 95,english = 99
rank = 29 id = 68636,sum = 291,chinese = 95,math = 98,english = 98
rank = 30 id = 84196,sum = 291,chinese = 95,math = 97,english = 99
rank = 31 id = 52630,sum = 290,chinese = 100,math = 93,english = 97
rank = 32 id = 46088,sum = 290,chinese = 99,math = 98,english = 93
rank = 33 id = 34454,sum = 290,chinese = 97,math = 95,english = 98
rank = 34 id = 27265,sum = 290,chinese = 96,math = 100,english = 94
rank = 35 id = 13197,sum = 290,chinese = 96,math = 99,english = 95
rank = 36 id = 51281,sum = 290,chinese = 95,math = 98,english = 97
rank = 37 id = 76302,sum = 290,chinese = 92,math = 99,english = 99
rank = 38 id = 7749,sum = 290,chinese = 91,math = 99,english = 100
rank = 39 id = 25967,sum = 289,chinese = 100,math = 98,english = 91
rank = 40 id = 22466,sum = 289,chinese = 100,math = 97,english = 92
rank = 41 id = 54673,sum = 289,chinese = 100,math = 96,english = 93
rank = 42 id = 7478,sum = 289,chinese = 99,math = 95,english = 95
rank = 43 id = 59210,sum = 289,chinese = 98,math = 99,english = 92
rank = 44 id = 37475,sum = 289,chinese = 98,math = 99,english = 92
rank = 45 id = 11500,sum = 289,chinese = 97,math = 95,english = 97
rank = 46 id = 65394,sum = 289,chinese = 96,math = 98,english = 95
rank = 47 id = 78608,sum = 289,chinese = 95,math = 94,english = 100
rank = 48 id = 18274,sum = 289,chinese = 93,math = 96,english = 100
rank = 49 id = 84211,sum = 289,chinese = 92,math = 100,english = 97
rank = 50 id = 15084,sum = 288,chinese = 100,math = 97,english = 91
rank = 51 id = 99803,sum = 288,chinese = 100,math = 93,english = 95
rank = 52 id = 16916,sum = 288,chinese = 100,math = 90,english = 98
rank = 53 id = 51229,sum = 288,chinese = 99,math = 96,english = 93
rank = 54 id = 93340,sum = 288,chinese = 99,math = 90,english = 99
rank = 55 id = 43355,sum = 288,chinese = 98,math = 97,english = 93
rank = 56 id = 37554,sum = 288,chinese = 98,math = 95,english = 95
rank = 57 id = 49365,sum = 288,chinese = 95,math = 97,english = 96
rank = 58 id = 27443,sum = 288,chinese = 95,math = 93,english = 100
rank = 59 id = 36595,sum = 288,chinese = 94,math = 94,english = 100
rank = 60 id = 29627,sum = 288,chinese = 92,math = 96,english = 100
rank = 61 id = 44584,sum = 287,chinese = 100,math = 88,english = 99
rank = 62 id = 40889,sum = 287,chinese = 100,math = 88,english = 99
rank = 63 id = 53256,sum = 287,chinese = 98,math = 97,english = 92
rank = 64 id = 58268,sum = 287,chinese = 98,math = 92,english = 97
rank = 65 id = 94537,sum = 287,chinese = 98,math = 91,english = 98
rank = 66 id = 94201,sum = 287,chinese = 98,math = 91,english = 98
rank = 67 id = 47096,sum = 287,chinese = 97,math = 99,english = 91
rank = 68 id = 44196,sum = 287,chinese = 96,math = 94,english = 97
rank = 69 id = 96168,sum = 287,chinese = 96,math = 92,english = 99
rank = 70 id = 72978,sum = 287,chinese = 95,math = 92,english = 100
rank = 71 id = 92800,sum = 287,chinese = 94,math = 99,english = 94
rank = 72 id = 49251,sum = 287,chinese = 94,math = 97,english = 96
rank = 73 id = 885,sum = 287,chinese = 93,math = 99,english = 95
rank = 74 id = 33986,sum = 287,chinese = 90,math = 98,english = 99
rank = 75 id = 25072,sum = 287,chinese = 89,math = 99,english = 99
rank = 76 id = 89575,sum = 286,chinese = 99,math = 100,english = 87
rank = 77 id = 3951,sum = 286,chinese = 99,math = 100,english = 87
rank = 78 id = 43215,sum = 286,chinese = 99,math = 95,english = 92
rank = 79 id = 3440,sum = 286,chinese = 98,math = 99,english = 89
rank = 80 id = 72227,sum = 286,chinese = 97,math = 92,english = 97
rank = 81 id = 40079,sum = 286,chinese = 97,math = 91,english = 98
rank = 82 id = 46064,sum = 286,chinese = 96,math = 90,english = 100
rank = 83 id = 30246,sum = 286,chinese = 95,math = 98,english = 93
rank = 84 id = 26361,sum = 286,chinese = 95,math = 97,english = 94
rank = 85 id = 8455,sum = 286,chinese = 95,math = 95,english = 96
rank = 86 id = 82829,sum = 286,chinese = 95,math = 92,english = 99
rank = 87 id = 16834,sum = 286,chinese = 94,math = 92,english = 100
rank = 88 id = 94755,sum = 286,chinese = 93,math = 97,english = 96
rank = 89 id = 96753,sum = 286,chinese = 92,math = 99,english = 95
rank = 90 id = 68859,sum = 286,chinese = 92,math = 98,english = 96
rank = 91 id = 91050,sum = 286,chinese = 88,math = 99,english = 99
rank = 92 id = 61427,sum = 285,chinese = 100,math = 91,english = 94
rank = 93 id = 92743,sum = 285,chinese = 99,math = 93,english = 93
rank = 94 id = 69197,sum = 285,chinese = 99,math = 92,english = 94
rank = 95 id = 21456,sum = 285,chinese = 99,math = 86,english = 100
rank = 96 id = 77305,sum = 285,chinese = 97,math = 98,english = 90
rank = 97 id = 69454,sum = 285,chinese = 97,math = 89,english = 99
rank = 98 id = 75610,sum = 285,chinese = 96,math = 90,english = 99
rank = 99 id = 10480,sum = 285,chinese = 94,math = 97,english = 94
rank = 100 id = 44309,sum = 285,chinese = 93,math = 100,english = 92
===========================================================================
rank = 1 id = 99803,sum = 288,chinese = 100,math = 93,english = 95
rank = 2 id = 16916,sum = 288,chinese = 100,math = 90,english = 98
rank = 3 id = 51229,sum = 288,chinese = 99,math = 96,english = 93
rank = 4 id = 93340,sum = 288,chinese = 99,math = 90,english = 99
rank = 5 id = 43355,sum = 288,chinese = 98,math = 97,english = 93
rank = 6 id = 37554,sum = 288,chinese = 98,math = 95,english = 95
rank = 7 id = 49365,sum = 288,chinese = 95,math = 97,english = 96
rank = 8 id = 27443,sum = 288,chinese = 95,math = 93,english = 100
rank = 9 id = 36595,sum = 288,chinese = 94,math = 94,english = 100
rank = 10 id = 29627,sum = 288,chinese = 92,math = 96,english = 100
rank = 11 id = 44584,sum = 287,chinese = 100,math = 88,english = 99
rank = 12 id = 40889,sum = 287,chinese = 100,math = 88,english = 99
rank = 13 id = 53256,sum = 287,chinese = 98,math = 97,english = 92
rank = 14 id = 58268,sum = 287,chinese = 98,math = 92,english = 97
rank = 15 id = 94537,sum = 287,chinese = 98,math = 91,english = 98
rank = 16 id = 94201,sum = 287,chinese = 98,math = 91,english = 98
rank = 17 id = 47096,sum = 287,chinese = 97,math = 99,english = 91
rank = 18 id = 44196,sum = 287,chinese = 96,math = 94,english = 97
rank = 19 id = 96168,sum = 287,chinese = 96,math = 92,english = 99
rank = 20 id = 72978,sum = 287,chinese = 95,math = 92,english = 100
rank = 21 id = 92800,sum = 287,chinese = 94,math = 99,english = 94
rank = 22 id = 49251,sum = 287,chinese = 94,math = 97,english = 96
rank = 23 id = 885,sum = 287,chinese = 93,math = 99,english = 95
rank = 24 id = 33986,sum = 287,chinese = 90,math = 98,english = 99
rank = 25 id = 25072,sum = 287,chinese = 89,math = 99,english = 99
rank = 26 id = 89575,sum = 286,chinese = 99,math = 100,english = 87
rank = 27 id = 3951,sum = 286,chinese = 99,math = 100,english = 87
rank = 28 id = 43215,sum = 286,chinese = 99,math = 95,english = 92
rank = 29 id = 3440,sum = 286,chinese = 98,math = 99,english = 89
rank = 30 id = 72227,sum = 286,chinese = 97,math = 92,english = 97
rank = 31 id = 40079,sum = 286,chinese = 97,math = 91,english = 98
rank = 32 id = 46064,sum = 286,chinese = 96,math = 90,english = 100
rank = 33 id = 30246,sum = 286,chinese = 95,math = 98,english = 93
rank = 34 id = 26361,sum = 286,chinese = 95,math = 97,english = 94
rank = 35 id = 8455,sum = 286,chinese = 95,math = 95,english = 96
rank = 36 id = 82829,sum = 286,chinese = 95,math = 92,english = 99
rank = 37 id = 16834,sum = 286,chinese = 94,math = 92,english = 100
rank = 38 id = 94755,sum = 286,chinese = 93,math = 97,english = 96
rank = 39 id = 96753,sum = 286,chinese = 92,math = 99,english = 95
rank = 40 id = 68859,sum = 286,chinese = 92,math = 98,english = 96
rank = 41 id = 91050,sum = 286,chinese = 88,math = 99,english = 99
rank = 42 id = 61427,sum = 285,chinese = 100,math = 91,english = 94
rank = 43 id = 92743,sum = 285,chinese = 99,math = 93,english = 93
rank = 44 id = 69197,sum = 285,chinese = 99,math = 92,english = 94
rank = 45 id = 21456,sum = 285,chinese = 99,math = 86,english = 100
rank = 46 id = 77305,sum = 285,chinese = 97,math = 98,english = 90
rank = 47 id = 69454,sum = 285,chinese = 97,math = 89,english = 99
rank = 48 id = 75610,sum = 285,chinese = 96,math = 90,english = 99
rank = 49 id = 10480,sum = 285,chinese = 94,math = 97,english = 94
rank = 50 id = 44309,sum = 285,chinese = 93,math = 100,english = 92
===========================================================================
id = 29627,sum = 288,chinese = 92,math = 96,english = 100
id = 44584,sum = 287,chinese = 100,math = 88,english = 99
id = 40889,sum = 287,chinese = 100,math = 88,english = 99
id = 53256,sum = 287,chinese = 98,math = 97,english = 92
id = 58268,sum = 287,chinese = 98,math = 92,english = 97
id = 94537,sum = 287,chinese = 98,math = 91,english = 98
```
# 注意事项
- 一般的排行榜没有这种内存排行的需求，都是基于redis来做，基本上只有游戏项目会用到，游戏项目为了省去网络的消耗，都是基于内存的排行榜,除非要做跨服排行
- 排行榜的SCORE_TYPE如果是自定义类型必须实现比较操作符　> == <，
- 排行榜的MEMBER_TYPE是unordered_map的key,如果是自定义类型必须自己实现hash函数(百度：自定义类型做unordered_map的key)
- 排行榜的SCORE_TYPE和MEMBER_TYPE如果是自定义类型原则上是pod类型的，可以内存拷贝的对象，浅拷贝不会又内存泄露问题