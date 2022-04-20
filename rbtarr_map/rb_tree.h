//
// * C++ 语言: 红黑树
// Created by dguco on 2022/4/13.
// https://www.cnblogs.com/skywang12345/p/3245399.html
// https://www.cnblogs.com/skywang12345/p/3603935.html
//

#ifndef RBTARR_MAP_RB_TREE_H
#define RBTARR_MAP_RB_TREE_H

#include <iomanip>
#include <iostream>
#include <list>
using namespace std;

#define RB_TREE_JAVA 1
#define DEBUG_RB_TREE 1

enum RBTColor
{
    RB_NONE = 0,
    RB_RED = 1,
    RB_BLACK = 2,
};

/**
 *单个节点类
* */
#pragma pack(1)
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE = unsigned int>
struct RBTNode
{
public:
    typedef std::pair<KeyType_,ValueType_> NodeDataType_;
public:
    RBTNode()
    {
        clear();
    }

    ~RBTNode()
    {
       clear();
    }

    void clear_rb()
    {
        parent_ = 0;
        left_ = 0;
        right_ = 0;
        color_ = RB_BLACK;
    }

    void clear()
    {
        parent_ = 0;
        left_ = 0;
        right_ = 0;
        color_ = RB_NONE;
        memset(data_,0,sizeof(NodeDataType_));
    }

    inline void set_cur(INDEX_TYPE value)    { cur_ = value;}
    inline INDEX_TYPE  get_cur()             { return cur_;}

    inline void set_prev(INDEX_TYPE value)   { left_ = value;}
    inline INDEX_TYPE  get_prev()            { return left_;}

    inline void set_next(INDEX_TYPE value)   { right_ = value;}
    inline INDEX_TYPE  get_next()            { return right_;}

    inline void set_parent(INDEX_TYPE value) { parent_ = value;}
    inline INDEX_TYPE  get_parent()          { return parent_;}

    inline void set_left(INDEX_TYPE value)   { left_ = value;}
    inline INDEX_TYPE  get_left()            { return left_;}

    inline void set_right(INDEX_TYPE value)  { right_ = value;}
    inline INDEX_TYPE  get_right()           { return right_;}

    inline void set_color(RBTColor value)    { color_ = value;}
    inline RBTColor  get_color()             { return (RBTColor)color_;}

    inline void set_key(KeyType_ &key_)      { value().first = key_;}
    inline KeyType_&     get_key()           { return  value().first;}

    inline void set_value(ValueType_& value_){value().second = value_;}
    inline ValueType_& getValue()            {return  value().second;}

    void dis_from_list()
    {
        set_next(0);
        set_prev(0);
    }

    std::pair<KeyType_,ValueType_>& value()
    {
        return *((NodeDataType_*)data_);
    }

    char* data()
    {
        return data_;
    }


public:
    RBTNode(KeyType_ value, RBTColor c, INDEX_TYPE p, INDEX_TYPE l, INDEX_TYPE r)
        : color_(c), parent_(), left_(l), right_(r)
    {}
private:
    char                                data_[sizeof(NodeDataType_)];  //节点对象信息
    INDEX_TYPE                          cur_;                          //当前节点在数组中的索引位置+1
    INDEX_TYPE                          parent_;                       //父亲节点在数组中的索引位置+1
    INDEX_TYPE                          left_;                         //左子节点在数组中的索引位置+1
    INDEX_TYPE                          right_;                        //右子节点在数组中的索引位置+1
    unsigned  char                      color_;                        //节点color
};

