#include "GUI_ActionManager.h"

GUI_ActionManager::GUI_ActionManager(int x, int y, int width, int height, ActionManager* ActionManager)
    : GUI_Scrollable_container(x, y, width, height)
{
    m_item = ActionManager;
    m_item->add_item_event += std::bind(&GUI_ActionManager::on_item_add, this, std::placeholders::_1);
    m_item->remove_item_event += std::bind(&GUI_ActionManager::on_item_remove, this, std::placeholders::_1);
}

GUI_ActionManager::~GUI_ActionManager(void)
{
}

void GUI_ActionManager::add_item_control(GUI_Object* object)
{
    static_cast<GUI_Item*>(object)->resize(m_size.w - 4 - 20, 21);
    static_cast<GUI_Item*>(object)->close += std::bind(&GUI_ActionManager::remove_item_from_source, this, std::placeholders::_1);
    GUI_Scrollable_container::add_item_control(object);
}

void GUI_ActionManager::on_item_add(tag_t const& e)
{
	const auto item = new GUI_Item(0, 0, 0, 0, e.task->m_action->get_description(e.task->m_parameter), e);
    add_item_control(item);
}

void GUI_ActionManager::remove_item_control(GUI_Object* object)
{
    for (auto current = m_items.begin(); current != m_items.end(); ++current) {
        if (static_cast<GUI_Item*>(*current) == object) {
	        auto lx = (*current)->m_position.x;
            auto ly = (*current)->m_position.y;
            if (!m_items.empty()) {
	            for (auto other = std::next(current, 1); other != m_items.end(); ++other) {
		            const auto cx = (*other)->m_position.x;
		            const auto cy = (*other)->m_position.y;
                    (*other)->m_position.x = lx;
                    (*other)->m_position.y = ly;
                    lx = cx;
                    ly = cy;
                }
            }
            GUI_Layer::remove(object);
            if (!m_items.empty()) {
	            const auto last_element = m_items.back();
                if (last_element->m_position.y + last_element->m_size.h + m_scroll.y + 2 < m_size.h) {
                    m_scroll.y += last_element->m_size.h;
                    if (m_scroll.y > 0) {
                        m_scroll.y = 0;
                    }
                }
                m_content_size.h = last_element->m_position.y + last_element->m_size.h + 2;
            } else {
                m_content_size.h = 0;
            }
            break;
        }
    }
    update();
    m_scrollbar.content_update();
}

void GUI_ActionManager::on_item_remove(tag_t const& e)
{
    for (auto& m_item : m_items)
    {
        if (static_cast<GUI_Item*>(m_item)->m_tag.task == e.task) {
            remove_item_control(m_item);
            break;
        }
    }
}

void GUI_ActionManager::remove_item_from_source(GUI_Object* object)
{
    m_item->remove(static_cast<GUI_Item*>(object)->m_tag.task);
}