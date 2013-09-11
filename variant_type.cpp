#include <stdio.h>
#include <memory>
#include <vector>
#include <map>
#include <string>

class Variant {
private:
    struct concept_t {
        virtual ~concept_t() = default;
        virtual std::string id() const = 0;
    };

    template <typename T> struct model : concept_t {
        model(T t) : _data(std::move(t)) {}
        std::string id() const override {
            // Totally non-portable, but...
            const char* name = __PRETTY_FUNCTION__;
            while(*name != '<') {
                name++;
            }
            name++;
            int cnt = 0;
            while(*(name + cnt) != '>') {
                cnt++;
            }
            return std::string(name, cnt);
        }
        T _data;
    };

    template <typename T> const model<T>* get_model_as() const {
        return dynamic_cast<const model<T>*>(_data.get());
    }

    std::shared_ptr<const concept_t> _data;

public:
    Variant() : _data(nullptr) {}
    template <typename T> Variant(T t) : _data(std::make_shared<model<T>>(t)) {}
    template <typename T> bool is() const { return bool(get_model_as<T>()); }
    const std::string id() const { return _data->id(); }
    template <typename T> const T& get() const {
        const model<T>* m = get_model_as<T>();
        if(!m) {
            asm("int3"); // crash to debugger
        }
        return m->_data;
    }
};

using VariantVec = std::vector<Variant>;
using VariantMap = std::map<std::string, Variant>;

int main() {

    VariantMap obj;
    obj["width"] = 800;
    obj["height"] = 600;
    obj["name"] = "Test app";
    obj["timestep"] = 1.0 / 60;

    for(auto c : obj) {
        printf("Name: %s, ID: %s, ", c.first.c_str(), c.second.id().c_str());
        if(c.second.is<int>()) {
            printf("%d\n", c.second.get<int>());
        } else if(c.second.is<double>()) {
            printf("%f\n", c.second.get<double>());
        } else if(c.second.is<const char*>()) {
            printf("\"%s\"\n", c.second.get<const char*>());
        }
    }
    printf("\nName property:%s\n", obj["name"].get<const char*>());
    printf("ID:%s\n", obj["name"].id().c_str());
}
