#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

class Resource
{
    public:
        Resource(int id, vector<int>* record)
            : id_(id)
            , record_(record)
        {
        }

        int id() const
        {
            return id_;
        }

        ~Resource()
        {
            if (record_ != nullptr)
            {
                record_->push_back(id_);
            }
        }

    private:
        int          id_;
        vector<int>* record_;
};

class ResourceStore
{
    public:
        ResourceStore(vector<int>* record)
            : record_(record)
        {
        }

        void add(int id)
        {
            items_.push_back(make_unique<Resource>(id, record_));
        }

        void remove(int id)
        {
            for (size_t i = 0; i < items_.size(); ++i)
            {
                if (items_[i]->id() == id)
                {
                    items_.erase(items_.begin() + i);
                    return;
                }
            }
        }

        ~ResourceStore()
        {
            for (size_t i = 0; i < items_.size(); ++i)
            {
                items_[i].reset();
            }
        }

    private:
        vector<unique_ptr<Resource>> items_;
        vector<int>*                 record_;
};
