#include "utils.h"

void utils::CenterizeTextOrigin(sf::Text &text)
{
    const sf::FloatRect labelBounds = text.getLocalBounds();
    text.setOrigin(0.5f * labelBounds.width,
                   0.5f * labelBounds.height);
}
