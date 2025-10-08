#include <iostream>
#include "core/window.hpp"
#include "core/globals.hpp"
#include "entity/entity_manager.hpp"
#include "renderer/render_components.hpp"
#include "renderer/basic_shapes.h"
#include "renderer/renderer.hpp"

#include "physics/physics_component.h"
#include "physics/physics_system.h"
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#include <glad/glad.h>  // Correct way to include glad

// Window window;
// Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));



Entity createPhysicsSphere(EntityManager& manager, const glm::vec3& position, float radius, float mass, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
Entity createStaticPlane(EntityManager& manager, const glm::vec3& position, const glm::vec3& normal, float size);
Entity createCapsuleEntity(EntityManager& manager, const glm::vec3& position, float radius, float height, const glm::vec3& color = glm::vec3(0.8f, 0.3f, 0.3f));

int main(){

    window.init();
    Renderer renderer;
    renderer.init();

    PhysicsSystem physics;
    physics.init();

    EntityManager& manager = EntityManager::getInstance();


  Entity groundPlane = createStaticPlane(manager, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 50.0f);
  physics.addRigidBody(manager.getComponent<PhysicsComponent>(groundPlane.getID())->body);


//   Entity sphere2 = createPhysicsSphere(manager,glm::vec3(0.0f, 24.0f, -5.0f),1.0,2.0);
//   physics.addRigidBody(manager.getComponent<PhysicsComponent>(sphere2.getID())->body);

  
//   Entity sphere3 = createPhysicsSphere(manager,glm::vec3(0.0f, 10.0f, -1.0f),2.0,5.0, glm::vec3(0.0f, 1.0f, 1.0f));
//   physics.addRigidBody(manager.getComponent<PhysicsComponent>(sphere3.getID())->body);

//     Entity sphere;
//     manager.addEntity(sphere);
//     auto position = glm::vec3(0.0f, 2.0f, 0.0f);
//     float radius = 2.0;
//     auto color = glm::vec3(1.0f, 1.0f, 0.0f);
  


//     // 1. Transform
//     auto transform = std::make_shared<TransformComponent>();
//     transform->position = position;
//     manager.addComponent(sphere.getID(), transform);

//     // 2. Rendering
//     Sphere sphereData;
//     sphereData.position = position;
//     sphereData.radius = radius;
//     sphereData.segments = 64;

    
//     auto renderable = std::make_shared<RenderableComponent>(generateSphere(sphereData));
//     GLuint colorTexture = createColorTexture(color, 512, 512); // Red
//     TextureComponent texComp;
//     texComp.id = colorTexture;
//     texComp.type = "texture_diffuse";

//     renderable->textures.push_back(texComp);
//     renderable->meshes[0].textures.push_back(texComp);

//     manager.addComponent(sphere.getID(), renderable);


//     // Entity plane;
    // manager.addEntity(plane);
    // auto normal = glm::vec3(0.0f, 1.0f, 0.0f);
    // position = glm::vec3(0.0f, -2.0f, 0.0f);
    // float size = 50.0;
    // color = glm::vec3(0.8f, 0.8f, 0.8f);

    // // 1. Transform
    // auto transform2 = std::make_shared<TransformComponent>();
    // transform2->position = position;
    // manager.addComponent(plane.getID(), transform2);

    // // 2. Rendering
    // Plane planeData;
    // planeData.normal = normal;
    // planeData.position = position;
    // planeData.size = size;

    // auto renderable2 = std::make_shared<RenderableComponent>(generatePlane(planeData));
    // GLuint colorTexture2 = createColorTexture(glm::vec3(0.8f, 0.8f, 0.8f), 512, 512); // Grey
    // TextureComponent texComp2;
    // texComp2.id = colorTexture2;
    // texComp2.type = "texture_diffuse";

    // renderable2->textures.push_back(texComp2);
    // renderable2->meshes[0].textures.push_back(texComp2);

    //   manager.addComponent(plane.getID(), renderable2);
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<float> colorDist(0.2f, 1.0f);
    std::uniform_real_distribution<float> radiusDist(0.1f, 1.0f);
    std::uniform_real_distribution<float> xzDist(-6.0f, 6.0f);
    std::uniform_real_distribution<float> yDist(0.0f, 5.0f);

    // Spawn random spheres near the ground
    const int sphereCount = 40;
    for (int i = 0; i < sphereCount; ++i) {
        float radius = radiusDist(rng);
        float mass = radius * 2.0f;
        glm::vec3 pos(xzDist(rng), yDist(rng), xzDist(rng));
        glm::vec3 color(colorDist(rng), colorDist(rng), colorDist(rng));

        Entity sphere = createPhysicsSphere(manager, pos, radius, mass, color);
        physics.addRigidBody(manager.getComponent<PhysicsComponent>(sphere.getID())->body);
    }

   

    while (!window.should_close())
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        window.process_input();


        //physics.update(deltaTime);
        renderer.render();
        
        window.swap_buffers();
        window.poll_events();
    }

    window.terminate();
    return 0;
}



