#ifndef WALL_H
#define WALL_H
#include "entity.h"
#include "texture_manager.h"
class wall : public entity
{
    public:
    enum type
    {
        wall1,
    };
        wall(type wtype, const texture_manager& textures);
        virtual void update_current(sf::Time delta, command_queue& cmds);
        virtual void draw_current(sf::RenderTarget& target,
								  sf::RenderStates  states) const;
        virtual unsigned int get_category() const;
        virtual ~wall();
        virtual sf::FloatRect getBoundingRect() const;
        const sf::Sprite& get_sprite() const;
    protected:

    private:
    sf::Sprite sprite;
    type wall_type;
};

#endif // WALL_H
