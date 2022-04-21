//
// Created by dguco on 2022/4/13.
//

#ifndef RBTARR_MAP_HASH_HELPER_H
#define RBTARR_MAP_HASH_HELPER_H

#include <cstddef>
#include <new>
#include <memory>
#include <limits.h>
#include <string.h>
#include <map>
#include "rb_tree.h"

namespace rbt_hash
{
/**
 *迭代器class
* */
template<typename KeyType_,typename ValueType_,typename IndexType_,std::size_t Cap_ = 0>
class node_list_iterator
{
public:
    typedef RBTree<KeyType_,ValueType_,IndexType_,Cap_> tree_type;
    typedef RBTNode<KeyType_,ValueType_,IndexType_> node_type;
    typedef node_list_iterator<KeyType_,ValueType_,IndexType_,Cap_> iterator_type;
    typedef std::pair<KeyType_,ValueType_> *pointer;
    typedef std::pair<KeyType_,ValueType_> &reference;

    node_list_iterator(const iterator_type &other)
        : root_(other.root_), array_(other.array_),nodelist_(other.nodelist_),curnode_(other.curnode_)
    {}

    explicit node_list_iterator(IndexType_ proot_, node_type *parray_)
        : root_(proot_), array_(parray_),curnode_(NULL)
    {
        look_rbtree();
    }

    explicit node_list_iterator(node_type* node)
        : root_(NULL), array_(NULL),curnode_(node)
    {
        look_rbtree();
    }

    node_list_iterator()
        : root_(0), array_(NULL),curnode_(NULL)
    {}

    reference operator*() const
    {
        return curnode_->value();
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    iterator_type &operator++()
    {
        look_rbtree();
        return *this;
    }

    iterator_type operator++(int)
    {
        iterator_type __tmp(*this);
        ++*this;
        return __tmp;
    }

    bool operator==(const iterator_type &other) const
    {
        return curnode_ == other.curnode_;
    }

    bool operator!=(const iterator_type &other) const
    {
        return !(*this == other);
    }

    iterator_type &operator=(const iterator_type &other)
    {
        if (this != &other) {
            array_ = other.array_;
            nodelist_ = other.nodelist_;
            curnode_ = other.curnode_;
            root_ = other.root_;
        }
        return *this;
    }

    node_type *get_node(std::size_t index)
    {
        if (index > 0 && index <= Cap_) {
            return &array_[index - 1];
        }
        return 0;
    }

    void look_rbtree()
    {
        if(nodelist_.size() <= 0)
        {
            tree_type rbtree(array_,root_);
            if(!rbtree.isEmpty())
            {
                rbtree.inOrder(nodelist_);
            }
            if(nodelist_.size() > 0)
            {
                curnode_ = nodelist_.front();
                nodelist_.pop_front();
                if(nodelist_.size() == 0)
                {
                    root_ = curnode_->get_right();
                }
            }
        }
    }
private:
    IndexType_  root_;                //红黑树的根节点
    node_type   *array_;              //节点所属的数组
    std::list<node_type*> nodelist_;   //
    node_type   *curnode_;            //节点
};

/**
 * 内存管理器
 */
template<class KeyType_, class ValueType_,typename IndexType_,std::size_t Cap_ = 0>
class node_pool
{
public:
    typedef RBTNode<KeyType_,ValueType_,IndexType_> node_type;
    typedef RBTree<KeyType_,ValueType_,IndexType_,Cap_> tree_type;
    typedef node_list_iterator<KeyType_,ValueType_,IndexType_,Cap_> iterator;
    typedef std::pair<KeyType_,ValueType_> class_type;
    typedef class_type *pointer;
    typedef const class_type  *const_pointer;
    typedef class_type &reference;
    typedef const class_type &const_reference;

    //构造函数
    node_pool()
    {
        clear();
    }

    //析构函数
    ~node_pool()
    {
        clear();
    }

    //清理
    void clear()
    {
        //构造空闲链表信息
        node_array_[0].clear();
        //设置前向节点为空
        node_array_[0].set_prev(0);
        for (IndexType_ i = 1; i < Cap_; i++) {
            node_array_[i - 1].set_next(i + 1/*真正的索引+1*/);
            node_array_[i].clear();
            node_array_[i].set_prev(i - 1 + 1/*真正的索引+1*/);
        }
        //设置后向节点为空
        node_array_[Cap_ - 1].set_next(0);
        size_ = 0;
        //已用的节点链表头节点的索引
        used_node_head_root_ = 0;
        //默认数组首个元素即可用节点链表的头结点
        free_node_head_ = 1;
    }

    //内存池当前大小
    std::size_t size() const
    {
        return size_;
    }

    //内存池当前容量
    std::size_t cap() const
    {
        return Cap_;
    }

