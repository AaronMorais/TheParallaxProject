#ifndef _PREPROCESSOR_H__
#define _PREPROCESSOR_H__

#include "lego_data.h"

namespace plx {

class Preprocessor {
public:
    Preprocessor(std::shared_ptr<plx::LegoData> data);

    void process();

    std::shared_ptr<plx::LegoData> data();

    class Brick {
    public:
        const std::vector<glm::vec3>& location() const;
    protected:
        std::vector<glm::vec3> m_location;
    };

    class OneOnePlate : public Brick {
    public:
        OneOnePlate(const std::vector<glm::vec3>& location);
        static const std::vector<std::vector<glm::vec3>>& orientations();
    private:
        static std::vector<std::vector<glm::vec3>> m_orientations;
    };

    class OneTwoPlate : public Brick {
    public:
        OneTwoPlate(const std::vector<glm::vec3>& location);
        static const std::vector<std::vector<glm::vec3>>& orientations();
    private:
        static std::vector<std::vector<glm::vec3>> m_orientations;
    };

    class OneFourPlate : public Brick {
    public:
        OneFourPlate(const std::vector<glm::vec3>& location);
        static const std::vector<std::vector<glm::vec3>>& orientations();
    private:
        static std::vector<std::vector<glm::vec3>> m_orientations;
    };

private:
    std::shared_ptr<plx::LegoData> m_data;
};

}

#endif
