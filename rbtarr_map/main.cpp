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

void testRBTree()
{
    RBTNode<int,int> nodeArr[10];
    int a[]= {10, 40, 30, 60, 90, 70, 20, 50, 80};
    int check_insert=1;    // "插入"动作的检测开关(0，关闭；1，打开)
    int check_remove=1;    // "删除"动作的检测开关(0，关闭；1，打开)
    int i;
    int ilen = (sizeof(a)) / (sizeof(a[0])) ;
    RBTree<int,int,unsigned int,10>* tree = new RBTree<int,int,unsigned int,10>(nodeArr);

    cout << "== 原始数据: ";
    for(i=0; i<ilen; i++)
    {
        cout << a[i] <<" ";
        nodeArr[i].set_key(a[i]);
    }
    cout << endl;

    for(i=0; i<ilen; i++)
    {
        tree->insert(&nodeArr[i]);
        // 设置check_insert=1,测试"添加函数"
        if(check_insert)
        {
            cout << "== 添加节点: " << a[i] << endl;
            cout << "== 树的详细信息: " << endl;
            if(!tree->isRBTree())
            {
                tree->print();
                cout << "== RBTree insert failed " << endl;
            }
            cout << endl;
        }

    }

    cout << "== 前序遍历: ";
    std::list<RBTNode<int,int>*> resList;
    resList.clear();
    tree->preOrder(resList);

    resList.clear();
    cout << "\n== 中序遍历: ";
    tree->inOrder(resList);

    resList.clear();
    cout << "\n== 后序遍历: ";
    tree->postOrder(resList);
    cout << endl;

    cout << "== RBTree ok:  " << tree->isRBTree() << endl;
    cout << "== 最小值: " << tree->minimum() << endl;
    cout << "== 最大值: " << tree->maximum() << endl;
    cout << "== 树的详细信息: " << endl;
    tree->print();

    // 设置check_remove=1,测试"删除函数"
    if(check_remove)
    {
        for(i=0; i<ilen; i++)
        {
            tree->remove(a[i]);

            cout << "== 删除节点: " << a[i] << endl;
            cout << "== 树的详细信息: " << endl;
            tree->print();
            if(!tree->isRBTree())
            {
                cout << "!!!!!!!!!!RBTree remove failed!!!!!!!!!!!!!!!!!!" << endl;
            }
            cout << endl;
        }
    }
}

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
    RbtHashMap<int,ValueType,TEST_COUNT>::iterator it = testMap->begin();
    for(;it != testMap->end();it++)
    {
        int a = it->second.a;
        std::cout << "it->first = " << it->first << " it->second = " << a << std::endl;
    }
    testMap->erase(HASH_CONFLICT_RATE * 1);
    std::cout << "Hello, World!" << std::endl;
    delete testMap;
    testMap = NULL;
    return 0;
}
