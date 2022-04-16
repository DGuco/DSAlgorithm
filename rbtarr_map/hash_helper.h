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

namespace rbt_hash
{

enum  RBColor
{
    RBColorNone = 0,
    RBColorRED = 1,
    RBColorBLACK = 2,
};

template<typename INDEX_TYPE>
struct listNode
{
    INDEX_TYPE                 cur_;                         //当前节点在数组中的索引位置+1
    INDEX_TYPE                 next_;                        //下一个节点在数组中的索引位置+1
    INDEX_TYPE                 prev_;                        //前一个节点在数组中的索引位置+1
    INDEX_TYPE                 _;                            //no used
    unsigned  char             color_;                       //节点color
};

template<typename INDEX_TYPE>
struct rbtNode
{
    INDEX_TYPE                 cur_;                          //当前节点在数组中的索引位置+1
    INDEX_TYPE                 parent_;                       //父亲节点在数组中的索引位置+1
    INDEX_TYPE                 left_;                         //左子节点在数组中的索引位置+1
    INDEX_TYPE                 right_;                        //右子节点在数组中的索引位置+1
    unsigned  char             color_;                        //节点color
};

/**
 *单个节点类
* */
#pragma pack(1)
template<typename CLASS_TYPE,typename INDEX_TYPE = unsigned int>
struct RBTNode
{
public:
    typedef CLASS_TYPE &reference;
    typedef CLASS_TYPE *pointer;
    RBTNode()
    {
        clear_list();
    }

    ~RBTNode()
    {
        clear_list();
    }

    void clear_list()
    {
        list_node_.cur_ = 0;
        list_node_.next_ = 0;
        list_node_.prev_ = 0;
        list_node_._ = 0;
        list_node_.color_ = RBColorNone;
        memset(data_, 0, sizeof(CLASS_TYPE));
    }

    void clear_rb()
    {
        rbt_node_.cur_ = 0;
        rbt_node_.parent_ = 0;
        rbt_node_.left_ = 0;
        rbt_node_.right_ = 0;
        rbt_node_.color_ = RBColorNone;
        memset(data_, 0, sizeof(CLASS_TYPE));
    }
    void set_cur(INDEX_TYPE value)  { list_node_.cur_ = value;}
    INDEX_TYPE  get_cur()           { return list_node_.cur_;}

    void set_next(INDEX_TYPE value)  { list_node_.next_ = value;}
    INDEX_TYPE  get_next()           { return list_node_.next_;}

    void set_prev(INDEX_TYPE value)  { list_node_.prev_ = value;}
    INDEX_TYPE  get_prev()           { return list_node_.prev_;}

    void dis_from_list()
    {
      set_next(0);
      set_prev(0);
    }

    reference value()
    {
        return *(reinterpret_cast<pointer>(data_));
    }

    char* data()
    {
        return data_;
    }
private:
    char data_[sizeof(CLASS_TYPE)];       //真正存放对象信息的内存块 c++ placement new operator把对象new到指定的内存位置
    union
    {
        listNode<INDEX_TYPE> list_node_;
        rbtNode<INDEX_TYPE>  rbt_node_;
    };
};

/**
 *迭代器class
* */
template<typename ClassType_>
class node_list_iterator
{
public:
    typedef RBTNode<ClassType_> node_type;
    typedef node_list_iterator<ClassType_> iterator_type;
    typedef ClassType_ *pointer;
    typedef ClassType_ &reference;

    node_list_iterator(const iterator_type &other)
        : ptr_(other.ptr_), array_(other.array_), size_(other.size_)
    {}

    explicit node_list_iterator(node_type *pptr_, node_type *parray_, std::size_t psize_)
        : ptr_(pptr_), array_(parray_), size_(psize_)
    {}

    node_list_iterator()
        : ptr_(0), array_(0), size_(0)
    {}

