#include "lab_m1/tema1/tema1.h"

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#include <math.h>

namespace constants
{
    const auto kRectangleToRombX{ 45 };
    const auto kRectangleToRombY{ 20 };
    const auto kFrameToSceneOverhead{ 5 };
    const auto kMinTimeToAppear{ 2.0f };
    const auto kMaxTimeToAppear{ 4.0f };
    const auto kMinTimeToAppearForStars{ 7.0f };
    const auto kMaxTimeToAppearForStars{ 10.0f };
    const auto kFirstTimeToAppear{ 2.5f };
    const auto kFirstTimeToAppearForStars{ 8.0f };
    const auto kIndexOutOfBounds{ -1 };
    const auto kMinSpeed{ 50 };
    const auto kMaxSpeed{ 100 };
    const auto kProjectileCooldown{ 1.5f };
    const auto kProjectileSpeed{ 75 };
    const auto kProjectileAngularSpeed{ 0.5f };
    const auto kScaleSpeed{ 1.0f };
    const auto kImprovmentMana{ 2.0f };

}

namespace helpers
{
    float average(float f1, float f2)
    {
        return (f1 + f2) / 2;
    }

    int getIndexfromColor(const glm::vec3& color)
    {
        if (color == glm::vec3{ 255, 0, 255 })
            return 0;
        else if (color == glm::vec3{ 0, 204, 255 })
            return 1;
        else if (color == glm::vec3{ 153, 153, 255 })
            return 2;
        else if (color == glm::vec3{ 255, 255, 0 })
            return 3;

        return constants::kIndexOutOfBounds;
    }
}

