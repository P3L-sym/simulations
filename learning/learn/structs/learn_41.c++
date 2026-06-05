#include <iostream>

struct student{
    int id;
    std::string name;
    double grade;
    //if you set a vaue in your struct you dont need to state the value 
    //so if i state
    //bool enrolled = true 
    //student1.enrolled = true; these can be removed 
    bool enrolled = true;
};

int main(){
    student student1;
    student1.id = 1;
    student1.name = "Sponge-Bob";
    student1.grade = 75.65;
    // student1.enrolled = true;

    student student2;
    student2.id = 2;
    student2.name = "Patrick-Star";
    student2.grade = 45.21;
    student2.enrolled = false;

    student student3;
    student3.id = 3;
    student3.name = "Sandy-Cheeks";
    student3.grade = 90.18;
    // student3.enrolled = true;

    std::cout << "#" << student1.id << ": " << student1.name << '\n' << student1.grade << '\n' << student1.enrolled << '\n';
    std::cout << '\n';
    std::cout << "#" << student2.id << ": " << student2.name << '\n' << student2.grade << '\n' << student2.enrolled << '\n';
    std::cout << '\n';
    std::cout << "#" << student3.id << ": " << student3.name << '\n' << student3.grade << '\n' << student3.enrolled << '\n';

    return 0;
}