    reference operator*() const
    {
        return ptr_->value();
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    iterator_type &operator++()
    {
        ptr_ = get_node(ptr_->get_next());
        return *this;
    }

    iterator_type operator++(int)
    {
        iterator_type __tmp(*this);
        ++*this;
        return __tmp;
    }

    iterator_type &operator--()
    {
        ptr_ = get_node(ptr_->prev());
        return *this;
    }

    iterator_type operator--(int)
    {
        iterator_type __tmp(*this);
        --*this;
        return __tmp;
    }

    bool operator==(const iterator_type &other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const iterator_type &other) const
    {
        return !(*this == other);
    }

    iterator_type &operator=(const iterator_type &other)
    {
        if (this != &other) {
            ptr_ = other.ptr_;
            array_ = other.array_;
            size_ = other.size_;
        }
        return *this;
    }

    node_type *get_node(std::size_t index)
    {
        if (index > 0 && index <= size_) {
            return &array_[index - 1];
        }
        return 0;
    }
private:
    node_type *ptr_;            //节点指针
    node_type *array_;            //节点所属的数组
    std::size_t size_;            //数组长度
};

template<typename CLASS_TYPE>
struct node_list_const_iterator
{
public:
    typedef RBTNode<CLASS_TYPE> node_type;
    typedef node_list_const_iterator<CLASS_TYPE> iterator_type;
    typedef const CLASS_TYPE *pointer;
    typedef const CLASS_TYPE &reference;

    node_list_const_iterator(const iterator_type &other)
        : ptr_(other.ptr_), array_(other.array_), size_(other.size_)
    {}

    explicit node_list_const_iterator(node_type *pptr_, node_type *parray_, int psize_)
        : ptr_(pptr_), array_(parray_), size_(psize_)
    {}

    node_list_const_iterator()
        : ptr_(0), array_(0), size_(0)
    {}

    reference operator*() const
    {
        return ptr_->value();
    }

    pointer operator->() const
    {
        return &(operator*());
    }

    iterator_type &operator++()
    {
        ptr_ = get_node(ptr_->next());
        return *this;
    }

    iterator_type operator++(int)
    {
        iterator_type __tmp(*this);
        ++*this;
        return __tmp;
    }

    iterator_type &operator--()
    {
        ptr_ = get_node(ptr_->prev());
        return *this;
    }

    iterator_type operator--(int)
    {
        iterator_type __tmp(*this);
        --*this;
        return __tmp;
    }

    bool operator==(const iterator_type &other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const iterator_type &other) const
    {
        return !(*this == other);
    }

    iterator_type &operator=(const iterator_type &other)
    {
        if (this != &other) {
            ptr_ = other.ptr_;
            array_ = other.array_;
            size_ = other.size_;
        }
        return *this;
    }