#define NodeType_ RBTNode<KeyType_,ValueType_,INDEX_TYPE>
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE = unsigned int,std::size_t Cap_ = 0>
class RBTree
{
public:
    RBTree(NodeType_ *pool,INDEX_TYPE root = 0);
    ~RBTree();
    bool isRBTree();
    // 前序遍历"红黑树"
    void preOrder(std::list<NodeType_*>& resList);
    // 中序遍历"红黑树"
    void inOrder(std::list<NodeType_*>& resList);
    // 后序遍历"红黑树"
    void postOrder(std::list<NodeType_*>& resList);
    // (递归实现)查找"红黑树"中键值为key的节点
    NodeType_ *search(KeyType_ key);
    // (非递归实现)查找"红黑树"中键值为key的节点
    NodeType_ *iterativeSearch(KeyType_ key);
    // 查找最小结点：返回最小结点的键值。
    KeyType_ minimum();
    // 查找最大结点：返回最大结点的键值。
    KeyType_ maximum();
    // 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
    NodeType_ *successor(NodeType_ *x);
    // 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
    NodeType_ *predecessor(NodeType_ *x);
    // 插入函数
    void insert(NodeType_ *node);
    // 删除结点(key为节点键值)
    void remove(KeyType_ key);
    // 打印红黑树
    void print();
    bool isEmpty();
    INDEX_TYPE root();
private:
    bool isRBTree(NodeType_* root, int blacknum, int count);
    // 前序遍历"红黑树"
    void preOrder(NodeType_ *tree, std::list<NodeType_*>& resList) ;
    // 中序遍历"红黑树"
    void inOrder(NodeType_ *tree, std::list<NodeType_*>& resList) ;
    // 后序遍历"红黑树"
    void postOrder(NodeType_ *tree, std::list<NodeType_*>& resList) ;
    // (递归实现)查找"红黑树x"中键值为key的节点
    NodeType_ *search(NodeType_ *x, KeyType_ key);
    // (非递归实现)查找"红黑树x"中键值为key的节点
    NodeType_ *iterativeSearch(NodeType_ *x, KeyType_ key);
    // 查找最小结点：返回tree为根结点的红黑树的最小结点。
    NodeType_ *minimum(NodeType_ *tree);
    // 查找最大结点：返回tree为根结点的红黑树的最大结点。
    NodeType_ *maximum(NodeType_ *tree);
    // 左旋
    void leftRotate(NodeType_ *x);
    // 右旋
    void rightRotate(NodeType_ *y);
    // 插入修正函数
    void insertFixUp(NodeType_ *node);
    // 删除函数
    void remove(NodeType_ *node);
    // 删除修正函数
    void removeFixUp(NodeType_ *node);
    // 删除修正函数
    void removeFixUp(NodeType_ *node, NodeType_ *parent);
    // 打印红黑树
    void print(NodeType_ *tree, KeyType_ key, int direction);
    inline NodeType_* getNode(INDEX_TYPE index);
    inline NodeType_ *parentOf(NodeType_ *node);
    inline void *setParent(NodeType_ *node, NodeType_ *parent);
    inline RBTColor colorOf(NodeType_ *node);
    inline bool isRed(NodeType_ *node);
    inline bool isBlack(NodeType_ *node);
    inline void setColor(NodeType_ *node, RBTColor color);
    inline void setRed(NodeType_ *node);
    inline void setBlack(NodeType_ *node);
    inline NodeType_ *leftOf(NodeType_ *node);
    inline void *setLeft(NodeType_ *node, NodeType_ *left);
    inline NodeType_ *rightOf(NodeType_ *node);
    inline void setRight(NodeType_ *node, NodeType_ *right);
    inline INDEX_TYPE curOf(NodeType_ *node);
private:
    NodeType_  *m_Pool;    //
    INDEX_TYPE   m_Root;
};

