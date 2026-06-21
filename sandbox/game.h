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

    private:
        Mesh m_quad = Mesh::createQuad();
        Player* m_player = nullptr;
        Entity* marker = nullptr;
        const float G = -9.81;
};
