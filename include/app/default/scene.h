#pragma once

#include <glm/glm.hpp>

#include <vector>

struct SceneDefault {
  static constexpr auto MAX_BALLS = 32;
  struct Ball {
    glm::vec2 pos{};
    float rad = 0;
    glm::vec2 vel{};

    Ball() = default;
    Ball(const Ball &) = default;
    Ball(Ball &&) = default;
    Ball &operator=(const Ball &) = default;
    Ball &operator=(Ball &&) = default;

    Ball(const glm::vec2 pos, const float rad, const glm::vec2 vel);

    Ball operator+(const glm::vec2 t) const;

    bool intersects(const Ball &that) const;

    glm::vec3 asVec3() const { return {pos, rad}; }

    virtual void onCollision() {}
  };
  struct Face : Ball {
    // static constexpr unsigned int MAX_GENERATION = 255;
    unsigned int generation = 1;

    using Ball::Ball;

    virtual void onCollision() override { generation++; }

    // unsigned int level() const;
  };
  std::vector<Ball> balls{};
  std::vector<Face> faces{};
  std::vector<Ball *> shapes{};

  enum struct ConfigMode { DEFAULT, CUSTOM } configMode = ConfigMode::DEFAULT;
  struct Config {
    float radius;
    glm::vec2 velocity;
  } defaultConfig{}, customConfig{};
  static constexpr Config FALLBACK_CONFIG{100, {0, -200}};
  bool defaultSuccess = false;

  enum struct State {
    BALL,
    DELETE,
    FACE,
  } state = State::BALL;

  SceneDefault();

  // void resolveCollisions();
  void update(const float dt);

  void updateDefaultConfig();

  Ball preview{};
  int hoveringIndex = -1;
  Ball *hovering = nullptr;
  bool placeable = false;
  void updateHovering(const glm::ivec2 pos);
  void updatePreview(const glm::ivec2 pos);

  void tryPlaceBall();
  void tryPlaceFace();

  void tryDeleteGeneric();

  void onClick();

  void clear();
};