/*
 * 构造函数
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::RBTree(NodeType_  *pool,INDEX_TYPE root)
    :m_Pool(pool),m_Root(root)
{
}

/*
 * 析构函数
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::~RBTree()
{
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
bool RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::isRBTree()
{
    //空树
    if (getNode(m_Root) == nullptr)
    {
        return true;
    }
    //根节点为黑色
    if (colorOf(getNode(m_Root))  == RB_RED)
    {
        cout << "根节点为红色" << endl;
        return false;
    }
    //黑色结点数量各路径上相同
    //先走一条得到基准值
    int Blacknum = 0;
    NodeType_* cur = getNode(m_Root);
    while (cur)
    {
        if (colorOf(cur) == RB_BLACK)
            Blacknum++;
        cur = leftOf(cur);
    }
    //检查子树
    int i = 0;
    return isRBTree(getNode(m_Root), Blacknum, i);
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
bool RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::isRBTree(NodeType_* root, int blacknum, int count)
{
    //递归到空节点
    if (root == nullptr)
    {
        if (blacknum == count)
            return true;
        cout << "各路径上黑色节点个数不同" << endl;
        return false;
    }
    //子节点为红则检查父节点是否为红（通过父节点检查子节点会遇到空节点）
    if (colorOf(root) == RB_RED && colorOf(parentOf(root)) == RB_RED)
    {
        cout << "存在连续红色节点" << endl;
        return false;
    }
    //计数黑结点
    if (colorOf(root) == RB_BLACK)
        count++;
    //递归左右子树
    return isRBTree(leftOf(root), blacknum, count) && isRBTree(rightOf(root), blacknum, count);
}

/*
 * 前序遍历"红黑树"
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::preOrder(NodeType_ *tree, std::list<NodeType_*>& resList)
{
    if (tree != NULL) {
        if(DEBUG_RB_TREE)
        {
            cout << tree->get_key() << " ";
        }
        resList.push_back(tree);
        preOrder(leftOf(tree),resList);
        preOrder(rightOf(tree),resList);
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::preOrder(std::list<NodeType_*>& resList)
{
    preOrder(getNode(m_Root),resList);
}

/*
 * 中序遍历"红黑树"
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::inOrder(NodeType_ *tree, std::list<NodeType_*>& resList)
{
    if (tree != NULL) {
        inOrder(leftOf(tree),resList);
        if(DEBUG_RB_TREE)
        {
            cout << tree->get_key() << " ";
        }
        resList.push_back(tree);
        inOrder(rightOf(tree),resList);
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::inOrder(std::list<NodeType_*>& resList)
{
    inOrder(getNode(m_Root),resList);
}

/*
 * 后序遍历"红黑树"
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::postOrder(NodeType_ *tree, std::list<NodeType_*>& resList)
{
    if (tree != NULL)
    {
        postOrder(leftOf(tree),resList);
        postOrder(rightOf(tree),resList);
        if(DEBUG_RB_TREE)
        {
            cout << tree->get_key() << " ";
        }
        resList.push_back(tree);
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::postOrder(std::list<NodeType_*>& resList)
{
    postOrder(getNode(m_Root),resList);
}

/*
 * (递归实现)查找"红黑树x"中键值为key的节点
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::search(NodeType_ *x, KeyType_ key)
{
    if (x == NULL || x->get_key() == key)
        return x;

    if (key < x->get_key())
        return search(leftOf(x), key);
    else
        return search(rightOf(x), key);
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::search(KeyType_ key)
{
    search(getNode(m_Root), key);
}

/*
 * (非递归实现)查找"红黑树x"中键值为key的节点
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::iterativeSearch(NodeType_ *x, KeyType_ key)
{
    while ((x != NULL) && (x->key != key)) {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::iterativeSearch(KeyType_ key)
{
    iterativeSearch(getNode(m_Root), key);
}

/*
 * 查找最小结点：返回tree为根结点的红黑树的最小结点。
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::minimum(NodeType_ *tree)
{
    if (tree == NULL)
        return NULL;

    while (leftOf(tree) != NULL)
        tree = leftOf(tree);
    return tree;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
KeyType_ RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::minimum()
{
    NodeType_ *p = minimum(getNode(m_Root));
    if (p != NULL)
        return p->get_key();

    return (KeyType_) NULL;
}

/*
 * 查找最大结点：返回tree为根结点的红黑树的最大结点。
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::maximum(NodeType_ *tree)
{
    if (tree == NULL)
        return NULL;

    while (rightOf(tree) != NULL)
        tree = rightOf(tree);
    return tree;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
KeyType_ RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::maximum()
{
    NodeType_ *p = maximum(getNode(m_Root));
    if (p != NULL)
        return p->get_key();

    return (KeyType_) NULL;
}

/*
 * 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::successor(NodeType_ *x)
{
    if(x == NULL)
    {
        return NULL;
    }

    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (rightOf(x) != NULL)
        return minimum(rightOf(x));

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    NodeType_ *y = parentOf(x);
    while ((y != NULL) && (x == rightOf(y)))
    {
        x = y;
        y = parentOf(y);
    }
    return y;
}

/*
 * 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_ *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::predecessor(NodeType_ *x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return maximum(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    NodeType_ *y = x->parent;
    while ((y != NULL) && (x == y->left)) {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 对红黑树的节点(x)进行左旋转
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::leftRotate(NodeType_ *x)
{
    // 设置x的右孩子为y
    NodeType_ *y = rightOf(x);

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    setRight(x, leftOf(y));
    if (leftOf(y) != NULL)
        setParent(leftOf(y),x);

    // 将 “x的父亲” 设为 “y的父亲”
    setParent(y, parentOf(x));

    if (parentOf(x) == NULL) {
        m_Root = curOf(y);            // 如果 “x的父亲” 是空节点，则将y设为根节点
    }
    else {
        if (leftOf(parentOf(x)) == x)
            setLeft(parentOf(x),y); //如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            setRight(parentOf(x),y); // 如果 x是它父节点的右孩子，则将y设为“x的父节点的右孩子”
    }

    // 将 “x” 设为 “y的左孩子”
    setLeft(y,x);
    // 将 “x的父节点” 设为 “y”
    setParent(x,y);
}

/*
 * 对红黑树的节点(y)进行右旋转
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::rightRotate(NodeType_ *y)
{
    // 设置x是当前节点的左孩子。
    NodeType_ *x = leftOf(y);

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    setLeft(y, rightOf(x));
    if (rightOf(x) != NULL)
        setParent(rightOf(x),y);

    // 将 “y的父亲” 设为 “x的父亲”
    setParent(x, parentOf(y));

    if (parentOf(y) == NULL) {
        m_Root = curOf(x);            // 如果 “y的父亲” 是空节点，则将x设为根节点
    }
    else {
        if (y == rightOf(parentOf(y)))
            setRight(parentOf(y),x);// 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            setLeft(parentOf(y),x); // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    setRight(x,y);
    // 将 “y的父节点” 设为 “x”
    setParent(y,x);
}

/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点    对应《算法导论》中的z
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::insertFixUp(NodeType_ *node)
{
    NodeType_ *parent, *gparent;

    // 若父节点存在，并且父节点的颜色是红色,根节点是黑色(被插入节点是一定存在非空祖父节点)
    while ((parent = parentOf(node)) && isRed(parent))
    {
        gparent = parentOf(parent);
        //若父节点是祖父节点的左孩子
        if (parent == leftOf(gparent))
        {
            // Case 1：叔叔节点是红色
            NodeType_ *uncle = rightOf(gparent);
            if (isRed(uncle))
            {
                setBlack(parent);//  (01) 将“父节点”设为黑色。
                setBlack(uncle); //  (02) 将“叔叔节点”设为黑色。
                setRed(gparent); //  (03) 将“祖父节点”设为“红色”。
                node = gparent;      //  (04) 将“祖父节点”设为“当前节点”(红色节点)
            }else  //红黑树特性,空节点或者黑色的非空节点都为黑色节点(叔叔是黑色)
            {
                if(!RB_TREE_JAVA) //实现方法1  算法导论 https://www.cnblogs.com/skywang12345/p/3624291.html
                {
                    // Case 2：叔叔是黑色，且当前节点是右孩子
                    if (rightOf(parent) == node)
                    {
                        NodeType_ *tmp;
                        leftRotate(parent);  // (01) 将“父节点”作为“新的当前节点”。 (02) 以“新的当前节点”为支点进行左旋。
                        tmp = parent;
                        parent = node;
                        node = tmp;
                    }
                    // Case 3条件：叔叔是黑色，且当前节点是左孩子。
                    setBlack(parent);                        //(01) 将“父节点”设为“黑色”。
                    setRed(gparent);                         //(02) 将“祖父节点”设为“红色”。
                    rightRotate(gparent);              //(03) 以“祖父节点”为支点进行右旋。
                }else //实现方法2 java jdk TreeMap(fixAfterInsertion)
                {
                    if (rightOf(parent) == node)
                    {
                        node = parent;
                        leftRotate(node);
                    }
                    //如意如果parent->right == node，这里的rb_parent(node)是对左旋之后的树进行操作，最终的结果和方法1是一样的
                    setBlack(parentOf(node));
                    setRed(parentOf(parentOf(node)));
                    rightRotate(parentOf(parentOf(node)));
                }

            }
        }
        else//若父节点是祖父节点的右孩子,将上面的操作中“rightRotate”和“leftRotate”交换位置，然后依次执行。
        {
            // Case 1条件：叔叔节点是红色
            NodeType_ *uncle = leftOf(gparent);
            if (isRed(uncle))
            {
                setBlack(parent); //  (01) 将“父节点”设为黑色。
                setBlack(uncle);  //  (02) 将“叔叔节点”设为黑色。
                setRed(gparent);  //  (03) 将“祖父节点”设为“红色”。
                node = gparent;       //  (04) 将“祖父节点”设为“当前节点”(红色节点)
            }else //红黑树特性,空节点或者黑色的非空节点都为黑色节点(叔叔是黑色)
            {
                if(!RB_TREE_JAVA)   //实现方法1  算法导论 https://www.cnblogs.com/skywang12345/p/3624291.html
                {
                    // Case 2条件：叔叔是黑色，且当前节点是左孩子
                    if (leftOf(parent) == node)
                    {
                        NodeType_ *tmp;
                        rightRotate(parent);
                        tmp = parent;
                        parent = node;
                        node = tmp;
                    }

                    // Case 3条件：叔叔是黑色，且当前节点是右孩子。
                    setBlack(parent);
                    setRed(gparent);
                    leftRotate(gparent);
                }else //实现方法2 java jdk TreeMap(fixAfterInsertion)
                {
                    if (leftOf(parent) == node)
                    {
                        node = parent;
                        rightRotate(node);
                    }
                    setBlack(parentOf(node));
                    setRed(parentOf(parentOf(node)));
                    leftRotate(parentOf(parentOf(node)));
                }
            }
        }
    }

    // 将根节点设为黑色
    setBlack(getNode(m_Root));
}

/*
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 插入的结点        // 对应《算法导论》中的node
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::insert(NodeType_ *node)
{
    NodeType_ *parent_node = NULL;
    NodeType_ *tmp_node = getNode(m_Root);

    //将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    {
        //存在root节点,先找新节点的父亲节点在哪
        while (tmp_node != NULL)
        {
            parent_node = tmp_node;
            //新节点比当前节点key小，往左子树开始找
            if (node->get_key() < tmp_node->get_key())
                tmp_node = leftOf(tmp_node);
            else //新节点比当前节点key大，往右子树开始找
                tmp_node = rightOf(tmp_node);
        }

        //把新节点父亲节点指向找到的节点
        setParent(node,parent_node);
        //如果有根节点
        if (parent_node != NULL)
        {
            //新节点比当前节点key小，插入父亲节点的左边
            if (node->get_key() < parent_node->get_key())
                setLeft(parent_node,node);
            else //新节点比当前节点key大，插入父亲节点的右边
                setRight(parent_node,node);
        }
        else
            m_Root = curOf(node);
    }

    //设置节点的颜色为红色
    setRed(node);
    //将它重新修正为一颗二叉查找树
    insertFixUp(node);
}

/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::removeFixUp(NodeType_ *node)
{
    NodeType_ *other;

    while (node != getNode(m_Root) && isBlack(node))
    {
        if (leftOf(parentOf(node)) == node)
        {
            other = rightOf(parentOf(node));
            if (isRed(other))
            {
                // Case 1: x的兄弟w是红色的
                setBlack(other);
                setRed(parentOf(node));
                leftRotate(parentOf(node));
                other = rightOf(parentOf(node));
            }
            if (isBlack(leftOf(other)) && isBlack(rightOf(other)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                setRed(other);
                node = parentOf(node);
            }
            else {
                if (isBlack(rightOf(other)))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    setBlack(leftOf(other));
                    setRed(other);
                    rightRotate(other);
                    other = rightOf(parentOf(node));
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                setColor(other, colorOf(parentOf(node)));
                setBlack(parentOf(node));
                setBlack(rightOf(other));
                leftRotate(parentOf(node));
                node = getNode(m_Root);
            }
        }
        else {
            other = leftOf(parentOf(node));
            if (isRed(other))
            {
                // Case 1: x的兄弟w是红色的
                setBlack(other);
                setRed(parentOf(node));
                rightRotate(parentOf(node));
                other = leftOf(parentOf(node));
            }
            if (isBlack(leftOf(other)) && isBlack(rightOf(other)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                setRed(other);
                node = parentOf(node);
            }
            else {
                if (isBlack(leftOf(other)))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    setBlack(rightOf(other));
                    setRed(other);
                    leftRotate(other);
                    other = leftOf(parentOf(other));
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                setColor(other, colorOf(parentOf(node)));
                setBlack(parentOf(node));
                setBlack(leftOf(other));
                rightRotate(parentOf(node));
                node = getNode(m_Root);
            }
        }
    }
    if (node)
        setBlack(node);
}

// 删除修正函数
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::removeFixUp(NodeType_ *node, NodeType_ *parent)
{
    NodeType_ *other;

    while ((!node || isBlack(node)) && node != getNode(m_Root))
    {
        if (leftOf(parent) == node)
        {
            other = rightOf(parent);
            if (isRed(other))
            {
                // Case 1: x的兄弟w是红色的
                setBlack(other);
                setRed(parent);
                leftRotate(parent);
                other = rightOf(parent);
            }
            if (isBlack(leftOf(other)) && isBlack(rightOf(other)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                setRed(other);
                node = parent;
                parent = parentOf(node);
            }
            else
            {
                if (isBlack(rightOf(other)))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    setBlack(leftOf(other));
                    setRed(other);
                    rightRotate(other);
                    other = rightOf(parent);
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                setColor(other, colorOf(parent));
                setBlack(parent);
                setBlack(rightOf(other));
                leftRotate(parent);
                node = getNode(m_Root);
                break;
            }
        }
        else
        {
            other = leftOf(parent);
            if (isRed(other))
            {
                // Case 1: x的兄弟w是红色的
                setBlack(other);
                setRed(parent);
                rightRotate(parent);
                other = leftOf(parent);
            }
            if (isBlack(leftOf(other)) && isBlack(rightOf(other)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                setRed(other);
                node = parent;
                parent = parentOf(node);
            }
            else
            {
                if (isBlack(leftOf(other)))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    setBlack(rightOf(other));
                    setRed(other);
                    leftRotate(other);
                    other = leftOf(parent);
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                setColor(other, colorOf(parent));
                setBlack(parent);
                setBlack(leftOf(other));
                rightRotate(parent);
                node = getNode(m_Root);
                break;
            }
        }
    }
    if (node)
        setBlack(node);
}

/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 删除的结点
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::remove(NodeType_ *node)
{
    if(!RB_TREE_JAVA)  //实现方法1  算法导论 https://www.cnblogs.com/skywang12345/p/3624291.html
    {
        NodeType_ *child, *parent;
        RBTColor color;

        // 被删除节点的"左右孩子都不为空"的情况。
        if ((leftOf(node) != NULL) && (rightOf(node) != NULL))
        {
            // 被删节点的后继节点。(称为"取代节点")
            // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
            NodeType_ *replace = node;

            // 获取后继节点
            replace = rightOf(replace);
            while (leftOf(replace) != NULL)
                replace = leftOf(replace);

            // "node节点"不是根节点d(只有根节点不存在父节点)
            if (parentOf(node))
            {
                if (leftOf(parentOf(node)) == node)
                    setLeft(parentOf(node),replace);
                else
                    setRight(parentOf(node),replace);
            }
            else
                // "node节点"是根节点，更新根节点。
                m_Root = curOf(replace);

            // child是"取代节点"的右孩子，也是需要"调整的节点"。
            // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
            child = rightOf(replace);
            parent = parentOf(replace);
            // 保存"取代节点"的颜色
            color = colorOf(replace);

            // "被删除节点"是"它的后继节点的父节点"
            if (parent == node)
            {
                parent = replace;
            }
            else
            {
                // child不为空
                if (child)
                    setParent(child,parent);
                setLeft(parent,child);
                setRight(replace, rightOf(node));
                setParent(rightOf(node),replace);
            }

            setParent(replace, parentOf(node));
            setColor(replace, colorOf(node));
            setLeft(replace, leftOf(node));
            setParent(leftOf(node),replace);
            if (color == RB_BLACK)
                removeFixUp(child,parent);

            node->clear_rb();
            return;
        }

        if (leftOf(node) != NULL)
            child = leftOf(node);
        else
            child = rightOf(node);

        parent = parentOf(node);
        // 保存"取代节点"的颜色
        color = colorOf(node);

        if (child)
            setParent(child,parent);

        // "node节点"不是根节点
        if (parent) {
            if (leftOf(parent) == node)
                setLeft(parent,child);
            else
                setRight(parent,child);
        }
        else
            m_Root = curOf(child);

        if (color == RB_BLACK)
            removeFixUp(child,parent);
        node->clear_rb();
    }else //实现方法2 java jdk TreeMap(deleteEntry)
    {
        // 被删除节点的"左右孩子都不为空"的情况。
        if ((leftOf(node) != NULL) && (rightOf(node) != NULL))
        {
            NodeType_* succNode = successor(node);
            node->set_key(succNode->get_key());
            node = succNode;
        }

        NodeType_* replacement = (leftOf(node) != NULL) ? leftOf(node) : rightOf(node);
        if(replacement != NULL)
        {
            // Link replacement to parent
            setParent(replacement, parentOf(node));
            if (parentOf(node) == NULL)
                m_Root = curOf(replacement);
            else if (node == leftOf(parentOf(node)))
                setLeft(parentOf(node),replacement);
            else
                setRight(parentOf(node),replacement);

            // Null out links so they are OK to use by fixAfterDeletion.
            setLeft(node,NULL);
            setRight(node,NULL);
            setParent(node,NULL);
            // Fix replacement
            if (colorOf(node) == RB_BLACK)
                removeFixUp(replacement);
            node->clear_rb();
        }else if(parentOf(node) == NULL)
        {
            m_Root = 0;
        }else
        {
            if(colorOf(node) == RB_BLACK)
            {
                removeFixUp(node);
            }

            if(parentOf(node) != NULL)
            {
                if(node == leftOf(parentOf(node)))
                {
                    setLeft(parentOf(node),NULL);
                }else if(node == rightOf(parentOf(node)))
                {
                    setRight(parentOf(node),NULL);
                }
                setParent(node,NULL);
            }
        }
    }
}

/*
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::remove(KeyType_ key)
{
    NodeType_ *node;

    // 查找key对应的节点(node)，找到的话就删除该节点
    if ((node = search(getNode(m_Root), key)) != NULL)
        remove(node);
}

/*
 * 销毁红黑树
 */