using namespace std;
using namespace m1;
using namespace constants;
using namespace helpers;


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    srand(NULL);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    squareSide = 100;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateX1 = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;

    indexPrototype = kIndexOutOfBounds;
    indexSceneInterval = kIndexOutOfBounds;
    timeToAppear = kFirstTimeToAppear;
    timeToAppearForStars = kFirstTimeToAppearForStars;
    enemySpeed = 0;
    lives = 3;
    currentMana = 10;

    endLine = Rectangle{ 20, 30, 50, 400 };

    frames = {
        Square{ 30, 638, 80 },
        Square{ 200, 638, 80 },
        Square{ 370, 638, 80 },
        Square{ 540, 638, 80 }
    };

    scenes = {
        Scene{ Square{ 100, 30, squareSide } },
        Scene{ Square{ 250, 30, squareSide } },
        Scene{ Square{ 400, 30, squareSide } },
        Scene{ Square{ 100, 180, squareSide } },
        Scene{ Square{ 250, 180, squareSide } },
        Scene{ Square{ 400, 180, squareSide } },
        Scene{ Square{ 100, 330, squareSide } },
        Scene{ Square{ 250, 330, squareSide } },
        Scene{ Square{ 400, 330, squareSide } }
    };

    prototypeGuns = {
        Gun{ Diamond{ 30, 650, 55 }, "diamond1", Rectangle{ 75, 670, 30, 15}, "rectangle1", glm::vec3{ 255, 0, 255 },  false, 1},
        Gun{ Diamond{ 200, 650, 55 }, "diamond2", Rectangle{ 245, 670, 30, 15 }, "rectangle2", glm::vec3{ 0, 204, 255 }, false, 2 },
        Gun{ Diamond{ 370, 650, 55 }, "diamond3", Rectangle{ 415, 670, 30, 15 }, "rectangle3", glm::vec3{ 153, 153, 255 }, false, 2 },
        Gun{ Diamond{ 540, 650, 55 }, "diamond4", Rectangle{ 585, 670, 30, 15 }, "rectangle4", glm::vec3{ 255, 255, 0 }, false, 3 }
    };

    prototypeEnemy = {
        Enemy{ Hexagone{ 1300, 385, 50 }, "hexagone1", glm::vec3{ 255, 0, 255 }, Hexagone{ 1300, 385, 30 }, "hexagone2", glm::vec3{ 0, 128, 0 }},
        Enemy{ Hexagone{ 1300, 230, 50 }, "hexagone3", glm::vec3{ 0, 204, 255 }, Hexagone{ 1300, 230, 30 }, "hexagone4", glm::vec3{ 0, 0, 1 }},
        Enemy{ Hexagone{ 1300, 75, 50 }, "hexagone5", glm::vec3{ 153, 153, 255 }, Hexagone{ 1300, 75, 30 }, "hexagone6", glm::vec3{ 1, 0, 0 }},
        Enemy{ Hexagone{ 1300, 385, 50 }, "hexagone7", glm::vec3{ 255, 255, 0 }, Hexagone{ 1300, 385, 30 }, "hexagone8", glm::vec3{ 0, 0, 0 }}

    };

    sceneIntervals = {
        SceneInterval{ 30, 30 + squareSide },
        SceneInterval{ 180, 180 + squareSide },
        SceneInterval{ 330, 330 + squareSide }
    };
    

    prototypeProjectiles = {
        Projectile { Star{ -1, -1, 0, 20.0f, 10.0f, 5 }, "star1", glm::vec3{255, 0, 255}},
        Projectile { Star{ -1, -1, 0, 20.0f, 10.0f, 5 }, "star2", glm::vec3{0, 204, 255}},
        Projectile { Star{ -1, -1, 0, 20.0f, 10.0f, 5 }, "star3", glm::vec3{153, 153, 255}},
        Projectile { Star{ -1, -1, 0, 20.0f, 10.0f, 5 }, "star4", glm::vec3{255, 255, 0}}
    };

    gameLives = {
        Square { 755, 630, 85},
        Square { 925, 630, 85},
        Square { 1095, 630, 85}
    };

    mana = {
        Projectile { Star{ 760, 600, 0, 10.0f, 5.0f, 5 }, "star1", glm::vec3{1, 0.5f, 0}},
        Projectile { Star{ 800, 600, 0, 10.0f, 5.0f, 5 }, "star1", glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 840, 600, 0, 10.0f, 5.0f, 5 }, "star1", glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 880, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 880, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 920, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 960, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 1000, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 1040, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}},
        Projectile{ Star{ 1080, 600, 0, 10.0f, 5.0f, 5 }, "star1",glm::vec3{1, 0.5f, 0}}
    };


    Mesh* square = object2D::CreateSquare("square", corner, squareSide, glm::vec3(0, 128, 0), true);
    Mesh* square1 = object2D::CreateSquare("square1", corner, 80.0f, glm::vec3(125, 0, 125), false);
    Mesh* square2 = object2D::CreateSquare("square2", corner, 85.0f, glm::vec3(255, 0, 125), true);

    Mesh* rectangle = object2D::CreateRectangle("rectangle", corner, endLine.width, endLine.height, glm::vec3(1, 0, 0), true);
    for (const auto& gun : prototypeGuns)
    {
        Mesh* diamond = object2D::CreateDiamond(gun.diamondMeshName, corner,
            gun.diamond.length, gun.color, true);
        AddMeshToList(diamond);

        Mesh* rectangle = object2D::CreateRectangle(gun.rectangleMeshName, corner,
            gun.rectangle.width, gun.rectangle.height, gun.color, true);
        AddMeshToList(rectangle);
    }

    for (auto& enemy : prototypeEnemy)
    {
        Mesh* hexagoneLarge = object2D::CreateHexagons(enemy.largeHexagoneMeshName, corner,
            enemy.largeHexagone.length, enemy.colorHexLarge, true);
        AddMeshToList(hexagoneLarge);

        Mesh* hexagoneSmall = object2D::CreateHexagons(enemy.smallHexagoneMeshName, corner,
            enemy.smallHexagone.length, enemy.colorHexSmall, true);
        AddMeshToList(hexagoneSmall);
    }

    for (auto& projectile : prototypeProjectiles)
    {
        Mesh* star = object2D::CreateStar(projectile.projectileMeshName, corner, projectile.star.outerDistance,
            projectile.star.innerDistance, projectile.star.numPoints, projectile.color, true);
        AddMeshToList(star);
    }

    Mesh* star1 = object2D::CreateStar("star", glm::vec3(0.0f, 0.0f, 0.0f), 10.0f, 5.0f, 5, glm::vec3(1, 0.5f, 0), true);
    Mesh* freeStar = object2D::CreateStar("freeStar", glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 10.0f, 5, glm::vec3(0, 1, 2), true);

    AddMeshToList(square);
    AddMeshToList(square1);
    AddMeshToList(square2);
    AddMeshToList(rectangle);
    AddMeshToList(star1);
    AddMeshToList(freeStar);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    CreateSceneProjectiles();

    UpdateSceneEnemies(deltaTimeSeconds);
    UpdateSceneGuns(deltaTimeSeconds);
    UpdateSceneProjectiles(deltaTimeSeconds);
    UpdateSceneStars(deltaTimeSeconds);

    CheckEnemyProjectileCollisions();
    CheckEnemyGunCollisions();

    DestroySceneEnemies();
    FreeScenes();
    DestroySceneGuns();
    DestroySceneProjectiles();
    DestroyFreeStars();

    RenderSceneStars();
    RenderEndLine();
    RenderFrames();
    RenderPrototypeGuns();
    RenderSceneGuns();
    RenderHoldedGun();
    RenderSceneEnemies();
    RenderSceneProjectiles();
    RenderScenes();
    RenderGameLives();
    RenderMana();

    EndGame();

    // life for first dimond
    modelMatrix25 = glm::mat3(1);
    modelMatrix25 *= transform2D::Translate(35, 600);

    // mana for second dimond
    modelMatrix26 = glm::mat3(1);
    modelMatrix26 *= transform2D::Translate(205, 600);

    modelMatrix27 = glm::mat3(1);
    modelMatrix27 *= transform2D::Translate(245, 600);

    // mana for third dimond
    modelMatrix28 = glm::mat3(1);
    modelMatrix28 *= transform2D::Translate(375, 600);

    modelMatrix29 = glm::mat3(1);
    modelMatrix29 *= transform2D::Translate(415, 600);

    // mana for third dimond
    modelMatrix30 = glm::mat3(1);
    modelMatrix30 *= transform2D::Translate(545, 600);

    modelMatrix31 = glm::mat3(1);
    modelMatrix31 *= transform2D::Translate(585, 600);

    modelMatrix32 = glm::mat3(1);
    modelMatrix32 *= transform2D::Translate(625, 600);

    // render mana for first dimond
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix25);

    // render mana for second dimond
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix26);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix27);

    // render mana for third dimond
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix28);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix29);

    // render mana for forth dimond
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix30);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix31);
    RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix32);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (isMousePressed)
    {
        this->mouseX = mouseX;
        this->mouseY = mouseY;
    }
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2)
    {
        auto i{ 0 };
        for (const auto& frame : frames)
        {
            /* see if the cursor is in any frame*/
            if (mouseX <= frame.centerX + frame.length
                && mouseX >= frame.centerX
                && resolution.y - mouseY <= frame.centerY + frame.length
                && resolution.y - mouseY >= frame.centerY)
            {
                isMousePressed = true;
                /* know what gun we draged*/
                indexHoldedGun = i;
            }

            i++;
        }

        for (auto& freeStar : freeStars)
        {
            /* see if the cursor is in the star so i can click on it */
            if (mouseX <= freeStar.star.centerX + freeStar.star.outerDistance
                && mouseX >= freeStar.star.centerX - freeStar.star.outerDistance
                && resolution.y - mouseY <= freeStar.star.centerY + freeStar.star.outerDistance
                && resolution.y - mouseY >= freeStar.star.centerY - freeStar.star.outerDistance)
            {
                freeStar.alive = false;
                /* increas the current mana if i click on stars
                but not overflow the capacity */
                if (currentMana < numMana)
                    currentMana += 1;
            }
        }
    }

    /* for the right button */
    if (button == GLFW_MOUSE_BUTTON_3)
    {
        for (int i = 0; i < numScenes; i++)
        {   /* if i have the cursor in the scene Squares*/
            if (mouseX <= scenes[i].square.centerX + scenes[i].square.length
                && mouseX >= scenes[i].square.centerX
                && resolution.y - mouseY <= scenes[i].square.centerY + scenes[i].square.length
                && resolution.y - mouseY >= scenes[i].square.centerY)
            {

                for (int j = 0; j < sceneGuns.size(); j++)
                {
                    /* if the gun is where i have the cursor, make it not alive */
                    if (sceneGuns[j].indexScene == i)
                        sceneGuns[j].alive = false;
                }
            }

        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (button == GLFW_MOUSE_BUTTON_2 && isMousePressed)
    {
        
        for (int i = 0; i < numScenes; i++)
        {
            /* if the  cursor is in the Square Scenes and the spot is empty and have enough mana to place the gun */
            if (mouseX <= scenes[i].square.centerX + scenes[i].square.length
                && mouseX >= scenes[i].square.centerX
                && resolution.y - mouseY <= scenes[i].square.centerY + scenes[i].square.length
                && resolution.y - mouseY >= scenes[i].square.centerY
                && scenes[i].empty && prototypeGuns[indexHoldedGun].mana <= currentMana)
            {
                const auto interval{ GetIntervalFromScene(scenes[i])};

                /* place the gun */
                sceneGuns.push_back(
                    Gun{
                        Diamond{ scenes[i].square.centerX + kFrameToSceneOverhead, scenes[i].square.centerY + squareSide / 4, 55, 1 },
                        prototypeGuns[indexHoldedGun].diamondMeshName,
                        Rectangle{ scenes[i].square.centerX + kRectangleToRombX + kFrameToSceneOverhead, scenes[i].square.centerY + squareSide / 4 + kRectangleToRombY, 30, 15, 1},
                        prototypeGuns[indexHoldedGun].rectangleMeshName,
                        prototypeGuns[indexHoldedGun].color,
                        true,
                        prototypeGuns[indexHoldedGun].mana,
                        interval,
                        i,
                    }
                );
                /* the scene spot is no longer available and decrease the mana */
                scenes[i].empty = false;
                currentMana -= prototypeGuns[indexHoldedGun].mana;
                break;
            }
        }
    }
 
    /* when drop the mouse is no longer pressed */
    isMousePressed = false;
    indexHoldedGun = numPrototypes;
    mouseX = INT_MIN;
    mouseY = INT_MIN;
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

int Tema1::GetIntervalFromScene(const Scene& scene)
{
    /* we have three intervals, from where the hexagones can come */
    for (int i = 0; i < numSceneIntervals; i++)
    {
        if (scene.square.centerY <= sceneIntervals[i].maxY
            && scene.square.centerY >= sceneIntervals[i].minY) {
            return i;
        }
    }

    return kIndexOutOfBounds;
}

void Tema1::EnemyEndLineCollide(Enemy& sceneEnemy)
{
    /* condition the hexagone touch the red rectangle at the end */
    if (sceneEnemy.largeHexagone.centerX <= endLine.centerX + endLine.width
        && sceneEnemy.smallHexagone.centerX <= endLine.centerX + endLine.width)
    {
        /* it will dye and the total lives will decrese */
        sceneEnemy.alive = false;
        lives--;
    }
}

bool Tema1::EnemyProjectileCollide(Enemy& sceneEnemy, Projectile& sceneProjectile)
{
    /* calculate the distance between the centers of the two figures */
    auto distance = glm::distance(
        glm::vec2(sceneEnemy.largeHexagone.centerX, sceneEnemy.largeHexagone.centerY),
        glm::vec2(sceneProjectile.star.centerX, sceneProjectile.star.centerY));

    /* return true if the distce is lower */
    return distance <= sceneEnemy.largeHexagone.length + sceneProjectile.star.outerDistance;
}

bool Tema1::EnemyGunCollide(Enemy& sceneEnemy, Gun& sceneGun)
{
    auto distance = glm::distance(
        glm::vec2(sceneEnemy.largeHexagone.centerX, sceneEnemy.largeHexagone.centerY),
        glm::vec2(sceneGun.diamond.centerX, sceneGun.diamond.centerY));

    return distance <= sceneEnemy.largeHexagone.length + sceneGun.diamond.length;
}

void m1::Tema1::CreateSceneProjectiles()
{
    for (auto& sceneGun : sceneGuns)
    {
        for (auto& sceneEnemy : sceneEnemies)
        {
            /* see if the color of the guns matches the color of the enemy so it can create
             and if is the same interval */
            if (sceneGun.color == sceneEnemy.colorHexLarge &&
                sceneGun.interval == sceneEnemy.interval &&
                sceneGun.alive && sceneEnemy.alive && sceneGun.cooldown <= 0)
            {
                auto color{ sceneGun.color };

                /* create the scene projectiles */
                sceneProjectiles.push_back(
                    Projectile{
                        Star{
                            sceneGun.rectangle.centerX + sceneGun.rectangle.width / 2,
                            sceneGun.rectangle.centerY,
                            0,
                            prototypeProjectiles[getIndexfromColor(color)].star.outerDistance,
                            prototypeProjectiles[getIndexfromColor(color)].star.innerDistance,
                            prototypeProjectiles[getIndexfromColor(color)].star.numPoints,
                        },                        
                        prototypeProjectiles[getIndexfromColor(color)].projectileMeshName,
                        prototypeProjectiles[getIndexfromColor(color)].color,
                        true,
                        prototypeProjectiles[getIndexfromColor(color)].interval
                    });

                sceneGun.cooldown = kProjectileCooldown;
            }
        }
    }
}

void Tema1::UpdateSceneEnemies(float deltaTime)
{
    if (timeToAppear <= 0)
    {
        /* get random  scene interval where they can appear */
        indexPrototype = rand() % numPrototypes;
        indexSceneInterval = rand() % numSceneIntervals;
        /* calculate random time to appear */
        timeToAppear = kMinTimeToAppear + static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / (kMaxTimeToAppear - kMinTimeToAppear)));
        /* calculate the random speed they are comming */
        enemySpeed = kMaxSpeed + rand() % kMaxSpeed;

        /* make them appear */
        sceneEnemies.push_back(
            Enemy{
                Hexagone{
                    prototypeEnemy[indexPrototype].largeHexagone.centerX,
                    /* it needs to appear at the half of any scene intervals */
                    average(sceneIntervals[indexSceneInterval].minY, sceneIntervals[indexSceneInterval].maxY),
                    prototypeEnemy[indexPrototype].largeHexagone.length,
                    1
                },
                prototypeEnemy[indexPrototype].largeHexagoneMeshName,
                prototypeEnemy[indexPrototype].colorHexLarge,
                Hexagone{
                    prototypeEnemy[indexPrototype].smallHexagone.centerX,
                    average(sceneIntervals[indexSceneInterval].minY, sceneIntervals[indexSceneInterval].maxY),
                    prototypeEnemy[indexPrototype].smallHexagone.length,
                    1
                },
                prototypeEnemy[indexPrototype].smallHexagoneMeshName,
                prototypeEnemy[indexPrototype].colorHexSmall,
                true,
                enemySpeed,
                indexSceneInterval
            }
        );
    }

    timeToAppear -= deltaTime;

    for (auto& sceneEnemy : sceneEnemies)
    {
        /* calculate the scale if the enemy die */
        if (!sceneEnemy.alive)
        {
            sceneEnemy.smallHexagone.scale -= kScaleSpeed * deltaTime;
            sceneEnemy.largeHexagone.scale -= kScaleSpeed * deltaTime;
        }
        else
        {
            /* if they don t die just go and if they get to the end die */
            sceneEnemy.smallHexagone.centerX -= sceneEnemy.speed * deltaTime;
            sceneEnemy.largeHexagone.centerX -= sceneEnemy.speed * deltaTime;

            EnemyEndLineCollide(sceneEnemy);
        }
    }
}

