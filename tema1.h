#pragma once

#include "components/simple_scene.h"

#include <array>
#include <vector>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

struct Square
{
    float centerX;
    float centerY;
    float length;
};

struct Diamond
{
    float centerX;
    float centerY;
    float length;
    float scale;
};

struct Rectangle
{
    float centerX;
    float centerY;
    float width;
    float height;
    float scale;
};

struct Hexagone
{
    float centerX;
    float centerY;
    float length;
    float scale;
};

struct Star
{
    float centerX;
    float centerY;
    float angular;
    float outerDistance;
    float innerDistance;
    float numPoints;
};


struct Gun
{
    Diamond diamond;
    std::string diamondMeshName;
    Rectangle rectangle;
    std::string rectangleMeshName;
    glm::vec3 color;
    bool alive;
    int mana;
    int interval;
    int indexScene;
    float cooldown{ 0 };
    
};

struct Enemy
{
    Hexagone largeHexagone;
    std::string largeHexagoneMeshName;
    glm::vec3 colorHexLarge;
    Hexagone smallHexagone;
    std::string smallHexagoneMeshName;
    glm::vec3 colorHexSmall;
    bool alive{ false };
    int speed{ 0 };
    int interval;
    float timeAlive{ 0 };
    int lives{ 3 };
};

struct Scene
{
    Square square;
    bool empty{ true };
};

struct SceneInterval
{
    float minY;
    float maxY;
};

struct Projectile
{
    Star star;
    std::string projectileMeshName;
    glm::vec3 color;
    bool alive;
    int interval;
 };


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
         Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        int GetIntervalFromScene(const Scene& scene);

        void EnemyEndLineCollide(Enemy& sceneEnemy);
        bool EnemyProjectileCollide(Enemy& sceneEnemy, Projectile& projectile);
        bool EnemyGunCollide(Enemy& sceneEnemy, Gun& sceneGun);


        void CreateSceneProjectiles();

        void UpdateSceneEnemies(float deltaTime);
        void UpdateSceneGuns(float deltaTime);
        void UpdateSceneProjectiles(float deltaTime);
        void UpdateSceneStars(float deltaTime);

        void CheckEnemyProjectileCollisions();
        void CheckEnemyGunCollisions();

        void DestroySceneEnemies();
        void FreeScenes();
        void DestroySceneGuns();
        void DestroySceneProjectiles();
        void DestroyFreeStars();

        void RenderEndLine();
        void RenderFrames();
        void RenderPrototypeGuns();
        void RenderSceneGuns();
        void RenderHoldedGun();
        void RenderScenes();
        void RenderSceneEnemies();
        void RenderSceneProjectiles();
        void RenderGameLives();
        void RenderMana();
        void RenderSceneStars();

        void EndGame();
       

     protected:
        float cx, cy;
        glm::mat3 modelMatrix, modelMatrix25, modelMatrix26, modelMatrix27, modelMatrix28, modelMatrix29, modelMatrix30, modelMatrix31, modelMatrix32;
        float translateX, translateY, translateX1;
        float scaleX, scaleY;
        float angularStep;

        glm::ivec2 resolution;
        float squareSide;

     private:
         static const int numPrototypes{ 4 };
         static const int numSceneIntervals{ 3 };
         static const int numScenes{ 9 };
         static const int numMana{ 10 };

         Rectangle endLine;

         std::array<Square, numPrototypes> frames;
         std::array<Scene, numScenes> scenes;
         std::array<Gun, numPrototypes> prototypeGuns;
         std::array<Enemy, numPrototypes> prototypeEnemy;
         std::array<Projectile, numPrototypes> prototypeProjectiles;
         std::array<SceneInterval, numSceneIntervals> sceneIntervals;
         std::array<Projectile, numMana> mana;

         std::vector<Gun> sceneGuns;
         std::vector<Enemy> sceneEnemies;
         std::vector<Projectile> sceneProjectiles;
         std::vector<Square> gameLives;
         std::vector<Projectile> freeStars;

         int indexHoldedGun;
         bool isMousePressed{ false };
         int mouseX, mouseY;

         int indexPrototype, indexSceneInterval;
         float timeToAppear;
         float timeToAppearForStars;
         int enemySpeed;

         int lives;
         int currentMana;
    };
}   // namespace m1