Entity createCapsuleEntity(EntityManager& manager, const glm::vec3& position, float radius, float height, const glm::vec3& color) {
    Entity capsule;
    manager.addEntity(capsule);

    // 1. Transform
    auto transform = std::make_shared<TransformComponent>();
    transform->position = position;
    manager.addComponent(capsule.getID(), transform);

    // 2. Rendering (basic capsule mesh + color texture)
    // auto renderable = std::make_shared<RenderableComponent>(generateCapsuleMesh(radius, height)); // You implement this
    // GLuint colorTexture = createColorTexture(color, 512, 512);
    
    // TextureComponent texComp;
    // texComp.id = colorTexture;
    // texComp.type = "texture_diffuse";

    // renderable->textures.push_back(texComp);
    // renderable->meshes[0].textures.push_back(texComp);
    // manager.addComponent(capsule.getID(), renderable);

    // 3. Physics (kinematic capsule)
    auto physics = std::make_shared<PhysicsComponent>();

    btCollisionShape* shape = new btCapsuleShape(radius, height); // Height excludes hemispheres
    btTransform startTransform;
    startTransform.setIdentity();
    startTransform.setOrigin(btVector3(position.x, position.y, position.z));

    btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, shape, btVector3(0, 0, 0));

    btRigidBody* body = new btRigidBody(rbInfo);
    body->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
    body->setAngularFactor(btVector3(0, 0, 0)); // Lock rotation

    physics->body = body;
    physics->shape = shape;
    physics->motionState = motionState;
    physics->mass = 0.0f;

    manager.addComponent(capsule.getID(), physics);

    return capsule;
}




Entity createStaticPlane(EntityManager& manager, const glm::vec3& position, const glm::vec3& normal, float size) {
    Entity plane;
    manager.addEntity(plane);


    // 1. Transform
    auto transform = std::make_shared<TransformComponent>();
    transform->position = position;
    manager.addComponent(plane.getID(), transform);

    // 2. Rendering
    Plane planeData;
    planeData.normal = normal;
    planeData.position = position;
    planeData.size = size;

    auto renderable = std::make_shared<RenderableComponent>(generatePlane(planeData));
    GLuint colorTexture = createColorTexture(glm::vec3(0.2f, 0.8f, 0.8f), 512, 512); // Grey
    TextureComponent texComp;
    texComp.id = colorTexture;
    texComp.type = "texture_diffuse";

    renderable->textures.push_back(texComp);
    renderable->meshes[0].textures.push_back(texComp);

    manager.addComponent(plane.getID(), renderable);

    // 3. Physics
    auto physics = std::make_shared<PhysicsComponent>();

    btCollisionShape* shape = new btStaticPlaneShape(
        btVector3(normal.x, normal.y, normal.z),
        0.0f // Plane constant (can adjust if needed)
    );

    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
        btQuaternion(0, 0, 0, 1),
        btVector3(position.x, position.y, position.z)
    ));

    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, shape, btVector3(0, 0, 0));
    physics->body = new btRigidBody(rbInfo);

    manager.addComponent(plane.getID(), physics);

    return plane;
}


Entity createPhysicsSphere(EntityManager& manager, const glm::vec3& position, float radius, float mass, glm::vec3 color) {
    Entity sphere;
    manager.addEntity(sphere);

    // 1. Transform
    auto transform = std::make_shared<TransformComponent>();
    transform->position = position;
    manager.addComponent(sphere.getID(), transform);

    // 2. Rendering
    Sphere sphereData;
    sphereData.position = position;
    sphereData.radius = radius;
    sphereData.segments = 64;

    auto renderable = std::make_shared<RenderableComponent>(generateSphere(sphereData));
    GLuint colorTexture = createColorTexture(color, 512, 512); // Red
    TextureComponent texComp;
    texComp.id = colorTexture;
    texComp.type = "texture_diffuse";

    renderable->textures.push_back(texComp);
    renderable->meshes[0].textures.push_back(texComp);

    manager.addComponent(sphere.getID(), renderable);

    // 3. Physics (only if you have a PhysicsComponent setup)
    auto physics = std::make_shared<PhysicsComponent>();

    btCollisionShape* shape = new btSphereShape(radius);
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(
        btQuaternion(0, 0, 0, 1),
        btVector3(position.x, position.y, position.z)
    ));

    btVector3 inertia(0, 0, 0);
    if (mass != 0.0f) {
        shape->calculateLocalInertia(mass, inertia);
    }

    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    physics->body = new btRigidBody(rbInfo);

    manager.addComponent(sphere.getID(), physics);

    return sphere;
}