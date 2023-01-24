#pragma once

#include <utility>
#include <stdexcept>
#include <vector>

#include "bucket.h"

// constants
static constexpr auto MAX_BUCKETS = 10;
static constexpr auto NO_KEY_MSG = "No such key in hashtable";


template<
    typename Key,
    typename T,
    typename Hash = std::hash<Key>
> class Hashmap {
public:
    // types
    using key_type               = Key;
    using const_key_reference    = const key_type&;
    using mapped_type            = T;
    using value_type             = std::pair<Key, T>;
    using size_type              = std::size_t;
    using hasher                 = Hash;

    using reference              = value_type&;
    using const_reference        = const value_type&;


    // construct/copy/destroy
    constexpr Hashmap(size_type bucket_count = MAX_BUCKETS,
                        const hasher &hash = hasher()) noexcept :
                        m_hash(hash) {
        for (int i = 0; i < bucket_count; ++i)
            m_buckets.push_back(Bucket<key_type, mapped_type>());
    }

    constexpr Hashmap(const Hashmap &other) noexcept {
        copy_data(other);
    }

    constexpr Hashmap &operator=(const Hashmap &other) {
        if (this == &other)
            return *this;

        copy_data(other);

        return *this;
    }

    // capacity and data access
    constexpr bool empty() const noexcept {
        return size() == 0;
    }

    constexpr size_type size() const noexcept {
        size_type total_size = 0;
        for (auto &bucket : m_buckets)
            total_size += bucket.size();
        return total_size;
    }

    // modifiers
    constexpr void add(const_reference key_val) {
        int idx = idx_by_key(key_val.first);
        m_buckets[idx].add(key_val);
    }

    constexpr bool contains(const_key_reference key) const {
        int idx = idx_by_key(key);
        auto bucket = m_buckets[idx];
        return bucket.contains(key);
    }

    mapped_type operator[](const_key_reference key) {
        int idx = idx_by_key(key);
        auto elements = m_buckets[idx].content();
        for (auto elem = elements.begin(); elem != elements.end(); ++elem)
            if (elem->first == key)
                return elem->second;
        throw std::runtime_error(NO_KEY_MSG);
    }

    constexpr mapped_type operator[](const_key_reference key) const {
        int idx = idx_by_key(key);
        auto elements = m_buckets[idx].data();
        for (auto elem = elements.begin(); elem != elements.end(); ++elem)
            if (elem->first == key)
                return elem->second;
        throw std::runtime_error(NO_KEY_MSG);
    }

    constexpr void remove(const_key_reference key) {
        int idx = idx_by_key(key);
        m_buckets[idx].remove(key);
    }

    constexpr void clear() noexcept {
        for (auto &bucket : m_buckets)
            bucket.clear();
    }

    auto data() const {
        return m_buckets;
    }
private:
    hasher m_hash;
    std::vector< Bucket<key_type, mapped_type> > m_buckets;

    void copy_data(const Hashmap &other) {
        m_hash = other.m_hash;
        m_buckets = other.m_buckets;
    }

    int idx_by_key(const key_type &key) const {
        return m_hash(key) % m_buckets.size();
    }
};