    node_type *get_node(int index)
    {
        if (index > 0 && index <= size_) {
            return &array_[index - 1];
        }
        return 0;
    }
private:
    node_type *ptr_;            //节点指针
    node_type *array_;            //节点所属的数组
    std::size_t size_;            //数组长度
};

/**
 * 内存管理器
 */
template<typename ClassType_,typename IndexType_,std::size_t Cap_ = 0>
class node_pool
{
public:
    typedef RBTNode<ClassType_, IndexType_> node_type;
    typedef node_list_iterator<ClassType_> iterator;
    typedef node_list_const_iterator<ClassType_> const_iterator;
    typedef ClassType_ *pointer;
    typedef const ClassType_ *const_pointer;
    typedef ClassType_ &reference;
    typedef const ClassType_ &const_reference;

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
        node_array_[0].clear_list();
        node_array_[0].set_cur(0 + 1/*真正的索引+1*/);
        //设置前向节点为空
        node_array_[0].set_prev(0);
        for (IndexType_ i = 1; i < Cap_; i++) {
            node_array_[i - 1].set_next(i + 1/*真正的索引+1*/);
            node_array_[i].clear_list();
            node_array_[i].set_prev(i - 1 + 1/*真正的索引+1*/);
            node_array_[i].set_cur(i + 1/*真正的索引+1*/);
        }
        //设置后向节点为空
        node_array_[Cap_ - 1].set_next(0);
        size_ = 0;
        //已用的节点链表头节点的索引
        used_node_head_ = 0;
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
    node_type *allocate_node(const ClassType_ &v)
    {
        node_type *p = allocate(v);
        if (p) {
            //插入到头结点
            insert_head(get_node(used_node_head_), p);
            //更新头结点索引
            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    node_type *allocate_node()
    {
        node_type *p = allocate();
        if (p) {
            //插入到头结点
            insert_head(get_node(used_node_head_), p);
            //更新头结点索引
            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    //申请一个可用节点
    node_type *allocate_node(const ClassType_ &v, node_type *next_node)
    {
        node_type *p = allocate(v);
        if (p) {
            //插入到结点
            insert_node(next_node, p);
            //更新头结点索引
            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    node_type *allocate_node(node_type *insert_node)
    {
        node_type *p = allocate();
        if (p) {
            //插入到结点
            insert_node(insert_node, p);
            //更新头结点索引
            used_node_head_ = p->get_cur();
            return p;
        }
        return 0;
    }

    void deallocate_node(node_type *node_)
    {
        node_type *node_head = get_node(used_node_head_);
        //当前节点是头结点
        if (node_ == node_head) {
            node_type *new_head = delete_head(node_);
            //更新头结点信息
            if (new_head) {
                used_node_head_ = new_head->get_cur();
            }
            else //链表删空了
            {
                used_node_head_ = -1;
            }
        }
        else {
            delete_node(node_);
        }
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

    iterator make_iteator(std::size_t index)
    {
        return iterator(get_node(index), node_array_, Cap_);
    }

    const_iterator make_const_iteator(std::size_t index)
    {
        return const_iterator(get_node(index), node_array_, Cap_);
    }

    // 下面为访问已经分配对象的iterator
    iterator begin()
    {
        if (used_node_head_ != -1) {
            return iterator(get_node(used_node_head_), node_array_, Cap_);
        }
        else {
            return end();
        }
    }

    iterator end()
    {
        return iterator();
    }

    const_iterator begin() const
    {
        if (used_node_head_ != -1) {
            return const_iterator(get_node(used_node_head_), node_array_, Cap_);
        }
        else {
            return end();
        }
    }

    const_iterator end() const
    {
        return const_iterator();
    }

private:
    //申请空间
    node_type *allocate(const ClassType_ &v)
    {
        if (size_ >= Cap_) {
            return 0;
        }
        node_type *p = get_node(free_node_head_);
        if (p) {
            size_++;
            //先把空闲头结点指向当前空闲头结点的下一个节点
            free_node_head_ = p->get_next();
            //call c++ placement new
            new(p->date()) ClassType_(v);
            return p;
        }
        else {
            return NULL;
        }
    }

    //申请空间
    node_type* allocate()
    {
        if (size_ >= Cap_) {
            return NULL;
        }
        else {
            node_type *p = get_node(free_node_head_);
            if (p) {
                size_++;
                //先把空闲头结点指向当前空闲头结点的下一个节点
                free_node_head_ = p->get_next();
                //call c++ placement new
                new(p->data()) ClassType_();
                return p;
            }
            else {
                return NULL;
            }
        }
    }

    void deallocate(node_type *node_)
    {
        if (node_ == 0) {
            return;
        }
        //调用析构函数
        node_->value().~CLASS_TYPE();
        //插入空闲链表头部
        insert_head(get_node(free_node_head_), node_);
    }

    //插入一个节点到链表头部
    void insert_head(node_type *old_head, node_type *new_head)
    {
        if (old_head) {
            if (new_head) {
                //老的头结点前指针指向新的头结点
                old_head->set_prev(new_head->get_cur());
                //新的头结点后指针指向老的的头节点
                new_head->set_next(old_head->get_cur());
                //新的头结点前指针置为0
                new_head->set_prev(0);
            }
        }
        else {
            if (new_head) {
                new_head->dis_from_list();
            }
        }
    }

    //插入一个节点到指定节点前
    void insert_node(node_type *tar_node, node_type *src_node)
    {
        if (tar_node == 0 || src_node == 0) {
            return;
        }
        int tar_prev = tar_node->get_prev();
        node_type *tar_prev_node = get_node(tar_prev);
        //有前节点
        if (tar_prev_node) {
            //目标节点前指针指向要插入的节点
            tar_prev_node->set_next(src_node->get_cur());
            //要插入的节点前指针指向tar_prev_node
            src_node->set_prev(tar_prev_node->get_cur());
        }
        else {
            //要插入的节点前指针置为0
            src_node->set_prev(0);
        }
        tar_node->set_prev(src_node->get_cur());
        src_node->set_next(tar_node->get_cur());
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
    IndexType_ size_;                    //内存池已用数量
    IndexType_ used_node_head_;                  //已用的节点链表头节点的索引
    IndexType_ free_node_head_;                  //空闲的节点链表头节点的索引
    node_type node_array_[Cap_];
};
}

#endif //RBTARR_MAP_HASH_HELPER_H
