#ifndef __VARIANT_LIST_H__
#define __VARIANT_LIST_H__

#include "Variant.hpp"

#include <memory>
#include <iterator>
#include <stdexcept>

class VariantList {
public:
    struct Node {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        std::shared_ptr<VariantBase> data;
    };

    VariantList()
        : head(std::make_shared<Node>()), tail(std::make_shared<Node>()), len(0) {
        head->next = tail;
        tail->prev = head;
    }
    VariantList(const VariantList &rhs)
        : VariantList() {
        for (auto item : rhs) {
            auto newnode = std::make_shared<Node>();
            newnode->data = item->duplicate();
            insert_node(newnode, end());
        }
    }

    VariantList (VariantList &&rhs) {
        swap(rhs);
    }

    VariantList &operator=(const VariantList &rhs) {
        clear();
        for (auto item : rhs) {
            auto newnode = std::make_shared<Node>();
            newnode->data = item->duplicate();
            insert_node(newnode, end());
        }
        return *this;
    }

    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, std::shared_ptr<VariantBase>> {
    public:
        const_iterator(std::shared_ptr<Node> node)
            : node(node) {}
        const_iterator(const const_iterator &rhs)
            : node(rhs.node) {}
        const_iterator(const_iterator &&rhs)
            : node(std::move(rhs.node)) {}
        virtual ~const_iterator() {}
        const_iterator &operator=(const const_iterator &rhs) {
            node = rhs.node;
            return *this;
        }

        const_iterator &operator++() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->next;
            return *this;
        }
        const_iterator operator++(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            const_iterator tmp(*this);
            node = node.lock()->next;
            return tmp;
        }
        const_iterator &operator--() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->prev.lock();
            return *this;
        }
        const_iterator operator--(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            const_iterator tmp(*this);
            node = node.lock()->prev.lock();
            return tmp;
        }

        bool operator==(const const_iterator &rhs) const {
            return rhs.node.lock() == node.lock();
        }
        bool operator!=(const const_iterator &rhs) const {
            return !(rhs == *this);
        }

        bool expired() const {
            return node.expired();
        }

        std::shared_ptr<VariantBase> operator*() const {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            return node.lock()->data;
        }

        VariantBase *operator->() const {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            return node.lock()->data.get();
        }

        std::shared_ptr<Node> get() const {
            return node.lock();
        }

    protected:
        std::weak_ptr<Node> node;
    };

    class iterator : public const_iterator {
    public:
        iterator(std::shared_ptr<Node> node)
            : const_iterator(node) {}
        iterator(const iterator &rhs)
            : const_iterator(rhs) {}
        iterator(iterator &&rhs)
            : const_iterator(rhs) {}
        iterator &operator=(const iterator &rhs) {
            node = rhs.node;
            return *this;
        }
        iterator &operator++() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->next;
            return *this;
        }
        iterator operator++(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            iterator tmp(*this);
            node = node.lock()->next;
            return tmp;
        }
        iterator &operator--() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->prev.lock();
            return *this;
        }
        iterator operator--(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            iterator tmp(*this);
            node = node.lock()->prev.lock();
            return tmp;
        }
        std::shared_ptr<VariantBase> &operator*() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            return node.lock()->data;
        }
        VariantBase *operator->() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            return node.lock()->data.get();
        }
    };

    class const_reverse_iterator : public const_iterator {
    public:
        const_reverse_iterator(std::shared_ptr<Node> node)
            : const_iterator(node) {}
        const_reverse_iterator(const const_reverse_iterator &rhs)
            : const_iterator(rhs) {}
        const_reverse_iterator(const_reverse_iterator &&rhs)
            : const_iterator(rhs) {}
        const_reverse_iterator &operator=(const const_reverse_iterator &rhs) {
            node = rhs.node;
            return *this;
        }
        const_reverse_iterator &operator++() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->prev.lock();
            return *this;
        }
        const_reverse_iterator operator++(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            const_reverse_iterator tmp(*this);
            node = node.lock()->prev.lock();
            return tmp;
        }
        const_reverse_iterator &operator--() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->next;
            return *this;
        }
        const_reverse_iterator operator--(int) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            const_reverse_iterator tmp(*this);
            node = node.lock()->next;
            return tmp;
        }
    };

    class reverse_iterator : public iterator {
    public:
        reverse_iterator(std::shared_ptr<Node> node)
            : iterator(node) {}
        reverse_iterator(const reverse_iterator &rhs)
            : iterator(rhs) {}
        reverse_iterator(reverse_iterator &&rhs)
            : iterator(rhs) {}
        reverse_iterator &operator=(const reverse_iterator &rhs) {
            node = rhs.node;
            return *this;
        }
        reverse_iterator &operator++() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->prev.lock();
            return *this;
        }
        reverse_iterator operator++(int dummy) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            reverse_iterator tmp(*this);
            node = node.lock()->prev.lock();
            return tmp;
        }
        reverse_iterator &operator--() {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            node = node.lock()->next;
            return *this;
        }
        reverse_iterator operator--(int dummy) {
            if (expired()) {
                throw std::runtime_error("Iterator is expired");
            }
            reverse_iterator tmp(*this);
            node = node.lock()->next;
            return tmp;
        }
    };

    void clear() {
        while (size() > 0) {
            pop_front();
        }
    }

    template <typename ElementType>
    iterator insert(const ElementType &elem, const iterator &dest) {
        auto newnode = std::make_shared<Node>();
        newnode->data = std::make_shared<Variant<ElementType>>(elem);
        return insert_node(newnode, dest);
    }

    iterator erase(const iterator &dest) {
        return erase_node(dest);
    }

    template <typename ElementType>
    void push_back(const ElementType &elem) {
        insert(elem, end());
    }

    template <typename ElementType>
    void push_front(const ElementType &elem) {
        insert(elem, begin());
    }

    void pop_back() {
        if (begin() == end()) {
            throw std::underflow_error("VariantList is empty");
        }
        erase(rbegin());
    }

    void pop_front() {
        if (begin() == end()) {
            throw std::underflow_error("VariantList is empty");
        }
        erase(begin());
    }

    bool empty() const {
        return size() == 0;
    }

    size_t size() const {
        return len;
    }

    void swap(VariantList &rhs) {
        std::swap(head, rhs.head);
        std::swap(tail, rhs.tail);
        std::swap(len, rhs.len);
    }

    const_iterator begin() const {
        return const_iterator(head->next);
    }

    iterator begin() {
        return iterator(head->next);
    }

    const_iterator end() const {
        return const_iterator(tail);
    }

    iterator end() {
        return iterator(tail);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next);
    }

    const_iterator cend() const {
        return const_iterator(tail);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(tail->prev.lock());
    }

    reverse_iterator rbegin() {
        return reverse_iterator(tail->prev.lock());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(head);
    }

    reverse_iterator rend() {
        return reverse_iterator(head);
    }

    const_reverse_iterator crbegin() const {
        return const_reverse_iterator(tail->prev.lock());
    }

    const_reverse_iterator crend() const {
        return const_reverse_iterator(head);
    }

    friend std::ostream &operator<<(std::ostream &os, const VariantList &rhs) {
        os << "[";
        bool first = true;
        for (auto item : rhs) {
            if (first) first = false;
            else os << ", ";
            os << *item;
        }
        os << "]";
        return os;
    }

private:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    size_t len;

    iterator insert_node(std::shared_ptr<Node> node, const iterator &dest) {
        if (dest.expired()) {
            throw std::runtime_error("Iterator is expired");
        }
        auto nextnode = dest.get();
        node->next = nextnode;
        auto prevnode = nextnode->prev.lock();
        node->prev = prevnode;
        nextnode->prev = node;
        prevnode->next = node;

        ++ len;

        return iterator(node);
    }

    iterator erase_node(iterator cur) {
        if (cur.expired()) {
            throw std::runtime_error("Iterator is expired");
        }
        auto thisnode = cur.get();
        thisnode->next->prev = thisnode->prev.lock();
        thisnode->prev.lock()->next = thisnode->next;

        -- len;

        return ++ cur;
    }
};

void swap(VariantList &a, VariantList &b) {
    a.swap(b);
}

#endif