//template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
//void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::destroy(NODE_TYPE *&tree)
//{
//    if (tree == NULL)
//        return;
//
//    if (tree->left != NULL)
//        return destroy(tree->left);
//    if (tree->right != NULL)
//        return destroy(tree->right);
//
//    delete tree;
//    tree = NULL;
//}

//template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
//void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::destroy()
//{
//    destroy(mRoot);
//}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::print(NodeType_ *tree, KeyType_ key, int direction)
{
    if (tree != NULL) {
        if (direction == 0)    // tree是根节点
            cout << setw(2) << tree->get_key() << "(B) is root" << endl;
        else                // tree是分支节点
            cout << setw(2) << tree->get_key() << ( isRed(tree) ? "(R)" : "(B)") << " is " << setw(2) << key << "'s "
                 << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(leftOf(tree), tree->get_key(), -1);
        print(rightOf(tree), tree->get_key(), 1);
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
NodeType_* RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::getNode(INDEX_TYPE index)
{
    if(index > 0 && index <= Cap_)
    {
        return &m_Pool[index -1];
    }
    return NULL;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::print()
{
    if (getNode(m_Root) != NULL)
        print(getNode(m_Root), getNode(m_Root)->get_key(), 0);
}
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
bool RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::isEmpty()
{
    return m_Root == 0;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
INDEX_TYPE RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::root()
{
    return m_Root;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline NodeType_* RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::parentOf(NodeType_* node)
{
    return  node == NULL ? NULL : getNode(node->get_parent());
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setParent(NodeType_ *node, NodeType_ *parent)
{
    if(node)
        node->set_parent(parent == NULL ? 0 : parent->get_cur());
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline RBTColor RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::colorOf(NodeType_* node)
{
    return RBTColor(node == NULL ? RB_BLACK : node->get_color());
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline bool RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::isRed(NodeType_* node)
{
    return colorOf(node) == RB_RED;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline bool RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::isBlack(NodeType_* node)
{
    return colorOf(node) == RB_BLACK;
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setColor(NodeType_* node, RBTColor color)
{
    if(node != NULL) node->set_color(color);
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setRed(NodeType_* node)
{
    setColor(node,RB_RED);
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setBlack(NodeType_* node)
{
    setColor(node,RB_BLACK);
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline NodeType_* RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::leftOf(NodeType_* node)
{
    return node == NULL ? NULL : getNode(node->get_left());
}
template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void *RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setLeft(NodeType_ *node, NodeType_ *left)
{
    if(node)
    {
        node->set_left(left == NULL ? 0 : left->get_cur());
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline NodeType_* RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::rightOf(NodeType_* node)
{
    return node == NULL ? NULL : getNode(node->get_right());
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline void RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::setRight(NodeType_ *node, NodeType_ *right)
{
    if(node)
    {
        node->set_right(right == NULL ? 0 : right->get_cur());
    }
}

template<typename KeyType_,typename ValueType_,typename INDEX_TYPE,std::size_t Cap_>
inline INDEX_TYPE RBTree<KeyType_,ValueType_,INDEX_TYPE,Cap_>::curOf(NodeType_ *node)
{
    return node == NULL ? 0 : node->get_cur();
}


#endif //RBTARR_MAP_RB_TREE_H
