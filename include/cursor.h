#ifndef CURSOR_H
#define CURSOR_H
#include "entity.h"
#include "texture_manager.h"
class cursor : public entity
{
    public:
        cursor(texture_manager& textures);
        virtual ~cursor();
        void draw_current(sf::RenderTarget& window, sf::RenderStates states) const;
        virtual sf::FloatRect getBoundingRect() const;
    protected:

    private:
        sf::Sprite sprite;
};

#endif // CURSOR_H
