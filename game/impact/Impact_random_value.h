#ifndef IMPACT_RANDOM_VALUE_H
#define IMPACT_RANDOM_VALUE_H

#include "effect.h"

class Impact_random_value : public Effect {
public:
    int m_min_value;

    Impact_random_value();
    ~Impact_random_value();

    Effect* clone() override;
};

#endif // IMPACT_RANDOM_VALUE_H