void Tema1::UpdateSceneGuns(float deltaTime)
{
    for (auto& sceneGun : sceneGuns)
    {
        if (sceneGun.cooldown > 0)
            sceneGun.cooldown -= deltaTime;

        /* if the gun should be dead, calculate the scale cause it needs to dissapear */
        if (!sceneGun.alive) 
        {
            sceneGun.diamond.scale -= kScaleSpeed * deltaTime;
            sceneGun.rectangle.scale -= kScaleSpeed * deltaTime;
        }
    }
}

void Tema1::UpdateSceneProjectiles(float deltaTime)
{
    for (auto& sceneProjectile : sceneProjectiles)
    {
        /* the projectiles needs rotate so calc the speed and angular */
        sceneProjectile.star.centerX += kProjectileSpeed * deltaTime;
        sceneProjectile.star.angular += (float)kProjectileAngularSpeed * deltaTime;
        
        /* if the star get to the end, just kill it */
        if (sceneProjectile.star.centerX >= resolution.x)
        {
            sceneProjectile.alive = false;
        }
    }
}

void Tema1::UpdateSceneStars(float deltaTime)
{
    /* appear random stars on the screen */
    if (timeToAppearForStars <= 0)
    {
        /* at a random interval  */
        timeToAppearForStars = kMinTimeToAppear + static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / (kMaxTimeToAppearForStars - kMinTimeToAppearForStars)));

        /* three at a time */
        for (int i = 0; i < 3; i++)
        {
            float randomX = static_cast<float>(rand() % resolution.x);
            float randomY = static_cast<float>(rand() % resolution.y);
            freeStars.push_back(
                Projectile{
                    Star { randomX, randomY, 0, 20.0f, 10.0f, 5},
                    "freeStar",
                    glm::vec3(220, 170, 190),
                    true
                });
        }
    }

    timeToAppearForStars -= deltaTime;

}

