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
    typedef RBTNode<KeyType_,IndexType_> node_type;
    typedef node_list_iterator<KeyType_,ValueType_,IndexType_,Cap_> iterator_type;
    typedef std::pair<KeyType_,ValueType_*> *pointer;
    typedef std::pair<KeyType_,ValueType_*> &reference;

    node_list_iterator(const iterator_type &other)
        : root_(other.root_), node_array_(other.node_array_), data_array_(other.data_array_), nodelist_(other.nodelist_), curnode_(other.curnode_)
    {}

    explicit node_list_iterator(IndexType_ proot_, node_type *parray_,ValueNode<KeyType_,ValueType_>* pdata_)
        : root_(proot_), node_array_(parray_), data_array_(pdata_), curnode_(NULL)
    {
        look_rbtree();
    }

    explicit node_list_iterator(node_type* node,node_type *parray_,ValueNode<KeyType_,ValueType_>* pdata_)
        : root_(0), node_array_(parray_), data_array_(pdata_), curnode_(node)
    {

    }

    node_list_iterator()
        : root_(0), node_array_(NULL), data_array_(NULL), curnode_(NULL)
    {}

    reference operator*()
    {
        IndexType_ index = get_cur();
        if(index >= 0 && index <= Cap_)
        {
            iteator_.first = curnode_->get_key();
            iteator_.second = data_array_[index - 1].pointer();
        }else
        {
            iteator_.first = curnode_->get_key();
            iteator_.second = NULL;
        }
        return iteator_;
    }

    pointer operator->()
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
            node_array_ = other.node_array_;
            nodelist_ = other.nodelist_;
            curnode_ = other.curnode_;
            root_ = other.root_;
        }
        return *this;
    }

    node_type *get_node(std::size_t index) const
    {
        if (index > 0 && index <= Cap_) {
            return &node_array_[index - 1];
        }
        return 0;
    }

    IndexType_ get_cur() const
    {
        if(curnode_)
        {
            return ARRAY_OFFSET(node_array_, curnode_);
        }
        return 0;
    }

    void look_rbtree()
    {
        if(nodelist_.size() <= 0)
        {
            nodelist_.clear();
            tree_type rbtree(node_array_, data_array_, root_);
            if(!rbtree.isEmpty())
            {
                rbtree.inOrder(nodelist_);
            }
        }
        if(nodelist_.size() > 0)
        {
            curnode_ = nodelist_.front();
            nodelist_.pop_front();
            if(nodelist_.size() == 0)
            {
                root_ = curnode_->get_right();
            }
        }else
        {
            curnode_ = NULL;
        }
    }

    node_type* curNode()
    {
        return curnode_;
    }

private:
    IndexType_                      root_;                //红黑树的根节点
    node_type*                      node_array_;              //节点所属的数组
    ValueNode<KeyType_,ValueType_>*          data_array_;
    std::list<node_type*>           nodelist_;   //
    std::pair<KeyType_,ValueType_*> iteator_;
    node_type                       *curnode_;            //节点
};

/**
 * 内存管理器
 */
template<typename KeyType_, typename ValueType_,typename IndexType_,std::size_t Cap_ = 0>
class node_pool
{
public:
    typedef RBTNode<KeyType_,IndexType_> node_type;
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
        memset(data_array_,0,sizeof(ValueNode<KeyType_,ValueType_>) * Cap_);
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
        rb_tree_head_root_ = 0;
        //默认数组首个元素即可用节点链表的头结点
        free_node_head_ = 1;
    }

    //析构函数
    ~node_pool()
    {
        clear();
    }

    //清理
    void clear()
    {
        for(int index = 0;index < Cap_;index++)
        {
            data_array_[index].value().~ValueType_();
        }
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
        rb_tree_head_root_ = 0;
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
        if (p)
        {
            return p;
        }
        return 0;
    }

    //申请一个可用节点
    node_type *allocate_node(const class_type &v, node_type *next_node)
    {
        node_type *p = allocate(v);
        if (p) {
            return p;
        }
        return 0;
    }

    void deallocate_node(node_type *node_)
    {
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

    void updateValue(node_type* node,ValueType_ value)
    {
        IndexType_ index = get_cur(node);
        if(index > 0 && index <= Cap_)
        {
            //调用析构函数
            data_array_[index - 1].value() = value;
        }
    }

    tree_type make_rbtree(std::size_t root)
    {
        return tree_type(node_array_,data_array_,root);
    }

    iterator make_iterator(IndexType_ root)
    {
        return iterator(root,node_array_,data_array_);
    }

    iterator make_iterator(node_type* node)
    {
        return iterator(node,node_array_,data_array_);
    }

    void set_rb_tree_head_root(IndexType_ value)
    {
        rb_tree_head_root_ = value;
    }

    IndexType_ rb_tree_head_root()
    {
        return rb_tree_head_root_;
    }
    // 下面为访问已经分配对象的iterator
    iterator begin()
    {
        if (rb_tree_head_root_ != 0) {
            return iterator(rb_tree_head_root_, node_array_,data_array_);
        }
        else {
            return end();
        }
    }

    iterator end()
    {
        return iterator();
    }

    void resetMapFromBinary(ValueNode<KeyType_,ValueType_>* data)
    {
        memcpy(data_array_,data,sizeof(ValueNode<KeyType_,ValueType_>) * Cap_);
    }

    ValueNode<KeyType_,ValueType_>* data()
    {
        return  data_array_;
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
        IndexType_ index = free_node_head_;
        if (p)
        {
            size_++;
            //call c++ placement new
            //把空闲头结点指向当前空闲头结点的下一个节点
            free_node_head_ = p->get_next();
            p->set_key(v.first);
            new(data_array_[index - 1].data) ValueType_(v.second);
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
        IndexType_ index = get_cur(node_);
        if(index > 0 && index <= Cap_)
        {
            //调用析构函数
            data_array_[index - 1].value().~ValueType_();
        }
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
private:
    IndexType_                  size_;                            //内存池已用数量
    IndexType_                  rb_tree_head_root_;               //红黑树链的头的根节点索引
    IndexType_                  free_node_head_;                  //空闲的节点链表头节点的索引
    //这里为了序列化map的时候能够快速的序列化整个数据的内存块，把真正的数据和红黑树节点信息分成两个数组
    //可以相互通过同一个索引快速取到对应的信息
    node_type                   node_array_[Cap_];
    ValueNode<KeyType_,ValueType_>       data_array_[Cap_];
};
}

#endif //RBTARR_MAP_HASH_HELPER_H
