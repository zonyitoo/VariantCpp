#ifndef __VARIANT_H__
#define __VARIANT_H__

#include <memory>
#include <utility>
#include <string>
#include <sstream>

template <typename ElementType>
class Variant;

class VariantBase {
public:
    virtual ~VariantBase() {}

    template <typename ElementType>
    ElementType get() const {
        return static_cast<Variant<ElementType> *>(this)->get();
    }

    template <typename ElementType>
    ElementType &get() {
        return static_cast<Variant<ElementType> *>(this)->get();
    }

    virtual std::shared_ptr<VariantBase> duplicate() = 0;
    virtual std::string string() const {
        std::stringstream buf;
        write_stream(buf);
        return buf.str();
    }
    virtual std::ostream &write_stream(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const VariantBase &rhs) {
        return rhs.write_stream(os);
    }
};

template <typename ElementType>
class Variant : public VariantBase {
public:
    Variant(const ElementType &elem)
        : elem(elem) {}
    Variant(const Variant<ElementType> &rhs)
        : elem(rhs.elem) {}
    Variant(Variant<ElementType> &&rhs)
        : elem(std::move(rhs.elem)) {}
    virtual ~Variant() {}

    Variant &operator=(const Variant<ElementType> &rhs) {
        elem = rhs.elem;
        return *this;
    }

    ElementType get() const {
        return elem;
    }

    ElementType &get() {
        return elem;
    }

    virtual std::shared_ptr<VariantBase> duplicate() {
        VariantBase *dup = new Variant(*this);
        return std::shared_ptr<VariantBase>(dup);
    }

    virtual std::ostream &write_stream(std::ostream &os) const {
        os << elem;
        return os;
    }

private:
    ElementType elem;
};

template <typename ElementType>
std::shared_ptr<VariantBase> make_variant(const ElementType &elem) {
    VariantBase *variant = new Variant<ElementType>(elem);
    return std::shared_ptr<VariantBase>(variant);
}

#endif
