#include "ExtendedViewport.h"

unsigned int ExtendedViewport::instance_counter = 0;

ExtendedViewport::ExtendedViewport(Camera* cam, sf::FloatRect viewportRect)
{
    ExtendedViewport::instance_counter++;
}

ExtendedViewport::~ExtendedViewport()
{
    ExtendedViewport::instance_counter--; 
}

Camera* ExtendedViewport::getCamera()
{
    return nullptr;
}

sf::FloatRect ExtendedViewport::getFloatRect()
{
    return sf::FloatRect();
}

void ExtendedViewport::connect(Camera* cam)
{
    if (cam == nullptr) {return;}
    if (this->cam != nullptr) {std::cout << "Viewport:connect::cam already connected\n"; return;}
    this->cam = cam;
    cam->resizeToViewPort(sf::Vector2f(this->viewportRect.width, this->viewportRect.height));
    cam->getCameraView()->setViewport(this->viewportRect);
}

Camera* ExtendedViewport::disconnect()
{
    Camera* tempcam = this->cam;
    this->cam = nullptr;
    return tempcam;
}