    //申请一个可用节点
    node_type *allocate_node(const class_type &v)
    {
        node_type *p = allocate(v);
        if (p) {
//            //插入到头结点
//            insert_head(get_node(used_node_head_), p);
//            //更新头结点索引
//            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    node_type *allocate_node()
    {
        node_type *p = allocate();
        if (p) {
//            //插入到头结点
//            insert_head(get_node(used_node_head_), p);
//            //更新头结点索引
//            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    //申请一个可用节点
    node_type *allocate_node(const class_type &v, node_type *next_node)
    {
        node_type *p = allocate(v);
        if (p) {
//            //插入到结点
//            insert_node(next_node, p);
//            //更新头结点索引
//            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    node_type *allocate_node(node_type *insert_node)
    {
        node_type *p = allocate();
        if (p) {
//            //插入到结点
//            insert_node(insert_node, p);
//            //更新头结点索引
//            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    void deallocate_node(node_type *node_)
    {
//        node_type *node_head = get_node(used_node_head_);
//        //当前节点是头结点
//        if (node_ == node_head) {
//            node_type *new_head = delete_head(node_);
//            //更新头结点信息
//            if (new_head) {
//                used_node_head_ = new_head->get_cur();
//            }
//            else //链表删空了
//            {
//                used_node_head_ = -1;
//            }
//        }
//        else {
//            delete_node(node_);
//        }
        //回收内存空间
        deallocate(node_);
    }

    node_type *get_node(IndexType_ index)
    {
        if (index > 0 && index <= Cap_)
        {
            return &node_array_[index - 1];
        }
        return NULL;
    }

    IndexType_ get_cur(node_type* node)
    {
        return ARRAY_OFFSET(node_array_,node);
    }

    tree_type make_rbtree(std::size_t root)
    {
        return tree_type(node_array_,root);
    }

    iterator make_iterator(IndexType_ root)
    {
        return iterator(root,node_array_);
    }

    iterator make_iterator(node_type* node)
    {
        return iterator(node);
    }
//    // 下面为访问已经分配对象的iterator
//    iterator begin()
//    {
//        if (used_node_head_ != -1) {
//            return iterator(get_node(used_node_head_), node_array_, Cap_);
//        }
//        else {
//            return end();
//        }
//    }
//
    iterator end()
    {
        return iterator();
    }

private:
    //申请空间
    node_type *allocate(const class_type &v)
    {
        if (size_ >= Cap_)
        {
            return 0;
        }
        node_type *p = get_node(free_node_head_);
        if (p)
        {
            size_++;
            //先把空闲头结点指向当前空闲头结点的下一个节点
            free_node_head_ = p->get_next();
            //call c++ placement new
            new(p->data()) class_type(v);
            p->init_rb();
            return p;
        }
        else
        {
            return NULL;
        }
    }

    //申请空间
    node_type* allocate()
    {
        if (size_ >= Cap_)
        {
            return NULL;
        }
        node_type *p = get_node(free_node_head_);
        if (p)
        {
            size_++;
            //先把空闲头结点指向当前空闲头结点的下一个节点
            free_node_head_ = p->get_next();
            //call c++ placement new
            new(p->data()) class_type();
            p->init_rb();
            return p;
        }
        else
        {
            return NULL;
        }
    }

    void deallocate(node_type *node_)
    {
        if (node_ == NULL)
        {
            return;
        }
        //调用析构函数
        node_->value().~class_type();
        free_node_head_ = get_cur(node_);
        //插入空闲链表头部
        insert_node(get_node(free_node_head_), node_);
    }

    //插入一个节点到指定节点前
    void insert_node(node_type *tar_node, node_type *new_head)
    {
        if (new_head == NULL)
        {
            return;
        }
        if(tar_node)
        {
            int tar_prev = tar_node->get_prev();
            node_type *tar_prev_node = get_node(tar_prev);
            //有前节点
            if (tar_prev_node)
            {
                //目标节点前指针指向要插入的节点
                tar_prev_node->set_next(get_cur(new_head));
                //要插入的节点前指针指向tar_prev_node
                new_head->set_prev(get_cur(tar_prev_node));
            }
            else
            {
                //要插入的节点前指针置为0
                new_head->set_prev(0);
            }
            tar_node->set_prev(get_cur(new_head));
            new_head->set_next(get_cur(tar_node));
        }else
        {
            new_head->dis_from_list();
        }
    }

    //删除头结点，返回新的头结点
    node_type *delete_head(node_type *old_head)
    {
        if (old_head == 0) {
            return 0;
        }

        node_type *new_head = get_node(old_head->get_next());
        if (new_head) {
            new_head->set_prev(-1);
        }

        old_head->dis_from_list();
        return new_head;
    }

    //删除结点
    void delete_node(node_type *node_)
    {
        if (node_ == 0) {
            return;
        }

        int next = node_->get_next();
        int prev = node_->get_prev();

        node_type *prev_node = get_node(node_->get_prev());
        node_type *next_node = get_node(node_->get_next());
        if (prev_node) {
            prev_node->set_next(next);
        }
        if (next_node) {
            next_node->set_prev(prev);
        }
        node_->dis_from_list();
    }
private:
    IndexType_ size_;                            //内存池已用数量
    IndexType_ used_node_head_root_;             //已用的节点链表头节点的索引
    IndexType_ free_node_head_;                  //空闲的节点链表头节点的索引
    node_type  node_array_[Cap_];
};
}

#endif //RBTARR_MAP_HASH_HELPER_H
