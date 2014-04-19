#ifndef VARIANT_LIST_H_
#define VARIANT_LIST_H_

#include <memory>
#include <iterator>
#include <stdexcept>
#include <iostream>
#include <sstream>

#define DEFINE_GETTER(type, name, member) \
    type name() const { return member; }

class VariantListElementBase {
public:
    VariantListElementBase() {}
    virtual ~VariantListElementBase() {}

    virtual std::string to_string() const {return "";};

    virtual std::ostream& write_to_stream(std::ostream& os) const {return os;}

    friend std::ostream& operator<<(std::ostream& os, const VariantListElementBase& elem) {
        return elem.write_to_stream(os);
    }

    virtual std::shared_ptr<VariantListElementBase> duplicate() const {
        return std::make_shared<VariantListElementBase>();
    }
};

template <typename ElementType>
class VariantListElement : public VariantListElementBase {
public:
    VariantListElement(const ElementType &elem) : elem(elem) {}
    VariantListElement(const VariantListElement &rhs) : elem(rhs.elem) {}
    virtual ~VariantListElement() {}
    ElementType get() const {
        return elem;
    }

    virtual std::string to_string() const {
        std::stringstream buf;
        buf << get();
        return buf.str();
    }
    
    virtual std::ostream& write_to_stream(std::ostream& os) const {
        os << elem;
        return os;
    }

    virtual std::shared_ptr<VariantListElementBase> duplicate() const {
        auto newelem = new VariantListElement<ElementType>(*this);
        return std::shared_ptr<VariantListElementBase>(newelem);
    }

private:
    ElementType elem;
};

class VariantList {
public:
    VariantList(): len(0) {
        head = std::make_shared<Node>();
        tail = head;
    }
    VariantList(const VariantList &rhs): len(0) {
        head = std::make_shared<Node>();
        tail = head;
        for (auto node : rhs) {
            auto newnode = std::make_shared<VariantList::Node>();
            newnode->elem = node->elem->duplicate();
            insert_node(newnode, end());
        }
    }
    VariantList& operator=(const VariantList& rhs) {
        clear();
        for (auto node : rhs) {
            auto newnode = std::make_shared<VariantList::Node>();
            newnode->elem = node->elem->duplicate();
            insert_node(newnode, end());
        }
        return *this;
    }
    ~VariantList() {
        clear();
    }

    struct Node {
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
        std::shared_ptr<VariantListElementBase> elem;

        template <typename ElementType>
        ElementType get() const {
            return static_cast<VariantListElement<ElementType> *>(elem.get())->get();
        }
    };

    DEFINE_GETTER(size_t, size, len);

    class iterator : std::iterator<std::bidirectional_iterator_tag, VariantList::Node> {
    public:
        iterator() {}
        iterator(const std::shared_ptr<VariantList::Node> n): node(n) {}
        iterator(const iterator& itr): node(itr.node) {}
        ~iterator() {}

        iterator& operator++() {
            node = node->next;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp(*this);
            node = node->next;
            return tmp;
        }
        iterator& operator--() {
            node = node->prev.lock();
            return *this;
        }
        iterator operator--(int) {
            iterator tmp(*this);
            node = node->prev.lock();
            return tmp;
        }

        bool operator==(const iterator& rhs) const {
            return rhs.node == node;
        }
        bool operator!=(const iterator& rhs) const {
            return !(rhs == *this);
        }
        std::shared_ptr<VariantList::Node> operator*() {
            return node;
        }

    private:
        std::shared_ptr<VariantList::Node> node;
    };

    void clear() {
        while (size() > 0) {
            pop_front();
        }
    }

    VariantList::iterator begin() const {
        return VariantList::iterator(head);
    }

    VariantList::iterator begin() {
        return VariantList::iterator(head);
    }

    VariantList::iterator end() const {
        return VariantList::iterator(tail.lock());
    }

    VariantList::iterator end() {
        return VariantList::iterator(tail.lock());
    }

    template <typename ElementType>
    VariantList::iterator insert(const ElementType& elem, iterator itr) {
        auto newnode = std::make_shared<VariantList::Node>();
        VariantListElementBase *elem_ptr = new VariantListElement<ElementType>(elem);
        newnode->elem = std::shared_ptr<VariantListElementBase>(elem_ptr);

        return insert_node(newnode, itr);
    }


    VariantList::iterator erase(iterator itr) {
        if (size() == 0) {
            throw std::underflow_error("List size is zero");
        } else if (itr == end()) {
            throw std::overflow_error("Cannot erase end()");
        }

        if (*(itr) == head) {
            head = head->next;
        } else {
            if (!(*itr)->prev.expired()) {
                (*itr)->prev.lock()->next = (*itr)->next;
            }
            (*itr)->next->prev = (*itr)->prev.lock();
        }

        -- len;
        return ++ itr;
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
        erase(-- end());
    }

    void pop_front() {
        erase(begin());
    }

    std::string to_string() const {
        std::stringstream buf;
        buf << "[";
        bool is_first = true;
        for (auto node : *this) {
            if (is_first) is_first = false;
            else buf << ", ";
            buf << *node->elem;
        }
        buf << "]";
        return buf.str();

    }

    friend std::ostream& operator<<(std::ostream& buf, const VariantList& lst) {
        buf << "[";
        bool is_first = true;
        using std::cout;
        using std::endl;
        for (auto node : lst) {
            if (is_first) is_first = false;
            else buf << ", ";
            buf << *node->elem;
        }
        buf << "]";
        return buf;
    }

private:
    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t len;

    VariantList::iterator insert_node(std::shared_ptr<VariantList::Node> newnode, VariantList::iterator itr) {
        newnode->next = (*itr);
        if (!(*itr)->prev.expired()) {
            newnode->prev = (*itr)->prev;
            (*itr)->prev.lock()->next = newnode;
        }
        (*itr)->prev = newnode;

        if (head == (*itr)) {
            head = newnode;
        }

        ++ len;

        return VariantList::iterator(newnode);
    }
};

#endif
