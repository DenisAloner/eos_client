#include "inventory_cell.h"
#include "writer.h"

InventoryCell::InventoryCell(GameObject* item)
    : m_item(item)
{
    m_kind = entity_e::inventory_cell;
}

void InventoryCell::reset_serialization_index()
{
    m_serialization_index = 0;
    if (m_item) {
        // ??? исправить, временная заглушка, необходимо добавить свойство, которое показывает был ли сброшен объект для сохранения уже
        if (m_item->m_serialization_index != 0) {
            m_item->reset_serialization_index();
        }
    }
}

iPacker& InventoryCell::get_packer()
{
    return Packer<InventoryCell>::instance();
}

std::u16string InventoryCell::serialize_to_json_reference(JsonWriter& value)
{
    return value.write(*this);
}

std::u16string InventoryCell::serialize_to_json_pointer(JsonWriter& value)
{
    return value.write(this);
}