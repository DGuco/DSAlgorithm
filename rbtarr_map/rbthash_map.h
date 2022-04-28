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
template <class KeyType_, class ValueType_, std::size_t _Cap = 0>
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
            //新的rbtree成为新的树链头部
            updateNextTree(new_node,hash_array_.rb_tree_head_root());
            updatePreTree(new_node,0);
            updatePreTree(hash_array_.rb_tree_head_root(),buckets_[bucket].root_);
            hash_array_.set_rb_tree_head_root(rb_tree.root());
            return true;
        }
        node_type* old_node = rb_tree.search(v.first);
        if(old_node != NULL)
        {
            //如果存在则更新旧值返回成功
            hash_array_.updateValue(old_node,v.second);
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
        //把新节点插入红黑树
        rb_tree.insert(new_node);
        if(oldmin_node != NULL)
        {
            if(new_node->get_key() < oldmin_node->get_key())
            {
                updatePreTree(new_node,preRoot);
                buckets_[bucket].minson_ = hash_array_.get_cur(new_node);
            }else
            {
                updatePreTree(oldmin_node,preRoot);
            }
        }

        if(oldmax_node != NULL)
        {
            if(new_node->get_key() > oldmax_node->get_key())
            {
                updateNextTree(new_node,nextRoot);
                buckets_[bucket].maxson_ = hash_array_.get_cur(new_node);
            }else
            {
                updateNextTree(oldmax_node,nextRoot);
            }
        }

        //update bucket info
        if(buckets_[bucket].root_ != rb_tree.root())
        {
            if(buckets_[bucket].root_ == hash_array_.rb_tree_head_root())
            {
                hash_array_.set_rb_tree_head_root(rb_tree.root());
            }
            buckets_[bucket].root_ = rb_tree.root();
            updateNextTree(preRoot,rb_tree.root());
            updatePreTree(nextRoot,rb_tree.root());
        }
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
        if(it == hash_array_.end())
        {
            return;
        }

        hash_function::hash<KeyType_> hash_func;
        std::size_t bucket = hash_func(it->first) % _Cap;
        tree_type rb_tree = hash_array_.make_rbtree(buckets_[bucket].root_);
        //该bucket是空的
        if ( rb_tree.isEmpty())
        {
            return;
        }
        //先记录并清除对应的前后的rbtree的信息，erase节点后，该信息可能丢失
        IndexType_  preRoot = 0;
        IndexType_  nextRoot = 0;
        node_type* oldmin_node = NULL;
        node_type* oldmax_node = NULL;
        {
            oldmin_node = hash_array_.get_node(buckets_[bucket].minson_);
            if(oldmin_node != NULL)
            {
                preRoot = oldmin_node->get_left();
            }
            oldmax_node = hash_array_.get_node(buckets_[bucket].maxson_);
            if(oldmax_node != NULL)
            {
                nextRoot = oldmax_node->get_right();
            }
        }
        node_type* remove_node = rb_tree.remove(it.curNode());
        if(remove_node == NULL)
        {
            return;
        }
        if(!rb_tree.isEmpty())
        {
            if(buckets_[bucket].minson_ == hash_array_.get_cur(remove_node))
            {
                updatePreTree(rb_tree,preRoot);
            }

            if(buckets_[bucket].maxson_  == hash_array_.get_cur(remove_node))
            {
                updateNextTree(rb_tree,nextRoot);
            }

            if(buckets_[bucket].root_ != rb_tree.root())
            {
                if(buckets_[bucket].root_ == hash_array_.rb_tree_head_root())
                {
                    hash_array_.set_rb_tree_head_root(rb_tree.root());
                }
                buckets_[bucket].root_ = rb_tree.root();
                updateNextTree(preRoot,buckets_[bucket].root_);
                updatePreTree(nextRoot,buckets_[bucket].root_);
            }
        }else//树删空了
        {
            //update bucket info
            buckets_[bucket].root_ = buckets_[bucket].minson_ = buckets_[bucket].maxson_ = 0;
            node_type* pre_root_node = hash_array_.get_node(preRoot);
            node_type* next_root_node = hash_array_.get_node(nextRoot);
            //前后两棵树不为空，把前树的尾部指向后树，后树的头部指向前树
            if(pre_root_node && next_root_node)
            {
                updateNextTree(preRoot,nextRoot);
                updatePreTree(nextRoot,preRoot);
            }
                //前树不为空后树为空
            else if(pre_root_node)
            {
                updateNextTree(preRoot,0);
            }
                //前树为空后树不为空,调整树链指针，并且后树成为新的树链的头
            else if(next_root_node)
            {
                updatePreTree(nextRoot,0);
                hash_array_.set_rb_tree_head_root(nextRoot);
            }
        }
        hash_array_.deallocate_node(remove_node);
    }

    void erase( const KeyType_& k )
    {
        return erase(find(k));
    }

    void clear()
    {
        bool sss = sizeof(ValueType_) == sizeof(ValueNode<KeyType_,ValueType_>);
        for( IndexType_ t = 0; t < _Cap; ++t )
        {
            buckets_[t].root_ = 0;
            buckets_[t].minson_ = 0;
            buckets_[t].maxson_ = 0;
        }
        hash_array_.clear();
    }

    ValueNode<KeyType_,ValueType_>* data()
    {
        return hash_array_.data();
    }

    void initFromBinaryData(ValueNode<KeyType_,ValueType_>* data)
    {
        hash_array_.resetMapFromBinary(data);
        for(IndexType_ index = 0;index < _Cap;index++)
        {
            if(data[index].IsValid())
            {
                insert(data[index].Key(),data[index].value());
            }
        }
    }

    RbtHashMap()
    {
        if(DEBUG_RB_TREE)
        {
            printf("RbtHashMap index size = %ld\n",sizeof(IndexType_));
        }
        clear();
    }
    ~RbtHashMap()
    {
        clear();
    }
