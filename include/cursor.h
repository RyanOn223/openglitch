#ifndef CURSOR_H
#define CURSOR_H
#include "entity.h"
#include "texture_manager.h"
class cursor : public entity
{
    public:
        cursor(texture_manager& textures);//, resource_manager<sf::Font, fonts::ID>& fonts);
        virtual ~cursor();
        void draw_current(sf::RenderTarget& window, sf::RenderStates states) const;
    protected:

    private:
        sf::Sprite sprite;
        //text_node* display;
};

#endif // CURSOR_H
