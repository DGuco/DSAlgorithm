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
using namespace std;

enum RBTColor
{
    RED, BLACK
};

template<class T>
class RBTNode
{
public:
    RBTColor color;    // 颜色
    T key;             // 关键字(键值)
    RBTNode *left;     // 左孩子
    RBTNode *right;    // 右孩子
    RBTNode *parent;   // 父结点

    RBTNode(T value, RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r)
        :
        key(value), color(c), parent(), left(l), right(r)
    {}
};

#define rb_parent(r)   ((r)->parent)
#define rb_color(r) ((r)->color)
#define rb_is_red(r)   ((r)->color==RED)
#define rb_is_black(r)  ((r)->color==BLACK)
#define rb_set_black(r)  do { (r)->color = BLACK; } while (0)
#define rb_set_red(r)  do { (r)->color = RED; } while (0)
#define rb_set_parent(r, p)  do { (r)->parent = (p); } while (0)
#define rb_set_color(r, c)  do { (r)->color = (c); } while (0)

template<class T>
class RBTree
{
public:
    RBTree();
    ~RBTree();

    bool isRBTree();
    // 前序遍历"红黑树"
    void preOrder();
    // 中序遍历"红黑树"
    void inOrder();
    // 后序遍历"红黑树"
    void postOrder();

    // (递归实现)查找"红黑树"中键值为key的节点
    RBTNode<T> *search(T key);
    // (非递归实现)查找"红黑树"中键值为key的节点
    RBTNode<T> *iterativeSearch(T key);

    // 查找最小结点：返回最小结点的键值。
    T minimum();
    // 查找最大结点：返回最大结点的键值。
    T maximum();

    // 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
    RBTNode<T> *successor(RBTNode<T> *x);
    // 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
    RBTNode<T> *predecessor(RBTNode<T> *x);

    // 将结点(key为节点键值)插入到红黑树中
    void insert(T key);

    // 删除结点(key为节点键值)
    void remove(T key);

    // 销毁红黑树
    void destroy();

    // 打印红黑树
    void print();
private:
    bool isRBTree(RBTNode<T>* root, int blacknum, int count);
    // 前序遍历"红黑树"
    void preOrder(RBTNode<T> *tree) const;
    // 中序遍历"红黑树"
    void inOrder(RBTNode<T> *tree) const;
    // 后序遍历"红黑树"
    void postOrder(RBTNode<T> *tree) const;

    // (递归实现)查找"红黑树x"中键值为key的节点
    RBTNode<T> *search(RBTNode<T> *x, T key) const;
    // (非递归实现)查找"红黑树x"中键值为key的节点
    RBTNode<T> *iterativeSearch(RBTNode<T> *x, T key) const;

    // 查找最小结点：返回tree为根结点的红黑树的最小结点。
    RBTNode<T> *minimum(RBTNode<T> *tree);
    // 查找最大结点：返回tree为根结点的红黑树的最大结点。
    RBTNode<T> *maximum(RBTNode<T> *tree);

    // 左旋
    void leftRotate(RBTNode<T> *&root, RBTNode<T> *x);
    // 右旋
    void rightRotate(RBTNode<T> *&root, RBTNode<T> *y);
    // 插入函数
    void insert(RBTNode<T> *&root, RBTNode<T> *node);
    // 插入修正函数
    void insertFixUp(RBTNode<T> *&root, RBTNode<T> *node);
    // 删除函数
    void remove(RBTNode<T> *&root, RBTNode<T> *node);
    // 删除修正函数
    void removeFixUp(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent);

    // 销毁红黑树
    void destroy(RBTNode<T> *&tree);

    // 打印红黑树
    void print(RBTNode<T> *tree, T key, int direction);
private:
    RBTNode<T> *mRoot;    // 根结点
};

/*
 * 构造函数
 */
template<class T>
RBTree<T>::RBTree()
    :mRoot(NULL)
{
    mRoot = NULL;
}

/*
 * 析构函数
 */
template<class T>
RBTree<T>::~RBTree()
{
    destroy();
}

template<class T>
bool RBTree<T>::isRBTree()
{
    //空树
    if (mRoot == nullptr)
    {
        return true;
    }
    //根节点为黑色
    if (rb_color(mRoot)  == RED)
    {
        cout << "根节点为红色" << endl;
        return false;
    }
    //黑色结点数量各路径上相同
    //先走一条得到基准值
    int Blacknum = 0;
    RBTNode<T>* cur = mRoot;
    while (cur)
    {
        if (rb_color(cur) == BLACK)
            Blacknum++;
        cur = cur->left;
    }
    //检查子树
    int i = 0;
    return isRBTree(mRoot, Blacknum, i);
}

