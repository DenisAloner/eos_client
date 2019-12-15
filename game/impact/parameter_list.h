#ifndef PARAMETER_LIST_H
#define PARAMETER_LIST_H

#include "interaction_list.h"

class Parameter_list : public Interaction_list {
public:
    interaction_e m_subtype;
    int m_basic_value;
    int m_basic_limit;

    int m_value;
    int m_limit;

    Parameter_list(interaction_e subtype);
    Parameter_list();
    bool update() override;
    Parameter_list* clone() override;
    void apply_visitor(Visitor_generic& visitor) override;

    iPacker& get_packer() override;

    constexpr static auto properties()
    {
        return make_union(
            Interaction_list::properties(),
            std::make_tuple(
                make_property(&Parameter_list::m_subtype, u"subtype"),
                make_property(&Parameter_list::m_basic_value, u"basic_value"),
                make_property(&Parameter_list::m_basic_limit, u"basic_limit")));
    }

    IJSONSERIALIZABLE();

private:
    class Update_visitor : public Visitor {
    public:
        Parameter_list& m_owner;

        Update_visitor(Parameter_list& owner);
        void visit(Object_interaction& value) override;
    };
};

#endif //PARAMETER_LIST_H