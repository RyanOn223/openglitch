#ifndef WORLD_H
#define WORLD_H

#include "include.h"
#include "texture_manager.h"
#include "resource_manager.h"
#include "scene_node.h"
#include "entity.h"
#include "monster.h"
#include "sprite_node.h"
#include "player.h"
#include "command_queue.h"
#include "cursor.h"
#include "projectile.h"
#include "pickup.h"
#include "wall.h"
#include "collision_manager.h"
class world : public sf::NonCopyable
{
    public:
        template <typename GameObject, typename Function>
        std::function<void(scene_node&, sf::Time)>
        derived_action(Function fn)
        {
            return [=] (scene_node& node, sf::Time dt)
            {
                // Check if cast is safe
                assert(dynamic_cast<GameObject*>(&node) != nullptr);
                // Downcast node and invoke function on it
                fn(static_cast<GameObject&>(node), dt);
            };
        }
        explicit world(sf::RenderWindow& window);
        virtual ~world();
        void update(sf::Time delta);
        void draw();
        command_queue& get_cmd_queue();
        //player& get_player();
        void set_zoom(float f);
        float get_zoom();
        class spawn_point
        {
            public:
                spawn_point(monster::type stype, float x, float y);
                monster::type stype;
                float x;
                float y;
        };
        cursor* get_cursor();
        private:
        enum scn_layer
        {
            bg_layer,
            air_layer,
            walls_layer,
            mon_layer,
            hud_layer,
            layer_count,
        };
    private:
        void load_textures();
        void build_scene();
        void adjust_player_v();
        void rotate_player();
        void update_cursor();
        void spawn_enemies();
        void add_enemy(monster::type, float x, float y);
        void add_enemies();
        bool matches_categories(scene_node::scn_pair& colliders, cmd_category::ID type_1, cmd_category::ID type_2);
        void destroy_OOB_entities();
        void load_walls();
    private:
         collision_manager cmanager;
         sf::RenderWindow& wwindow;
         sf::View world_view;
         texture_manager textures;
         resource_manager<sf::Font, fonts::ID> fonts;
         resource_manager<sf::Sound, sounds::ID> sounds;
         scene_node scene_graph;
         std::array<scene_node*, layer_count> scene_layers;
         sf::FloatRect world_bounds;
         sf::Vector2f spawn_position;
         float zoom_level;
         monster* the_player;
         cursor* the_cursor;
         //float player_speed;
         command_queue world_cmd_queue;
         std::vector<spawn_point> enemy_spawn_points;
};

#endif // WORLD_H
