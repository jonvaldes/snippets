#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

class Variant {
private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual std::string id() const = 0;
        virtual concept_t* copy() const = 0;
    };

    template <typename T> struct model : concept_t {
        model(T t) : _elem(std::move(t)) {}

        std::string id() const override {
            // Totally non-portable, but it's just for debugging purposes
            std::string name(__PRETTY_FUNCTION__);
            name.erase(name.begin(), name.begin() + name.find_first_of("<")+1);
            name.erase(name.begin() + name.find_first_of(">"), name.end());
            return name;
        }
        concept_t* copy() const override { return new model<T>(_elem); }
        T _elem;
    };

    template <typename T> const model<T>* get_model_as() const {
        return dynamic_cast<const model<T>*>(_data.get());
    }

    template <typename T> model<T>* get_model_as() { return dynamic_cast<model<T>*>(_data.get()); }

    std::unique_ptr<concept_t> _data;

public:
    Variant() : _data(nullptr) {}
    template <typename T> Variant(T t) {
        concept_t* c = new model<T>(t);
        _data.reset(c);
    }

    Variant(Variant&& other) {
        if(!other._data) {
            asm("int3");
        }
        _data = std::move(other._data);
    }

    Variant(const Variant& other) {
        if(!other._data) {
            asm("int3");
        }
        _data.reset(other._data->copy());
    }

    Variant& operator=(const Variant& other) {
        _data.reset(other._data->copy());
        return *this;
    }

    template <typename T> bool is() const { return bool(get_model_as<T>()); }
    const std::string id() const {
        if(!_data)
            asm("int3");
        return _data->id();
    }
    template <typename T> const T& get() const {
        const model<T>* m = get_model_as<T>();
        if(!m) {
            asm("int3"); // crash to debugger
        }
        return m->_elem;
    }
    template <typename T> T& get() {
        model<T>* m = get_model_as<T>();
        if(!m) {
            asm("int3"); // crash to debugger
        }
        return m->_elem;
    }
};

using VariantVec = std::vector<Variant>;
using VariantMap = std::map<std::string, Variant>;

