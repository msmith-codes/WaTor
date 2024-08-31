#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

#include <Entity.hpp>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const std::string WINDOW_TITLE = "WaTor";

const unsigned int GRID_SIZE = 20;
const unsigned int NUM_FISH = 100;
const unsigned int NUM_SHARKS = 50;
const unsigned int FISH_SPREAD = 100;
const unsigned int SHARK_SPREAD = 50;

const float SHARK_RADIUS = 0.5f;

void onStart();
void onUpdate();
void onRender();

void drawGrid();

bool checkCollision(Vector2 a, Vector2 b);
float distanceTo(Vector2 pos1, Vector2 pos2);

std::vector<Entity> entities;

int main(int argc, char* argv[])
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str());
    SetTargetFPS(5);
    srand(static_cast<unsigned>(time(0)));
    onStart();

    while(!WindowShouldClose()) {
        onUpdate();
        onRender();
    }

    return 0;
}

void onStart()
{
    entities.clear();

    for (int i = 0; i < NUM_FISH; ++i) {
        Entity fish = { 
            { (float)(rand() % (WINDOW_WIDTH / GRID_SIZE)), (float)(rand() % (WINDOW_HEIGHT / GRID_SIZE)) }, 
            EntityType::FISH, 
            0 
        };
        entities.push_back(fish);
    }

    for (int i = 0; i < NUM_SHARKS; ++i) {
        Entity shark = { 
            { (float)(rand() % (WINDOW_WIDTH / GRID_SIZE)), (float)(rand() % (WINDOW_HEIGHT / GRID_SIZE)) }, 
            EntityType::SHARK, 
            0 
        };
        entities.push_back(shark);
    }

    TraceLog(LOG_INFO, "Total entities after initialization: %lu", entities.size());

}

void onUpdate()
{
    std::vector<Entity> updatedEntities;
    for (auto& entity : entities) {
        if (entity.type == EntityType::FISH) {
            entity.position.x += (rand() % (3) - 1);
            entity.position.y += (rand() % (3) - 1);

            // Wrap around
            if (entity.position.x < 0) entity.position.x = (WINDOW_WIDTH / GRID_SIZE);
            if (entity.position.x >= (WINDOW_WIDTH / GRID_SIZE)) entity.position.x = 0;
            if (entity.position.y < 0) entity.position.y = (WINDOW_HEIGHT / GRID_SIZE);
            if (entity.position.y >= (WINDOW_HEIGHT / GRID_SIZE)) entity.position.y = 0;
        } else if (entity.type == EntityType::SHARK) {
            entity.position.x += (rand() % (3) - 1);
            entity.position.y += (rand() % (3) - 1);

            // Wrap around
            if (entity.position.x < 0) entity.position.x = (WINDOW_WIDTH / GRID_SIZE) - 1;
            if (entity.position.x >= (WINDOW_WIDTH / GRID_SIZE)) entity.position.x = 0;
            if (entity.position.y < 0) entity.position.y = (WINDOW_HEIGHT / GRID_SIZE) - 1;
            if (entity.position.y >= (WINDOW_HEIGHT / GRID_SIZE)) entity.position.y = 0;
        }
        updatedEntities.push_back(entity);
    }

    std::vector<Entity> finalEntities;
    std::vector<bool> fishEaten(entities.size(), false); // Track if fish is eaten

    for (const auto& shark : updatedEntities) {
        if (shark.type == EntityType::SHARK) {
            for (auto it = updatedEntities.begin(); it != updatedEntities.end(); ) {
                if (it->type == EntityType::FISH && distanceTo(shark.position, it->position) < SHARK_RADIUS) {
                    TraceLog(LOG_INFO, "Shark at (%f, %f) eats fish at (%f, %f)", 
                             shark.position.x, shark.position.y, it->position.x, it->position.y);
                    it = updatedEntities.erase(it);
                    continue;
                } else {
                    ++it;
                }
            }
        }
    }

    for (size_t i = 0; i < updatedEntities.size(); ++i) {
        if (!fishEaten[i]) {
            finalEntities.push_back(updatedEntities[i]);
        }
    }

    entities = finalEntities;
}

void onRender()
{
    BeginDrawing();
        ClearBackground(RAYWHITE);
        drawGrid();
        for (const auto& entity : entities) {
            if (entity.type == EntityType::FISH) {
                DrawRectangle((int)entity.position.x * GRID_SIZE, (int)entity.position.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, BLUE);
            } else if (entity.type == EntityType::SHARK) {
                DrawRectangle((int)entity.position.x * GRID_SIZE, (int)entity.position.y * GRID_SIZE, GRID_SIZE, GRID_SIZE, RED);
            }
        }
    EndDrawing();
}

void drawGrid()
{
    for(unsigned int x = 0; x < WINDOW_WIDTH; x += GRID_SIZE) {
        DrawLine(x, 0, x, WINDOW_HEIGHT, LIGHTGRAY);
    }

    for(unsigned int y = 0; y < WINDOW_HEIGHT; y += GRID_SIZE) {
        DrawLine(0, y, WINDOW_WIDTH, y, LIGHTGRAY);
    }
}

float distanceTo(Vector2 a, Vector2 b)
{
    return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool checkCollision(Vector2 pos1, Vector2 pos2) {
    float x1 = pos1.x * GRID_SIZE;
    float y1 = pos1.y * GRID_SIZE;
    float x2 = pos2.x * GRID_SIZE;
    float y2 = pos2.y * GRID_SIZE;

    return (x1 < x2 + GRID_SIZE && x1 + GRID_SIZE > x2 &&
            y1 < y2 + GRID_SIZE && y1 + GRID_SIZE > y2);
}