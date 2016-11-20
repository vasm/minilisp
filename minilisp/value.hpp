#ifndef value_h
#define value_h

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <exception>


namespace value {
    
    enum class type_id {
        unknown,
        identifier,
        integer,
        real,
        str,
        list
    };

    class typeid_exception: public std::exception
    {
    public:
        typeid_exception(type_id t): _t(t)
        {}

        const char* what() const noexcept
        {
            std::stringstream ss;
            ss << "Type with id = " << (int)_t << " is unknown\n";
            return ss.str().c_str();
        }

        private:
            type_id _t;
    };

    std::string type_id_as_str(type_id);
    
    
    class gen_value {
    public:
        virtual ~gen_value() {}
        
        virtual type_id type() const {
            return type_id::unknown;
        }
    };
    
    
    typedef std::shared_ptr<gen_value> ptr;
    typedef std::unique_ptr<gen_value> uptr;
    
    
    /*
     * Common class for values that can only be set&read.
     * Profit: it is easily extensible!
     */
    template <typename T, type_id T_id>
    class simple_value: public gen_value {
    public:
        simple_value(const T& val): _val(val)
        {}
        
        type_id type() const {
            return T_id;
        }
        
        T get() const {
            return _val;
        }
        
        void set(const T& val) {
            _val = val;
        }
        
    protected:
        T _val;
    };
    
    
    typedef simple_value<long, type_id::integer> integer;
    typedef simple_value<double, type_id::real> real;
    typedef simple_value<std::string, type_id::str> str;
    typedef simple_value<std::vector<ptr>, type_id::list> list;


    ptr read(std::istream& is);
} // namespace value

#endif /* value_h */
