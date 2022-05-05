#include <iostream>
#include <list>
#include <unordered_map>
#include "rbthash_map.h"

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
#define TEST_COUNT 100000
#define REMOVE_COUNT 50000

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

void testInsert()
{
    printf("==========================test insert start===================================\n");
    RbtHashMap<int,ValueType,TEST_COUNT>* testMap = new RbtHashMap<int,ValueType,TEST_COUNT>();
    std::unordered_map<int,ValueType> stdmap;
    while(true)
    {
        int key = rand();
        if(stdmap.find(key) == stdmap.end())
        {
            stdmap.insert(std::make_pair(key,ValueType(key)));
            testMap->insert(key,key);
        }

        if(stdmap.size() >= TEST_COUNT)
        {
            break;
        }
    }

    if(stdmap.size() != testMap->size())
    {
        printf("test insert failed stdsize = %d,rbtsize = %d\n",stdmap.size(),testMap->size());
        exit(0);
    }

    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it = testMap->begin();
    for(;it != testMap->end();it++)
    {
        std::unordered_map<int,ValueType>::iterator  stdit = stdmap.find(it->first);
        if(stdit == stdmap.end())
        {
            printf("test insert failed never insert key = %d\n",it->first);
            exit(0);
        }
        if(it->second->a != stdit->second.a)
        {
            printf("test insert failed stdvalue= %d,rbtvalue = %d\n",stdit->second.a,it->second->a);
            exit(0);
        }
    }
    printf("==========================test insert done===================================\n");
    delete testMap;
    testMap = NULL;
}

void testremove()
{
    printf("==========================test remove begin===================================\n");
    RbtHashMap<int,ValueType,TEST_COUNT>* testMap = new RbtHashMap<int,ValueType,TEST_COUNT>();
    std::unordered_map<int,ValueType> stdmap;
    while(true)
    {
        int key = rand();
        if(stdmap.find(key) == stdmap.end())
        {
            stdmap.insert(std::make_pair(key,ValueType(key)));
            testMap->insert(key,key);
        }

        if(stdmap.size() >= TEST_COUNT)
        {
            break;
        }
    }

    std::unordered_map<int,ValueType>::iterator stdit = stdmap.begin();
    int count = 0;
    for(;stdit != stdmap.end();)
    {
        if(!testMap->erase(stdit->first))
        {
            printf("test remove failed never remove key = %d\n",stdit->first);
            exit(0);
        }
        stdit = stdmap.erase(stdit);
        count++;
        if(count >= REMOVE_COUNT)
        {
            break;
        }
    }

    if(stdmap.size() != testMap->size())
    {
        printf("test remove failed stdsize = %d,rbtsize = %d\n",stdmap.size(),testMap->size());
        exit(0);
    }

    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it = testMap->begin();
    count = 0;
    for(;it != testMap->end();it++)
    {
        count++;
        std::unordered_map<int,ValueType>::iterator  stdit = stdmap.find(it->first);
        if(stdit == stdmap.end())
        {
            printf("test remove failed never lost key = %d\n",it->first);
            exit(0);
        }
        if(it->second->a != stdit->second.a)
        {
            printf("test remove failed stdvalue= %d,rbtvalue = %d\n",stdit->second.a,it->second->a);
            exit(0);
        }
    }
    printf("==========================test remove done===================================\n");
    delete testMap;
    testMap = NULL;
}

int main()
{
    testInsert();
    testremove();
    std::cout << "Test Done,Hello, World!" << std::endl;

}
