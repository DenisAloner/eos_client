#ifndef VISITORS_H
#define	VISITORS_H

class Attribute_map;
class Object_state;
class GameObject;
class Interaction_list;
class Parameter_list;
class Vision_list;
class Vision_component;
class Parts_list;
class Object_part;
class Tag_list;


namespace ObjectTag
{
	class Label;
}

class Visitor_generic
{
public:

	virtual void visit(GameObject& value) {};
	virtual void visit(Attribute_map& value) {};
	virtual void visit(Object_state& value) {};
	virtual void visit(Interaction_list& value) {};
	virtual void visit(Parameter_list& value) {};
	virtual void visit(Tag_list& value) {};
	virtual void visit(Vision_list& value) {};
	virtual void visit(Vision_component& value) {};
	virtual void visit(Parts_list& value) {};
	virtual void visit(Object_part& value) {};
	virtual void visit(ObjectTag::Label& value) {};

};

class Visitor_simple :public Visitor_generic
{
public:

	void visit(GameObject& value) override;
	void visit(Attribute_map& value) override;
	void visit(Object_state& value) override;
	void visit(Interaction_list& value) override;
	void visit(Parameter_list& value) override;
	void visit(Vision_list& value) override;
	void visit(Vision_component& value) override;
	void visit(Parts_list& value) override;
	void visit(Object_part& value) override;
	void visit(Tag_list& value) override;

};

class Visitor_doublehandle :public Visitor_simple
{
public:

	void visit(GameObject& value) override;
	void visit(Attribute_map& value) override;
	void visit(Object_state& value) override;
	void visit(Interaction_list& value) override;
	void visit(Parameter_list& value) override;
	void visit(Vision_list& value) override;
	void visit(Vision_component& value) override;
	void visit(Parts_list& value) override;
	void visit(Object_part& value) override;
	void visit(Tag_list& value) override;

	virtual void begin(GameObject& value);
	virtual void end(GameObject& value);
	virtual void begin(Attribute_map& value);
	virtual void end(Attribute_map& value);
	virtual void begin(Object_state& value);
	virtual void end(Object_state& value);
	virtual void begin(Interaction_list& value);
	virtual void end(Interaction_list&value);
	virtual void begin(Parameter_list& value);
	virtual void end(Parameter_list& value);
	virtual void begin(Vision_list& value);
	virtual void end(Vision_list& value);
	virtual void begin(Vision_component& value);
	virtual void end(Vision_component& value);
	virtual void begin(Object_part& value);
	virtual void end(Object_part& value);
	virtual void begin(Parts_list& value);
	virtual void end(Parts_list& value);
	virtual void begin(Tag_list& value);
	virtual void end(Tag_list& value);

};

#endif VISITORS_H