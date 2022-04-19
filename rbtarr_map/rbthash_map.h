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
template <class KeyType_, class ValueType_, std::size_t _Cap = 666666666>
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
    typedef std::pair<KeyType_, ValueType_> value_type;
    typedef node_pool<value_type, IndexType_, _Cap> hash_array;

    typedef typename hash_array::iterator iterator;
    typedef typename hash_array::const_iterator const_iterator;
    typedef typename hash_array::reference reference;
    typedef typename hash_array::const_reference const_reference;
    typedef typename hash_array::node_type node_type;

    iterator begin()
    {
        return hash_array_.begin ();
    }

    iterator end()
    {
        return hash_array_.end();
    }

    const_iterator begin() const
    {
        return hash_array_.begin();
    }

    const_iterator end() const
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

    bool insert(const value_type& v)
    {
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(v.first) % _Cap;

        iterator it_begin = hash_array_.make_iteator(buckets_[bucket].root_);

        //该bucket是空的
        if ( it_begin == this->end())
        {
            //申请一个节点
            node_type* new_node = hash_array_.allocate_node(v);
            if(!new_node)
            {
                return false;
            }
            //设置链表头和尾
            buckets_[bucket].root_ = buckets_[bucket].minson_ = new_node->get_cur();
            return true;
        }

        //hash冲突
        iterator it_end  = hash_array_.make_iteator(buckets_[bucket].minson_);
        ++it_end;

        //检查是否存在改key的元素
        while ( it_begin != it_end )
        {
            //该key对应的元素已经存在，插入失败
            if (it_begin->first == v.first)
            {
                return false;
            }
            ++it_begin;
        }

        //申请一个节点
        node_type* new_node = hash_array_.allocate_node(v,hash_array_.get_node(buckets_[bucket].root_));
        if( !new_node )
        {
            return false;
        }

        //更新头结点信息
        buckets_[bucket].root_ = new_node->get_cur();
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

        iterator it_begin = hash_array_.make_iteator(buckets_[bucket].root_);

        if ( it_begin == this->end() )
        {
            return this->end();
        }

        iterator it_end  = hash_array_.make_iteator(buckets_[bucket].minson_);
        ++it_end;

        while ( it_begin != it_end )
        {
            if ( it_begin->first == k )
            {
                return it_begin;
            }

            ++it_begin;
        }
        return this->end();
    }

    const_iterator find(const KeyType_& k) const
    {
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(k) % _Cap;

        const_iterator it_begin = hash_array_.make_const_iteator(buckets_[bucket].root_);

        if ( it_begin == this->end() )
        {
            return this->end();
        }

        const_iterator it_end  = hash_array_.make_const_iteator(buckets_[bucket].minson_);
        ++it_end;

        while ( it_begin != it_end )
        {
            if ( it_begin->first == k )
            {
                return it_begin;
            }

            ++it_begin;
        }

        return this->end();
    }

    void erase( iterator it )
    {
        if ( it == this->end() )
        {
            return;
        }
        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(it->first) % _Cap;

        iterator it_begin = hash_array_.make_iteator(buckets_[bucket].root_);
        iterator it_end = hash_array_.make_iteator(buckets_[bucket].minson_);

        //hash链表只有自己
        if ( it == it_begin && it == it_end )
        {
            buckets_[bucket].root_ = buckets_[bucket].minson_ = -1;
        }
        else if (  it == it_begin ) //hrash链表自己是头
        {
            ++ it_begin;
            buckets_[bucket].root_ = hash_array_.get_node(hash_array_.get_node(buckets_[bucket].root_)->get_next())->get_cur();
        }
        else if ( it == it_end )//hash链表自己是尾
        {
            -- it_end;
            buckets_[bucket].minson_ = hash_array_.get_node(hash_array_.get_node(buckets_[bucket].minson_)->get_prev())->get_cur();
        }

        hash_array_.deallocate_node( &*it);
    }

    std::size_t erase( const KeyType_& k )
    {
        erase (find(k));
        return size();
    }

    void clear()
    {
        for( IndexType_ t = 0; t < _Cap; ++t )
        {
            buckets_[t].root_ = 0;
            buckets_[t].minson_ = 0;
        }
        hash_array_.clear();
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
    };

    bucket_type		buckets_[_Cap];     //bucket array
    hash_array	 	hash_array_;		//内存管理器
};
}
#endif //RBTARR_MAP_RBTARR_MAP_H
