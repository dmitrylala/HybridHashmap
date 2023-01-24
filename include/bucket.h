#pragma once

#include <list>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>
#include <range/v3/view/concat.hpp>
#include <range/v3/algorithm/contains.hpp>


// constants
static constexpr auto DEFAULT_CACHE_SIZE = 10;


template<
    typename Key,
    typename T
> class Bucket {
public:
    // types
    using key_type               = Key;
    using const_key_reference    = const key_type&;
    using mapped_type            = T;
    using value_type             = std::pair<key_type, mapped_type>;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using size_type              = std::size_t;

    // construct/copy/destroy
    constexpr Bucket(size_type cache_size = DEFAULT_CACHE_SIZE) noexcept : m_cache(), m_list() {
        m_cache.reserve(cache_size);
    }

    constexpr Bucket(const Bucket &other) noexcept {
        copy_data(other);
    }

    constexpr Bucket &operator=(const Bucket &other) {
        if (this == &other)
            return *this;

        copy_data(other);

        return *this;
    }

    // getters and data access
    constexpr bool empty() const noexcept { return size() == 0; }
    constexpr size_type size() const noexcept { return m_cache.size() + m_list.size(); }
    constexpr size_type capacity() const noexcept { return m_cache.capacity(); }
    auto data() const { return ranges::views::concat(m_cache, m_list); }
    auto cache() const { return m_cache; }
    auto list() const { return m_list; }
    auto content() { return ranges::views::concat(m_cache, m_list); }

    // modifiers
    constexpr void add(const_reference key_val) {
        if (contains(key_val.first))
            return;
            
        if (!is_cache_full()) {
            m_cache.push_back(key_val);
            return;
        }

        m_list.push_back(key_val);
    }

    constexpr void remove(const_key_reference key) {
        auto cache_it = cache_get_by_key(key);
        if (cache_it != m_cache.end()) {
            m_cache.erase(cache_it);
            align_cache();
            return;
        }

        auto list_it = list_get_by_key(key);
        if (list_it != m_list.end())
            m_list.erase(list_it);
    }

    // other
    bool contains(const_key_reference key) const noexcept { return cache_contains(key) || list_contains(key); }

    constexpr void clear() noexcept {
        m_cache.clear();
        m_list.clear();
    }
private:
    std::vector<value_type> m_cache;
    std::list<value_type> m_list;

    void copy_data(const Bucket& other) {
        m_cache = other.m_cache;
        m_list = other.m_list;
        m_cache.reserve(other.capacity());
    }

    size_type count_free_cache_space() const { return m_cache.capacity() - m_cache.size(); }
    bool is_cache_full() const { return count_free_cache_space() == 0; }

    void align_cache() {
        if (is_cache_full())
            return;

        size_type free_space = count_free_cache_space();
        size_type n_elems_not_in_cache = m_list.size();
        size_type n_elems_to_move = std::min(free_space, n_elems_not_in_cache);

        for (size_type i = 0; i < n_elems_to_move; ++i) {
            m_cache.push_back(m_list.front());
            m_list.pop_front();
        }
    }

    auto cache_get_by_key(const_key_reference key) const {
        for (auto t = m_cache.begin(); t != m_cache.end(); ++t)
            if (t->first == key)
                return t;
        return m_cache.end();
    }

    auto list_get_by_key(const_key_reference key) const {
        for (auto t = m_list.begin(); t != m_list.end(); ++t)
            if (t->first == key)
                return t;
        return m_list.end();
    }

    bool cache_contains(const_key_reference key) const {
        return cache_get_by_key(key) != m_cache.end();
    }

    bool list_contains(const_key_reference key) const {
        return list_get_by_key(key) != m_list.end();
    }
};
