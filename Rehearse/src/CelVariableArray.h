#ifndef INC_CELVARIABLEARRAY_H
#define INC_CELVARIABLEARRAY_H

#include <stdexcept>
#include <string>
#include <vector>
#include <cstdarg>

namespace rehearse {


template<class T>
class CelVariableArray {
private:
    std::string name;
    std::vector<T> list;
    std::vector<size_t> dimensions;
    std::vector<size_t> dimension_factors;

    // meta info for index offsets
    int current_dimension_index;
    size_t current_offset;

public:
    CelVariableArray(){
        current_dimension_index = 0;
        current_offset = 0;
    }

    CelVariableArray(std::string &name) : name(name) {
        current_dimension_index = 0;
        current_offset = 0;
    }

    CelVariableArray(const char *namestr) : name(namestr) {
        current_dimension_index = 0;
        current_offset = 0;
    }

    virtual ~CelVariableArray(){
    }

    std::string &getName(){
        return name;
    }


    CelVariableArray& operator[](size_t index){
        if (index >= dimensions[current_dimension_index]){
            throw std::out_of_range("BAD SUBSCRIPT ERROR"); // commodore64 lover
        }

        current_offset += index * dimension_factors[current_dimension_index];
        current_dimension_index++;
        return *this;
    }

    operator T& () {
        if (current_dimension_index != dimensions.size()){
            throw std::out_of_range("Wrong number of array dereferencing");
        }

        size_t ret_current_offset = current_offset;

        current_dimension_index = 0;
        current_offset = 0;

        return list[ret_current_offset];
    }

    void multiDimensionResize(int number_of_dimensions, ...){
        dimensions.clear();
        dimension_factors.clear();
        dimension_factors.resize(number_of_dimensions);

        va_list arguments;
        va_start(arguments, number_of_dimensions);

        size_t multiplier = 1;

        for (int i=0; i<number_of_dimensions; i++){
            size_t size = va_arg(arguments, int);
            dimensions.push_back(size);
            dimension_factors[i] = multiplier;
            multiplier *= size;
        }

        va_end(arguments);
        list.resize(multiplier, T(name));
    }

    void resize(size_t size){
        multiDimensionResize(1, size);
    }

};

}   // namespace rehearse

#endif
