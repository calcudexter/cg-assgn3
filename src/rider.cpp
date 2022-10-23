#include "rider.hpp"

using namespace std;
using namespace csX75;

Human::Human()
{
    // Creating the torso
    Cuboid cuboid(7, 10, 3, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
    torso = new HNode(NULL, cuboid.num_vertices, cuboid.vert_arr, cuboid.col_arr, cuboid.num_vertices*sizeof(glm::vec4), cuboid.num_vertices*sizeof(glm::vec4), "Torso");

    // Creating the limbs
    Cylinder limb(6, 1, 10, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));   

    left_arm[0] = new HNode(torso, limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "UArmL");
    left_arm[0]->change_parameters(5, 2.0, 0.0, 90.0, 0.0, 0.0, -5.0, -5.0, 0.0);

    left_arm[1] = new HNode(left_arm[0], limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "LArmL");
    left_arm[1]->change_parameters(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, -3.0);

    right_arm[0] = new HNode(torso, limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "UArmR");
    right_arm[0]->change_parameters(-5.0, 2.0, 0.0, 90.0, 0.0, 0.0, 5.0, -5.0, 0.0);

    right_arm[1] = new HNode(right_arm[0], limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "LArmR");
    right_arm[1]->change_parameters(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, -3.0);

    left_leg[0] = new HNode(torso, limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "ThighL");
    left_leg[0]->change_parameters(2, -8.0, 0.0, 90.0, 0.0, 0.0, -5.0, 5.0, 0.0);

    left_leg[1] = new HNode(left_leg[0], limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "LegL");
    left_leg[1]->change_parameters(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, -3.0);

    Sphere shoe(1.5, 10, glm::vec4(1.0f, 0.8f, 0.0f, 1.0f));
    shoes[0] = new HNode(left_leg[1], shoe.num_vertices, shoe.vert_arr, shoe.col_arr, shoe.num_vertices*sizeof(glm::vec4), shoe.num_vertices*sizeof(glm::vec4), "ShoeL");
    shoes[0]->change_parameters(0.0, 0.0, 3.75, 0.0, 0.0, 0.0);

    right_leg[0] = new HNode(torso, limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "ThighR");
    right_leg[0]->change_parameters(-2.0, -8.0, 0.0, 90.0, 0.0, 0.0, 5.0, 5.0, 0.0);

    right_leg[1] = new HNode(right_leg[0], limb.num_vertices, limb.vert_arr, limb.col_arr, limb.num_vertices*sizeof(glm::vec4), limb.num_vertices*sizeof(glm::vec4), "LegR");
    right_leg[1]->change_parameters(0.0, 0.0, 6.0, 0.0, 0.0, 0.0, 0.0, 0.0, -3.0);
    shoes[1] = new HNode(right_leg[1], shoe.num_vertices, shoe.vert_arr, shoe.col_arr, shoe.num_vertices*sizeof(glm::vec4), shoe.num_vertices*sizeof(glm::vec4), "ShoeR");
    shoes[1]->change_parameters(0.0, 0.0, 3.75, 0.0, 0.0, 0.0);


    // Creating the neck
    Cylinder gardan(1, 1.5, 10, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
    neck = new HNode(torso, gardan.num_vertices, gardan.vert_arr, gardan.col_arr, gardan.num_vertices*sizeof(glm::vec4), gardan.num_vertices*sizeof(glm::vec4), "Neck");
    neck->change_parameters(0.0, 5.5, 0.0, 90.0, 0.0, 0.0, 0.0, -5.0, 0.0);

    // Creating the face
    Sphere sphere(2, 10, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    head = new HNode(neck, sphere.num_vertices, sphere.vert_arr, sphere.col_arr, sphere.num_vertices*sizeof(glm::vec4), sphere.num_vertices*sizeof(glm::vec4), "Head");
    head->change_parameters(0.0, 0.0, -2.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.5);

    Cylinder cap0(0.2, 3, 10, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    cap[0] = new HNode(head, cap0.num_vertices, cap0.vert_arr, cap0.col_arr, cap0.num_vertices*sizeof(glm::vec4), cap0.num_vertices*sizeof(glm::vec4), "CapRim");
    cap[0]->change_parameters(0.0, 0.0, -1.5, 0.0, 0.0, 0.0);

    Cylinder cap1(2, 2, 10, glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    cap[1] = new HNode(cap[0], cap1.num_vertices, cap1.vert_arr, cap1.col_arr, cap1.num_vertices*sizeof(glm::vec4), cap1.num_vertices*sizeof(glm::vec4), "Cap");
    cap[1]->change_parameters(0.0, 0.0, -1.1, 0.0, 0.0, 0.0);
}
Human::~Human() {
    delete this->left_arm[0], this->left_arm[1];
    delete this->left_leg[0], this->left_leg[1];
    delete this->right_arm[0], this->right_arm[1];
    delete this->right_leg[0], this->right_leg[1];
    delete this->torso, this->neck, this->head;
    delete this->cap[0], this->cap[1];
}

void Human::print_rot()
{
    this->torso->print_rot();
    this->left_arm[0]->print_rot();
    this->left_arm[1]->print_rot();
    this->right_arm[0]->print_rot();
    this->right_arm[1]->print_rot();
    this->left_leg[0]->print_rot();
    this->left_leg[1]->print_rot();
    this->right_leg[0]->print_rot();
    this->right_leg[1]->print_rot();
}

void Human::init_arrange()
{
    torso->init_rot(3.036873, 0.942478, -1.570796);
    left_arm[0]->init_rot(-0.733038, -0.000000, 0.000000);
    left_arm[1]->init_rot(-1.884956, -0.000000, 0.000000);
    right_arm[0]->init_rot(-0.733038, -0.000000, 0.000000);
    right_arm[1]->init_rot(-1.780236, -0.000000, 0.000000);
    left_leg[0]->init_rot(-1.361357, -0.000000, 0.000000);
    left_leg[1]->init_rot(1.884956, -0.000000, 0.000000);
    right_leg[0]->init_rot(-1.361357, -0.000000, 0.000000);
    right_leg[1]->init_rot(1.884956, -0.000000, 0.000000);
}