private:
    void updatePreTree(tree_type rb_tree,IndexType_ preRoot)
    {
        node_type* newmin_node = rb_tree.minimum();
        if(newmin_node)
        {
            newmin_node->set_left(preRoot);
            if(preRoot != 0)
            {
                newmin_node->set_color((RBTColor)(RB_MIN_NODE | newmin_node->get_color()));
            }else
            {
                newmin_node->set_color(tree_type::rbColorOf(newmin_node));
            }
            hash_function::hash<KeyType_> hash_func;
            std::size_t bucket = hash_func(newmin_node->get_key()) % _Cap;
            buckets_[bucket].minson_ = hash_array_.get_cur(newmin_node);
        }
    }

    void updatePreTree(IndexType_ root,IndexType_ preRoot)
    {
        node_type* rootNode = hash_array_.get_node(root);
        if(rootNode)
        {
            hash_function::hash<KeyType_> hash_func;
            std::size_t bucket = hash_func(rootNode->get_key()) % _Cap;
            node_type* minNode = hash_array_.get_node(buckets_[bucket].minson_);
            updatePreTree(minNode,preRoot);
        }
    }

    void updatePreTree(node_type* minNode,IndexType_ preRoot)
    {
        if(minNode)
        {
            minNode->set_left(preRoot);
            if(preRoot != 0)
            {
                minNode->set_color((RBTColor)(RB_MIN_NODE | minNode->get_color()));
            }else
            {
                minNode->set_color(tree_type::rbColorOf(minNode));
            }
        }
    }

    void updateNextTree(tree_type rb_tree,IndexType_ nextRoot)
    {
        node_type* newmax_node = rb_tree.maximum();
        if(newmax_node)
        {
            newmax_node->set_right(nextRoot);
            if(nextRoot != 0)
            {
                newmax_node->set_color((RBTColor)(RB_MAX_NODE | newmax_node->get_color()));
            }else
            {
                newmax_node->set_color(tree_type::rbColorOf(newmax_node));
            }
            hash_function::hash<KeyType_> hash_func;
            std::size_t bucket = hash_func(newmax_node->get_key()) % _Cap;
            buckets_[bucket].maxson_ = hash_array_.get_cur(newmax_node);
        }
    }

    void updateNextTree(IndexType_ root,IndexType_ nextRoot)
    {
        node_type* rootNode = hash_array_.get_node(root);
        if(rootNode)
        {
            hash_function::hash<KeyType_> hash_func;
            std::size_t bucket = hash_func(rootNode->get_key()) % _Cap;
            node_type* maxNode = hash_array_.get_node(buckets_[bucket].maxson_);
            updateNextTree(maxNode,nextRoot);
        }
    }

    void updateNextTree(node_type* maxNode,IndexType_ nextRoot)
    {
        if(maxNode)
        {
            maxNode->set_right(nextRoot);
            if(nextRoot != 0)
            {
                maxNode->set_color((RBTColor)(RB_MAX_NODE | maxNode->get_color()));
            }else
            {
                maxNode->set_color(tree_type::rbColorOf(maxNode));
            }
        }
    }
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