void Tema1::CheckEnemyProjectileCollisions()
{
    for (auto& sceneEnemy : sceneEnemies)
    {
        for (auto& sceneProjectile : sceneProjectiles)
        {
            /* if projectile color matches enemy color
            call tge collide func*/
            if (sceneEnemy.colorHexLarge == sceneProjectile.color &&
                EnemyProjectileCollide(sceneEnemy, sceneProjectile))
            {
                /* update fileds */
                sceneProjectile.alive = false;
                /* initilaized lives with 3 */
                sceneEnemy.lives--;
                /* if projectiles go in an emey 3 times and lives get on 0 */
                if (sceneEnemy.lives == 0)
                {
                    /* enemy needs to die */
                    sceneEnemy.alive = false;
                    /* if total mana is not full, then when a hex die wee and a number of mana at the current mana */
                    if (currentMana + kImprovmentMana <= numMana)
                        currentMana += kImprovmentMana;
                }
            }
        }
    }
}

void Tema1::CheckEnemyGunCollisions()
{
    for (auto& sceneEnemy : sceneEnemies)
    {
        for (auto& sceneGun : sceneGuns)
        {
            /* if hex color match the gun color then kill the gun
            cause the gun  didn t make it to kill de hex*/
            if ( EnemyGunCollide(sceneEnemy, sceneGun))
            {
                // kill the gun
                sceneGun.alive = false;
            }
        }
    }
}

