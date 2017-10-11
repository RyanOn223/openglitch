#ifndef SHADOW_H
#define SHADOW_H
#include "include.h"

sf::Vector2f normalize(sf::Vector2f vector);

/* Draws the shadows based on the
 * VertexArray given to it
 */
class ShadowDrawable : public sf::Drawable
                     , public sf::Transformable {
public:
    void setVertices(sf::VertexArray nVertices);
private:
    sf::VertexArray vertices;
    virtual void draw(sf::RenderTarget& target
                      , sf::RenderStates states) const;
};

/* Calculates shadow in
 * its update() method
 * and draws them in
 * its draw() method
 */
class ShadowHandler {
public:
    ShadowHandler();
    // Every 4 Vector2f's in the
    // obstaclePoints vector
    // represents 1 square
    void setObstacles(std::vector<sf::Vector2f> obstacles, int tileSize);
    void setScreenDiagonal(int screenX, int screenY);
    std::vector<sf::Vector2f> getObstaclesInRange
        (sf::Vector2f sightCenter);
    void update(sf::Vector2f sightCenter
                , sf::Vector2f viewCenter);
    void draw(sf::RenderWindow* window); // Calls the draw methods
                                         // for the shadows and the
                                         // BlockRedrawer
private:
    int screenDiagonal, screenWidth, screenHeight;
    std::vector<sf::Vector2f> obstaclePoints;
    ShadowDrawable shadows;
};
#endif
