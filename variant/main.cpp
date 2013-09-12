#include <stdio.h>
#include <sstream>
#include "variant.h"

std::string VariantToJSON(const Variant& v) {
    std::ostringstream ss;
    if(v.is<int>()) {
        ss << v.get<int>();
    } else if(v.is<double>()) {
        ss << v.get<double>();
    } else if(v.is<float>()) {
        ss << v.get<float>();
    } else if(v.is<const char*>()) {
        ss << "\"" << v.get<const char*>() << "\"";
    } else if(v.is<std::nullptr_t>()) {
        ss << "null";
    } else if(v.is<VariantVec>()) {
        ss << "[";
        const VariantVec& vec = v.get<VariantVec>();
        for(int i = 0; i < vec.size(); ++i) {
            if(i) {
                ss << ",";
            }
            ss << VariantToJSON(vec[i]);
        }
        ss << "]";
    } else if(v.is<VariantMap>()) {
        ss << "{";
        const VariantMap& m = v.get<VariantMap>();
        for(auto it = begin(m); it != end(m); ++it) {
            ss << ((it == begin(m)) ? "" : ",");
            ss << "\"" << it->first << "\" : ";
            ss << VariantToJSON(it->second);
        }
        ss << "}";
    }
    return ss.str();
}

int main() {

    Variant variantObj((VariantMap()));
    VariantMap& obj = variantObj.get<VariantMap>();
    obj["width"] = 800;
    obj["height"] = 600;
    obj["name"] = "Test app";
    obj["timestep"] = 1.0 / 60;

    VariantVec vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back("Hello");
    obj["vec"] = vec;

    VariantMap obj2;
    obj2["test"] = "Mooooooo!";
    obj2["test2"] = 12334.1f;

    obj["obj"] = obj2;

    Variant obj3(obj);
    obj["obj3"] = obj3;
    obj["obj4"] = obj["obj3"];
    obj["obj3"].get<VariantMap>()["width"] = 300;
    obj["width"] = 100;
    obj["ptr"] = nullptr;

    printf("%s\n", VariantToJSON(variantObj).c_str());
}
