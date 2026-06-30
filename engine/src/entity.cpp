#include <apollo/entity.h>

namespace apollo {
    Entity::Entity(Mesh *mesh, glm::vec2 position) : m_position(position), m_prevPosition(position) ,m_mesh(mesh) {
    }

    Entity::~Entity() {
    }

    void Entity::update(float deltaTime) { 
        updateAnimation(deltaTime);
    }

    void Entity::fixedUpdate(float deltaTime) {

    }

    void Entity::setTextureSize(Texture* tex, float size) {
        m_tex = tex;
        if (!tex || tex->height() == 0) return;
        float aspect = (float)tex->width() / tex->height();
        m_scale = glm::vec2(aspect, 1.0f) * size;

        if (!m_colSizeSet) {
            m_colSize = glm::abs(m_scale);
        }
    }

    void Entity::sizeToFrame(const Frame& f, float size) {
        if (!f.texture || f.texture->height() == 0) return;

        float cellW = f.uv.w * f.texture->width();
        float cellH = f.uv.h * f.texture->height();
        if (cellH == 0.0f) return;

        float aspect = cellW / cellH;
        m_scale = glm::vec2(aspect, 1.0f) * size;

        if (!m_colSizeSet) {
            m_colSize = glm::abs(m_scale);
        }
    }

    
    void Entity::addAnimation(const std::string& name, Animation anim, float scale) {
        m_animations[name] = AnimEntry{ std::move(anim), scale };
        
        if (!m_currentAnim) {
            play(name);
            const Frame& f = m_animations[name].anim.currentFrame();
            m_tex = f.texture;
            setUVRect(f.uv);
            sizeToFrame(f, m_currentScale);
        }
    }
    
    void Entity::play(const std::string& name) {
        if (m_currentName == name) return;
        auto it = m_animations.find(name);
        if (it != m_animations.end()) {
            m_currentAnim = &it->second.anim;
            m_currentScale = it->second.scale;  
            m_currentName = name;
            m_currentAnim->reset();
        }
    }

    void Entity::updateAnimation(float deltaTime) {
        if (!m_currentAnim) return;
        
        m_currentAnim->update(deltaTime);
        const Frame& f = m_currentAnim->currentFrame();
        m_tex = f.texture;
        setUVRect(f.uv);
        sizeToFrame(f, m_currentScale); 
    }

    glm::mat4 Entity::getModelMatrix(float alpha) const {
        glm::vec2 pos = m_prevPosition * (1.0f - alpha) + m_position * alpha;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(pos, 0.0f)); // position
        model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0,0,1));  // rotation (around z, for 2D)
        model = glm::scale(model, glm::vec3(m_scale, 1.0f));                     // scale

        glm::vec2 a;

        // This is to unify visual scale and collision scale
        // m_scale varies too much in animations
        a.x = (-m_anchor.x * m_colSize.x) / (m_scale.x != 0.0f ? m_scale.x : 1.0f);
        a.y = ( m_anchor.y * m_colSize.y) / (m_scale.y != 0.0f ? m_scale.y : 1.0f);
        model = glm::translate(model, glm::vec3(a, 0.0f)); // anchor (shift)
        return model;
    }
    
    Collider Entity::getBounds() const {
        return getBoundsAt(m_position);
    }
    
    Collider Entity::getBoundsAt(glm::vec2 position) const {
        glm::vec2 offset = m_colSize * 0.5f;

        glm::vec2 anchorShift = glm::vec2(-m_anchor.x, m_anchor.y) * m_colSize;
        glm::vec2 center = position + anchorShift; 
        return { center - offset, center + offset};
    }
}
