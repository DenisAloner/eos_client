#ifndef PATTERN_VISITOR_H
#define PATTERN_VISITOR_H

#include <vector>

class iVisitor {
public:
    template <typename T>
    void call(std::vector<T>& ref)
    {
        for (auto& current : ref) {
            current.accept(*this);
        }
    }
};

class iVisitable {
public:
    virtual void accept(iVisitor& ref) = 0;
};


#endif //PATTERN_VISITOR_H
