#include <apollo/apollo.h>
#include "player.h"

class Game : public Sketch {
    public:
        Game();
    protected: 
        void start() override;
        void update(float deltaTime) override;

    private:
        Mesh m_quad = Mesh::createQuad();
        Player* m_player = nullptr;
};