void Tema1::DestroySceneEnemies()
{
    /* if the condition that enemy is dead and scale < 0 the fo through the list with scene enemies and erase the dead enemy */
    auto deadEnemyCondition = [](Enemy& enemy) { 
        return !enemy.alive && enemy.largeHexagone.scale <= 0 && enemy.smallHexagone.scale <= 0;
        };
    sceneEnemies.erase(std::remove_if(sceneEnemies.begin(), sceneEnemies.end(), deadEnemyCondition), sceneEnemies.end());
}

void Tema1::FreeScenes()
{
    for (const auto& sceneGun : sceneGuns)
    {
        /* if the scene gun is no longer alive i use scale to dissapear */
        if (!sceneGun.alive && sceneGun.diamond.scale <= 0 && sceneGun.rectangle.scale <= 0)
        {
            /* now the spot is empty for another gun to come in */
            scenes[sceneGun.indexScene].empty = true;
        }
    }
}

void Tema1::DestroySceneGuns()
{
    /* if the condition that gun is dead and scale < 0 the fo through the list with scene guns and erase the dead gun */
    auto deadGunsCondition = [](Gun& gun) {
        return !gun.alive && gun.diamond.scale <= 0 && gun.rectangle.scale <= 0;
        };
    sceneGuns.erase(std::remove_if(sceneGuns.begin(), sceneGuns.end(), deadGunsCondition), sceneGuns.end());
}

