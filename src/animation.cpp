#include "animation.h"
#include "datatables.h"
namespace
{
    std::vector<animation_data> table = init_animation_data();
}
animation::animation(animation::type Type) :
    sprite(),
    frame_size(table[Type].frame_size),
    num_frames(table[Type].num_frames),
    current_frame(0),
    duration(table[Type].duration),
    elapsed_time(sf::Time::Zero),
    repeat(table[Type].repeat),
    atype(Type)
{

}
animation::animation(const texture_manager& textures, animation::type Type) :
    sprite(textures.get(table[Type].texture), table[Type].texture_rect),
    frame_size(table[Type].frame_size),
    num_frames(table[Type].num_frames),
    current_frame(0),
    duration(table[Type].duration),
    elapsed_time(sf::Time::Zero),
    repeat(table[Type].repeat),
    atype(Type)
{
    setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
}
void animation::set_texture(const sf::Texture& tex)
{
    sprite.setTexture(tex);
}
const sf::Texture* animation::get_texture() const
{
    return sprite.getTexture();
}
void animation::set_frame_size(sf::Vector2i size)
{
    frame_size = size;
}
sf::Vector2i animation::get_frame_size() const
{
    return frame_size;
}
void animation::set_num_frames(std::size_t num)
{
    num_frames = num;
}
std::size_t animation::get_num_frames() const
{
    return num_frames;
}
void animation::set_duration(sf::Time dur)
{
    duration = dur;
}
sf::Time animation::get_duration() const
{
    return duration;
}
void animation::set_repeat(bool r)
{
    repeat = r;
}
bool animation::is_repeat() const
{
    return repeat;
}
void animation::restart()
{
    current_frame = 0;
}
bool animation::is_finished() const
{
    return current_frame >= num_frames;
}
sf::FloatRect animation::getLocalBounds() const
{
    return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(get_frame_size()));
}
sf::FloatRect animation::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}
void animation::update(sf::Time delta)
{
    sf::Time time_per_frame = duration / static_cast<float>(num_frames);
    elapsed_time += delta;
    sf::Vector2i texture_bounds(sprite.getTexture()->getSize());
    sf::IntRect texture_rect = sprite.getTextureRect();

    if (current_frame == 0)
    {
        texture_rect = sf::IntRect(0, 0, frame_size.x, frame_size.y);
    }
    //while there is a frame to process
    while(elapsed_time >= time_per_frame && (current_frame <= num_frames || repeat))
    {
        //move texture_rect to the left
        texture_rect.left += texture_rect.width;
        //if we've hit the right side of the texture
        if (texture_rect.left + texture_rect.width > texture_bounds.x)
        {
            //move it down a line
            texture_rect.left = 0;
            texture_rect.top += texture_rect.height;
        }
        //progress to the next frame
        elapsed_time -= time_per_frame;
        if (repeat)
        {
            current_frame = (current_frame + 1) % num_frames;
            if (current_frame == 0) texture_rect = sf::IntRect(0, 0, frame_size.x, frame_size.y);
        }
        else current_frame++;
    }
    sprite.setTextureRect(texture_rect);
}
void animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(sprite, states);
}
animation::type animation::get_type() const
{
    return atype;
}
