//
// Created by dguco on 2022/4/13.
//

#ifndef RBTARR_MAP_RBTARR_MAP_H
#define RBTARR_MAP_RBTARR_MAP_H


#include <map>
#include <limits.h>
#include "hash_helper.h"
#include "hash_function.h"

using namespace std;

namespace rbt_hash
{
template <class KeyType_, class ValueType_, std::size_t _Cap = 100>
class RbtHashMap
{
public:
#if _Cap <= USHRT_MAX - 1
    typedef unsigned short IndexType_;
#elif _Cap <= UINT_MAX - 1
    typedef unsigned int IndexType_;
#else
    typedef unsigned long long IndexType_;
#endif
    typedef node_pool<KeyType_,ValueType_,IndexType_, _Cap> hash_array;
    typedef typename hash_array::tree_type tree_type;
    typedef typename hash_array::class_type class_type;
    typedef typename hash_array::iterator iterator;
    typedef typename hash_array::reference reference;
    typedef typename hash_array::node_type node_type;

    iterator begin()
    {
        return hash_array_.begin ();
    }

    iterator end()
    {
        return hash_array_.end();
    }

    std::size_t cap() const
    {
        return hash_array_.cap();
    }

    std::size_t size() const
    {
        return hash_array_.size();
    }

    bool insert(const class_type& v)
    {
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(v.first) % _Cap;

        tree_type rb_tree = hash_array_.make_rbtree(buckets_[bucket].root_);
        //该bucket是空的
        if ( rb_tree.isEmpty())
        {
            //申请一个节点
            node_type* new_node = hash_array_.allocate_node(v);
            if(!new_node)
            {
                return false;
            }
            rb_tree.insert(new_node);
            //设置链表头和尾
            buckets_[bucket].root_ = buckets_[bucket].minson_ = buckets_[bucket].maxson_ = rb_tree.root();
            return true;
        }
        node_type* old_node = rb_tree.search(v.first);
        if(old_node != NULL)
        {
            //如果存在则更新旧值返回成功
            old_node->value().second = v.second;
            return true;
        }

        //申请一个节点
        node_type* new_node = hash_array_.allocate_node(v,hash_array_.get_node(buckets_[bucket].root_));
        if( !new_node )
        {
            return false;
        }
        //先记录并清除对应的前后的rbtree的信息，插入新节点后，该信息可能丢失
        IndexType_  preRoot = 0;
        IndexType_  nextRoot = 0;
        node_type* oldmin_node = NULL;
        node_type* oldmax_node = NULL;
        {
            oldmin_node = hash_array_.get_node(buckets_[bucket].minson_);
            if(oldmin_node != NULL)
            {
                preRoot = oldmin_node->get_left();
                rb_tree.setLeft(oldmin_node,NULL);
                rb_tree.setRbColor(oldmin_node,rb_tree.rbColorOf(oldmin_node));
            }
            oldmax_node = hash_array_.get_node(buckets_[bucket].maxson_);
            if(oldmax_node != NULL)
            {
                nextRoot = oldmax_node->get_right();
                rb_tree.setRight(oldmax_node,NULL);
                rb_tree.setRbColor(oldmax_node,rb_tree.rbColorOf(oldmax_node));
            }
        }

        //把新节点插入红黑树
        rb_tree.insert(new_node);
        if(oldmin_node != NULL)
        {
            if(new_node->value().first < oldmin_node->value().first)
            {
                new_node->set_left(preRoot);
                new_node->set_color((RBTColor)(RB_MIN_NODE | new_node->get_color()));
                buckets_[bucket].minson_ = hash_array_.get_cur(new_node);
            }else
            {
                oldmin_node->set_left(preRoot);
                oldmin_node->set_color((RBTColor)(RB_MIN_NODE | oldmin_node->get_color()));
            }
        }

        if(oldmax_node != NULL)
        {
            if(new_node->value().first > oldmax_node->value().first)
            {
                new_node->set_right(nextRoot);
                new_node->set_color((RBTColor)(RB_MAX_NODE | new_node->get_color()));
                buckets_[bucket].maxson_ = hash_array_.get_cur(new_node);
            }else
            {
                oldmax_node->set_right(nextRoot);
                oldmax_node->set_color((RBTColor)(RB_MAX_NODE | oldmax_node->get_color()));
            }
        }

        //更新最小节点结点信息
        buckets_[bucket].root_ = rb_tree.root();
        return true;
    }

    bool insert(const KeyType_& k, const ValueType_& v)
    {
        return insert(std::make_pair(k,v));
    }

    iterator find(const KeyType_& k)
    {
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(k) % _Cap;

        tree_type rb_tree = hash_array_.make_rbtree(buckets_[bucket].root_);
        //该bucket是空的
        if ( rb_tree.isEmpty())
        {
            return this->end();
        }
        return hash_array_.make_iterator(rb_tree.search(k));
    }

    void erase( iterator it )
    {

    }

    std::size_t erase( const KeyType_& k )
    {
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(k) % _Cap;

        tree_type rb_tree = hash_array_.make_rbtree(buckets_[bucket].root_);
        //该bucket是空的
        if ( rb_tree.isEmpty())
        {
            return 0;
        }
        node_type* node = rb_tree.remove(k);
        buckets_[bucket].root_ = rb_tree.root();
        hash_array_.deallocate_node(node);
        return  0;
    }

    void clear()
    {
        for( IndexType_ t = 0; t < _Cap; ++t )
        {
            buckets_[t].root_ = 0;
            buckets_[t].minson_ = 0;
            buckets_[t].maxson_ = 0;
        }
        hash_array_.clear();
        printf("========= map index size = %d\n",sizeof(IndexType_));
    }

    RbtHashMap() {clear();}
    ~RbtHashMap() {clear();}

private:
    RbtHashMap(const RbtHashMap& other );

private:
    struct bucket_type
    {
        IndexType_ root_;  			    //相同bucket rbtree根节点索引
        IndexType_ minson_;  			//相同bucket rbtree最小节点索引
        IndexType_ maxson_;  			//相同bucket rbtree最大节点索引
    };

    bucket_type		buckets_[_Cap];     //bucket array
    hash_array	 	hash_array_;		//内存管理器
};
}
#endif //RBTARR_MAP_RBTARR_MAP_H