void Tema1::DestroySceneProjectiles()
{
    /* if the condition that projectile is dead go through the list with scene projectiles and erase the dead projectile */
    auto deadProjectilesCondition = [](Projectile& projectile) { return !projectile.alive; };
    sceneProjectiles.erase(std::remove_if(sceneProjectiles.begin(), sceneProjectiles.end(), deadProjectilesCondition), sceneProjectiles.end());
}

void Tema1::DestroyFreeStars()
{
    /* if the condition that free star is dead then go through the list with free stars and erase the dead free star */
    auto deadFreeStarsCondition = [](Projectile& freeStar) { return !freeStar.alive; };
    freeStars.erase(std::remove_if(freeStars.begin(), freeStars.end(), deadFreeStarsCondition), freeStars.end());
}

void Tema1::RenderEndLine()
{
    auto modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(endLine.centerX, endLine.centerY);
    RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);
}

void Tema1::RenderFrames()
{
    for (const auto& frame : frames)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(frame.centerX, frame.centerY);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderPrototypeGuns()
{
    for (const auto& gun : prototypeGuns)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gun.diamond.centerX, gun.diamond.centerY);
        RenderMesh2D(meshes[gun.diamondMeshName], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gun.rectangle.centerX, gun.rectangle.centerY);
        RenderMesh2D(meshes[gun.rectangleMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderSceneGuns()
{

    for (const auto& gun : sceneGuns)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gun.diamond.centerX + gun.diamond.length / 2, gun.diamond.centerY + gun.diamond.length / 4);
        modelMatrix *= transform2D::Scale(gun.diamond.scale, gun.diamond.scale);
        modelMatrix *= transform2D::Translate(-gun.diamond.length / 2 * gun.diamond.scale, - gun.diamond.length / 4);

        RenderMesh2D(meshes[gun.diamondMeshName], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gun.rectangle.centerX - gun.rectangle.width / 4, gun.rectangle.centerY - gun.rectangle.height / 3);
        modelMatrix *= transform2D::Scale(gun.rectangle.scale, gun.rectangle.scale);
        modelMatrix *= transform2D::Translate(gun.rectangle.width / 4 * gun.rectangle.scale, gun.rectangle.height / 3);
        RenderMesh2D(meshes[gun.rectangleMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderHoldedGun()
{
    if (isMousePressed)
    {
        /* if the mouse is pressed i want to render the figure anywhere ehere the cursor is until i depress it */
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(mouseX - kRectangleToRombX, resolution.y - mouseY - kRectangleToRombY);
        RenderMesh2D(meshes[prototypeGuns[indexHoldedGun].diamondMeshName], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(mouseX, resolution.y - mouseY);
        RenderMesh2D(meshes[prototypeGuns[indexHoldedGun].rectangleMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderScenes()
{

    for (const auto& scene : scenes)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(scene.square.centerX, scene.square.centerY);
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderSceneEnemies()
{
    for (const auto& sceneEnemy : sceneEnemies)
    { 

        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(sceneEnemy.smallHexagone.centerX, sceneEnemy.smallHexagone.centerY);
        modelMatrix *= transform2D::Scale(sceneEnemy.smallHexagone.scale, sceneEnemy.smallHexagone.scale);
        RenderMesh2D(meshes[sceneEnemy.smallHexagoneMeshName], shaders["VertexColor"], modelMatrix);

        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(sceneEnemy.largeHexagone.centerX, sceneEnemy.largeHexagone.centerY);
        modelMatrix *= transform2D::Scale(sceneEnemy.smallHexagone.scale, sceneEnemy.smallHexagone.scale);
        RenderMesh2D(meshes[sceneEnemy.largeHexagoneMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderSceneProjectiles()
{
    for (const auto& sceneProjectile : sceneProjectiles)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(sceneProjectile.star.centerX, sceneProjectile.star.centerY);
        modelMatrix *= transform2D::Rotate(sceneProjectile.star.angular);
        RenderMesh2D(meshes[sceneProjectile.projectileMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderGameLives()
{
    for (int i = 0; i < lives; i++)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(gameLives[i].centerX, gameLives[i].centerY);
        RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::RenderMana()
{
    for (int i = 0; i < currentMana; i++)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(mana[i].star.centerX, mana[i].star.centerY);
        RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
    }
}

void m1::Tema1::RenderSceneStars()
{
    for (const auto& freeStar : freeStars)
    {
        auto modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(freeStar.star.centerX, freeStar.star.centerY);
        RenderMesh2D(meshes[freeStar.projectileMeshName], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::EndGame()
{
    if (lives > 0)
        return;

    /* no more lives, game over */
    std::cout << "Game Over!!!!" << std::endl;
    exit(1);
}
