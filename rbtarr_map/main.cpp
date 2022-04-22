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
    int a;
};

int main()
{
    RbtHashMap<int,ValueType,TEST_COUNT>* testMap = new RbtHashMap<int,ValueType,TEST_COUNT>();
    for(int i = 0;i < TEST_COUNT;i++)
    {
        testMap->insert(i * HASH_CONFLICT_RATE,ValueType(i * HASH_CONFLICT_RATE));
    }
    testMap->insert(9999999999,ValueType(999999999));

    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it = testMap->begin();
    for(;it != testMap->end();it++)
    {
        int a = it->second->a;
        std::cout << "it->first = " << it->first << " it->second = " << a << std::endl;
    }
    testMap->erase(HASH_CONFLICT_RATE * 1);
    std::cout << "Hello, World!" << std::endl;
    delete testMap;
    testMap = NULL;
    return 0;
}