template<class T>
bool RBTree<T>::isRBTree(RBTNode<T>* root, int blacknum, int count)
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
    if (rb_color(root) == RED && rb_color(rb_parent(root)) == RED)
    {
        cout << "存在连续红色节点" << endl;
        return false;
    }
    //计数黑结点
    if (rb_color(root) == BLACK)
        count++;
    //递归左右子树
    return isRBTree(root->left, blacknum, count) && isRBTree(root->right, blacknum, count);
}

/*
 * 前序遍历"红黑树"
 */
template<class T>
void RBTree<T>::preOrder(RBTNode<T> *tree) const
{
    if (tree != NULL) {
        cout << tree->key << " ";
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

template<class T>
void RBTree<T>::preOrder()
{
    preOrder(mRoot);
}

/*
 * 中序遍历"红黑树"
 */
template<class T>
void RBTree<T>::inOrder(RBTNode<T> *tree) const
{
    if (tree != NULL) {
        inOrder(tree->left);
        cout << tree->key << " ";
        inOrder(tree->right);
    }
}

template<class T>
void RBTree<T>::inOrder()
{
    inOrder(mRoot);
}

/*
 * 后序遍历"红黑树"
 */
template<class T>
void RBTree<T>::postOrder(RBTNode<T> *tree) const
{
    if (tree != NULL) {
        postOrder(tree->left);
        postOrder(tree->right);
        cout << tree->key << " ";
    }
}

template<class T>
void RBTree<T>::postOrder()
{
    postOrder(mRoot);
}

/*
 * (递归实现)查找"红黑树x"中键值为key的节点
 */
template<class T>
RBTNode<T> *RBTree<T>::search(RBTNode<T> *x, T key) const
{
    if (x == NULL || x->key == key)
        return x;

    if (key < x->key)
        return search(x->left, key);
    else
        return search(x->right, key);
}

template<class T>
RBTNode<T> *RBTree<T>::search(T key)
{
    search(mRoot, key);
}

/*
 * (非递归实现)查找"红黑树x"中键值为key的节点
 */
template<class T>
RBTNode<T> *RBTree<T>::iterativeSearch(RBTNode<T> *x, T key) const
{
    while ((x != NULL) && (x->key != key)) {
        if (key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    return x;
}

template<class T>
RBTNode<T> *RBTree<T>::iterativeSearch(T key)
{
    iterativeSearch(mRoot, key);
}

/*
 * 查找最小结点：返回tree为根结点的红黑树的最小结点。
 */
template<class T>
RBTNode<T> *RBTree<T>::minimum(RBTNode<T> *tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->left != NULL)
        tree = tree->left;
    return tree;
}

template<class T>
T RBTree<T>::minimum()
{
    RBTNode<T> *p = minimum(mRoot);
    if (p != NULL)
        return p->key;

    return (T) NULL;
}

/*
 * 查找最大结点：返回tree为根结点的红黑树的最大结点。
 */
template<class T>
RBTNode<T> *RBTree<T>::maximum(RBTNode<T> *tree)
{
    if (tree == NULL)
        return NULL;

    while (tree->right != NULL)
        tree = tree->right;
    return tree;
}

template<class T>
T RBTree<T>::maximum()
{
    RBTNode<T> *p = maximum(mRoot);
    if (p != NULL)
        return p->key;

    return (T) NULL;
}

/*
 * 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
 */
template<class T>
RBTNode<T> *RBTree<T>::successor(RBTNode<T> *x)
{
    if(x == NULL)
    {
        return NULL;
    }

    // 如果x存在右孩子，则"x的后继结点"为 "以其右孩子为根的子树的最小结点"。
    if (x->right != NULL)
        return minimum(x->right);

    // 如果x没有右孩子。则x有以下两种可能：
    // (01) x是"一个左孩子"，则"x的后继结点"为 "它的父结点"。
    // (02) x是"一个右孩子"，则查找"x的最低的父结点，并且该父结点要具有左孩子"，找到的这个"最低的父结点"就是"x的后继结点"。
    RBTNode<T> *y = x->parent;
    while ((y != NULL) && (x == y->right)) {
        x = y;
        y = y->parent;
    }

    return y;
}

/*
 * 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
 */
template<class T>
RBTNode<T> *RBTree<T>::predecessor(RBTNode<T> *x)
{
    // 如果x存在左孩子，则"x的前驱结点"为 "以其左孩子为根的子树的最大结点"。
    if (x->left != NULL)
        return maximum(x->left);

    // 如果x没有左孩子。则x有以下两种可能：
    // (01) x是"一个右孩子"，则"x的前驱结点"为 "它的父结点"。
    // (01) x是"一个左孩子"，则查找"x的最低的父结点，并且该父结点要具有右孩子"，找到的这个"最低的父结点"就是"x的前驱结点"。
    RBTNode<T> *y = x->parent;
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
template<class T>
void RBTree<T>::leftRotate(RBTNode<T> *&root, RBTNode<T> *x)
{
    // 设置x的右孩子为y
    RBTNode<T> *y = x->right;

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 将 “x的父亲” 设为 “y的父亲”
    y->parent = x->parent;

    if (x->parent == NULL) {
        root = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
    }
    else {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
    }

    // 将 “x” 设为 “y的左孩子”
    y->left = x;
    // 将 “x的父节点” 设为 “y”
    x->parent = y;
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
template<class T>
void RBTree<T>::rightRotate(RBTNode<T> *&root, RBTNode<T> *y)
{
    // 设置x是当前节点的左孩子。
    RBTNode<T> *x = y->left;

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;

    if (y->parent == NULL) {
        root = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
    }
    else {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    x->right = y;

    // 将 “y的父节点” 设为 “x”
    y->parent = x;
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
template<class T>
void RBTree<T>::insertFixUp(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *parent, *gparent;

    // 若父节点存在，并且父节点的颜色是红色,根节点是黑色(被插入节点是一定存在非空祖父节点)
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);
        //若父节点是祖父节点的左孩子
        if (parent == gparent->left)
        {
            // Case 1：叔叔节点是红色
            RBTNode<T> *uncle = gparent->right;
            if (uncle && rb_is_red(uncle))
            {
                rb_set_black(parent);//  (01) 将“父节点”设为黑色。
                rb_set_black(uncle); //  (02) 将“叔叔节点”设为黑色。
                rb_set_red(gparent); //  (03) 将“祖父节点”设为“红色”。
                node = gparent;      //  (04) 将“祖父节点”设为“当前节点”(红色节点)
            }else  //红黑树特性,空节点或者黑色的非空节点都为黑色节点(叔叔是黑色)
            {
                if(0) //实现方法1  算法导论
                {
                    // Case 2：叔叔是黑色，且当前节点是右孩子
                    if (parent->right == node)
                    {
                        RBTNode<T> *tmp;
                        leftRotate(root, parent);  // (01) 将“父节点”作为“新的当前节点”。 (02) 以“新的当前节点”为支点进行左旋。
                        tmp = parent;
                        parent = node;
                        node = tmp;
                    }
                    // Case 3条件：叔叔是黑色，且当前节点是左孩子。
                    rb_set_black(parent);               //(01) 将“父节点”设为“黑色”。
                    rb_set_red(gparent);                //(02) 将“祖父节点”设为“红色”。
                    rightRotate(root, gparent);      //(03) 以“祖父节点”为支点进行右旋。
                }

                if (1)  //实现方法2 java jdk TreeMap(fixAfterInsertion)
                {
                    if (parent->right == node)
                    {
                        node = parent;
                        leftRotate(root,node);
                    }
                    //如意如果parent->right == node，这里的rb_parent(node)是对左旋之后的树进行操作，最终的结果和方法1是一样的
                    rb_set_black(rb_parent(node));
                    rb_set_red(rb_parent(rb_parent(node)));
                    rightRotate(root,rb_parent(rb_parent(node)));
                }

            }
        }
        else//若父节点是祖父节点的右孩子,将上面的操作中“rightRotate”和“leftRotate”交换位置，然后依次执行。
        {
            // Case 1条件：叔叔节点是红色
            RBTNode<T> *uncle = gparent->left;
            if (uncle && rb_is_red(uncle))
            {
                rb_set_black(parent); //  (01) 将“父节点”设为黑色。
                rb_set_black(uncle);  //  (02) 将“叔叔节点”设为黑色。
                rb_set_red(gparent);  //  (03) 将“祖父节点”设为“红色”。
                node = gparent;       //  (04) 将“祖父节点”设为“当前节点”(红色节点)
            }else //红黑树特性,空节点或者黑色的非空节点都为黑色节点(叔叔是黑色)
            {
                if(0)   //实现方法1  算法导论
                {
                    // Case 2条件：叔叔是黑色，且当前节点是左孩子
                    if (parent->left == node)
                    {
                        RBTNode<T> *tmp;
                        rightRotate(root, parent);
                        tmp = parent;
                        parent = node;
                        node = tmp;
                    }

                    // Case 3条件：叔叔是黑色，且当前节点是右孩子。
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    leftRotate(root, gparent);
                }

                if(1) //实现方法2 java jdk TreeMap(fixAfterInsertion)
                {
                    if (parent->left == node)
                    {
                        node = parent;
                        rightRotate(root,node);
                    }
                    rb_set_black(rb_parent(node));
                    rb_set_red(rb_parent(rb_parent(node)));
                    leftRotate(root, rb_parent(rb_parent(node)));
                }
            }
        }
    }

    // 将根节点设为黑色
    rb_set_black(root);
}

/*
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 插入的结点        // 对应《算法导论》中的node
 */
template<class T>
void RBTree<T>::insert(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *parent_node = NULL;
    RBTNode<T> *tmp_node = root;

    //将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    {
        //存在root节点,先找新节点的父亲节点在哪
        while (tmp_node != NULL)
        {
            parent_node = tmp_node;
            //新节点比当前节点key小，往左子树开始找
            if (node->key < tmp_node->key)
                tmp_node = tmp_node->left;
            else //新节点比当前节点key大，往右子树开始找
                tmp_node = tmp_node->right;
        }

        //把新节点父亲节点指向找到的节点
        node->parent = parent_node;
        //如果有根节点
        if (parent_node != NULL)
        {
            //新节点比当前节点key小，插入父亲节点的左边
            if (node->key < parent_node->key)
                parent_node->left = node;
            else //新节点比当前节点key大，插入父亲节点的右边
                parent_node->right = node;
        }
        else
            root = node;
    }

    //设置节点的颜色为红色
    node->color = RED;
    //将它重新修正为一颗二叉查找树
    insertFixUp(root, node);
}

/*
 * 将结点(key为节点键值)插入到红黑树中
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     key 插入结点的键值
 */
template<class T>
void RBTree<T>::insert(T key)
{
    RBTNode<T> *z = NULL;

    // 如果新建结点失败，则返回。
    if ((z = new RBTNode<T>(key, BLACK, NULL, NULL, NULL)) == NULL)
        return;

    insert(mRoot, z);
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
template<class T>
void RBTree<T>::removeFixUp(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent)
{
    RBTNode<T> *other;

    while ((!node || rb_is_black(node)) && node != root) {
        if (parent->left == node) {
            other = parent->right;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                leftRotate(root, parent);
                other = parent->right;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->right || rb_is_black(other->right)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->left);
                    rb_set_red(other);
                    rightRotate(root, other);
                    other = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else {
            other = parent->left;
            if (rb_is_red(other)) {
                // Case 1: x的兄弟w是红色的
                rb_set_black(other);
                rb_set_red(parent);
                rightRotate(root, parent);
                other = parent->left;
            }
            if ((!other->left || rb_is_black(other->left)) &&
                (!other->right || rb_is_black(other->right))) {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的
                rb_set_red(other);
                node = parent;
                parent = rb_parent(node);
            }
            else {
                if (!other->left || rb_is_black(other->left)) {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                    rb_set_black(other->right);
                    rb_set_red(other);
                    leftRotate(root, other);
                    other = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(other, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(other->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 删除的结点
 */
template<class T>
void RBTree<T>::remove(RBTNode<T> *&root, RBTNode<T> *node)
{
    RBTNode<T> *child, *parent;
    RBTColor color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ((node->left != NULL) && (node->right != NULL))
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBTNode<T> *replace = node;

        // 获取后继节点
        replace = replace->right;
        while (replace->left != NULL)
            replace = replace->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node)) {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = replace;
            else
                rb_parent(node)->right = replace;
        }
        else
            // "node节点"是根节点，更新根节点。
            root = replace;

        // child是"取代节点"的右孩子，也是需要"调整的节点"。
        // "取代节点"肯定不存在左孩子！因为它是一个后继节点。
        child = replace->right;
        parent = rb_parent(replace);
        // 保存"取代节点"的颜色
        color = rb_color(replace);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node) {
            parent = replace;
        }
        else {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            replace->right = node->right;
            rb_set_parent(node->right, replace);
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return;
    }

    if (node->left != NULL)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    // 保存"取代节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点
    if (parent) {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/*
 * 删除红黑树中键值为key的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */
template<class T>
void RBTree<T>::remove(T key)
{
    RBTNode<T> *node;

    // 查找key对应的节点(node)，找到的话就删除该节点
    if ((node = search(mRoot, key)) != NULL)
        remove(mRoot, node);
}

/*
 * 销毁红黑树
 */
template<class T>
void RBTree<T>::destroy(RBTNode<T> *&tree)
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        return destroy(tree->left);
    if (tree->right != NULL)
        return destroy(tree->right);

    delete tree;
    tree = NULL;
}

template<class T>
void RBTree<T>::destroy()
{
    destroy(mRoot);
}

/*
 * 打印"二叉查找树"
 *
 * key        -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */
template<class T>
void RBTree<T>::print(RBTNode<T> *tree, T key, int direction)
{
    if (tree != NULL) {
        if (direction == 0)    // tree是根节点
            cout << setw(2) << tree->key << "(B) is root" << endl;
        else                // tree是分支节点
            cout << setw(2) << tree->key << (rb_is_red(tree) ? "(R)" : "(B)") << " is " << setw(2) << key << "'s "
                 << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(tree->left, tree->key, -1);
        print(tree->right, tree->key, 1);
    }
}

template<class T>
void RBTree<T>::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->key, 0);
}

#endif //RBTARR_MAP_RB_TREE_H
