#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "scene_node.h"
#include "resource_manager.h"

class text_node : public scene_node
{
    public:
        explicit text_node(const resource_manager<sf::Font, fonts::ID>& fonts, const std::string& text);
        void set_string(const std::string& text);
        virtual ~text_node();

    protected:

    private:
        std::unique_ptr<sf::Text> text;
        virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif // TEXT_NODE_H
