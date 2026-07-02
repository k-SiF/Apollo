#include <apollo/apollo.h>
#include "player.h"

using namespace apollo;

class Game : public Sketch {
    public:
        Game();
    protected: 
        void start() override;
        void update(float deltaTime) override;
        void onRender(float alpha) override;
        void drawWorld(float alpha) override;
        void drawUI() override;

    private:
        Font m_font { "assets/fonts/Courier_Prime.ttf", 48.0f };  
        Mesh m_quad = Mesh::createQuad();
        Player* m_player = nullptr;
        Entity* marker = nullptr;
        Entity* campfire = nullptr;
        const float G = -9.81;
};
