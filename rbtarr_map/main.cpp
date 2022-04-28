#include <iostream>
#include <list>
#include "rbthash_map.h"
#include "rb_tree.h"

/**
 * https://zhuanlan.zhihu.com/p/31805309
 * https://blog.csdn.net/m0_49374492/article/details/119045823
 * https://zhuanlan.zhihu.com/p/31758048
 * https://www.cnblogs.com/skywang12345/p/3624291.html
 * https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
 * https://www.cnblogs.com/skywang12345/p/3624202.html
 * @return
 */

using namespace rbt_hash;
#define HASH_CONFLICT_RATE 4   //hash冲突倍率
#define TEST_COUNT 1000

class ValueType
{
public:
    ValueType(int value) : a(value)
    {
    }

    ~ValueType()
    {
    }

    int Key()
    {
        return  a;
    }

    bool IsValid()
    {
        return  a >= 0;
    }

    int a;
};

int main()
{
    RbtHashMap<int,ValueType,TEST_COUNT>* testMap = new RbtHashMap<int,ValueType,TEST_COUNT>();

    for(int i = 0;i < TEST_COUNT;i++)
    {
        testMap->insert(i * HASH_CONFLICT_RATE,ValueType(i * HASH_CONFLICT_RATE));
    }
    //testMap->insert(99999,ValueType(99999));
    RbtHashMap<int,ValueType,TEST_COUNT>* test1Map = new RbtHashMap<int,ValueType,TEST_COUNT>();
    //调用memcpy复制rtbhashmap,验证二进制可复制性
    //memcpy((char*)test1Map,(char*)testMap,sizeof(RbtHashMap<int,ValueType,TEST_COUNT>));
    test1Map->initFromBinaryData(testMap->data());
    int same = memcmp((char*)test1Map,(char*)testMap,sizeof(RbtHashMap<int,ValueType,TEST_COUNT>));
    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it = test1Map->begin();
    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it1 = test1Map->find(1);
    for(;it != test1Map->end();it++)
    {
        int a = it->second->a;
        std::cout << "it->first = " << it->first << " it->second = " << a << std::endl;
    }

    printf("same = %d",same);
    testMap->erase(HASH_CONFLICT_RATE * 1);
    std::cout << "Hello, World!" << std::endl;
    delete testMap;
    testMap = NULL;
    delete test1Map;
    test1Map = NULL;
    return 0;
}
