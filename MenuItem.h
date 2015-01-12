#ifndef _MENU_ITEM_H_
#define _MENU_ITEM_H_

#include <string>
#include <boost/shared_ptr.hpp>
#include "Vector.h"

namespace typing
{
   class MenuItem
    {
    public:
        // Ctors/Dtors
        MenuItem(const juzutil::Vector2& origin, const std::string& text, unsigned int itemID, float height, bool selected)
            : m_origin(origin), m_text(text), m_id(itemID), m_height(height), m_selected(selected)
        {
        }

        // Methods
        static void Init();
        void Draw();

        void Select(bool select)
        {
                m_selected = select;
        }

        bool IsSelected() const
        {
            return m_selected;
        }

        unsigned int GetID() const
        {
            return m_id;
        }

    private:
        // Constants
        static const std::string FONT;
        static const float       BORDER_GAP_X;
        static const float       BORDER_GAP_Y;
        static const float       BORDER_LINE_LENGTH;

        // Members
        juzutil::Vector2 m_origin;
        std::string      m_text;
        unsigned int     m_id;
        float            m_height;
        bool             m_selected;
   };
   typedef boost::shared_ptr<MenuItem> MenuItemPtr;
}

#endif // _MENU_ITEM_H_
