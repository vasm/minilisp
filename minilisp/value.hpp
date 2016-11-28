#ifndef value_hpp
#define value_hpp

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <sstream>
#include <vector>


namespace value
{
enum class type_id {
    unknown,
    identifier,
    integer,
    real,
    str,
    list,
    builtin
};

std::string type_id_as_str(type_id);

/***
     * TODO: migrate from RTTI and all those dynamic_casts
     *       by creating metatype system
     *  See Qt documentation on QVariant
     */
class gen_value {
public:
    virtual ~gen_value() {}

    virtual type_id type() const {
        return type_id::unknown;
    }

    virtual void dump(std::ostream& /*os*/) {
    }
};


typedef std::shared_ptr<gen_value> ptr;
typedef std::unique_ptr<gen_value> uptr;

typedef std::vector<ptr> list_t;

template <typename T>
struct type_map {
    static const type_id id = type_id::unknown;
};

template <>
struct type_map<long> {
    static const type_id id = type_id::integer;
};

template <>
struct type_map<double> {
    static const type_id id = type_id::real;
};

template <>
struct type_map<std::string> {
    static const type_id id = type_id::str;
};

template <>
struct type_map<list_t> {
    static const type_id id = type_id::list;
};


/*
     * Common class for values that can only be set&read.
     * Profit: it is easily extensible!
     */
template <typename T>
class simple_value: public gen_value {
public:
    // we presume that T has a default ctor
    simple_value(): _val()
    {}

    simple_value(const T& val): _val(val)
    {}

    simple_value(const T&& val): _val(val)
    {}

    type_id type() const {
        return type_map<T>::id;
    }

    T get() const {
        return _val;
    }

    void set(const T& val) {
        _val = val;
    }

    void read(std::istream& is) {
        is >> _val;
    }

    void dump(std::ostream& os) {
        os << _val;
    }

protected:
    T _val;
};

typedef simple_value<long> integer;
typedef simple_value<double> real;
typedef simple_value<std::string> str;

class identifier: public gen_value
{
public:
    identifier(const std::string& s): _val(s)
    {}

    type_id type() const {
        return type_id::identifier;
    }

    const std::string& get() {
        return _val;
    }

    void dump(std::ostream &os) {
        os << "id: " << _val;
    }

protected:
    std::string _val;
};

class list: public gen_value
{
public:
    type_id type() const {
        return type_id::list;
    }

    void dump(std::ostream& os) {
        os << '[';

        for (auto it = _val.begin(); it != _val.end(); ++it) {
            (*it)->dump(os);
            if (std::next(it) != _val.end())
                os << ", ";
        }

        os << ']';
    }

    void push_back(ptr& p) {
        _val.push_back(p);
    }

protected:
    list_t _val;
};

class builtin: public gen_value
{
public:
    typedef std::function<ptr(ptr)> func;

    type_id type() const {
        return type_id::builtin;
    }

    void dump(std::ostream& os) {
        os << "Builtin func at 0x" << std::ios::hex << _f.target<func*>() << std::endl;
    }

private:
    builtin() = delete;
    builtin(const builtin&) = delete;
    func _f;
};

ptr read(std::istream& is);
} // namespace value

#endif /* value_h */
