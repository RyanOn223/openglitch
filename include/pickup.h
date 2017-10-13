#ifndef PICKUP_H
#define PICKUP_H
#include "include.h"
#include "monster.h"
#include "datatables.h"
class pickup : public entity
{
    public:
        enum type
        {
            sm_health_pack,
            sm_ammo,
            small_pistol,
            type_count,
        };
        pickup(pickup::type Type, const texture_manager& textures);
        virtual unsigned int get_category() const;
        virtual sf::FloatRect getBoundingRect() const;
        void apply(monster& the_player) const;
        virtual ~pickup();
        void enable_outline();
        void disable_outline();
    protected:
        virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
        pickup::type mtype;
        sf::Sprite sprite;
        bool draw_outline;
};

#endif // PICKUP_H
