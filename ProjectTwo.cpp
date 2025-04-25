//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Audrey Saidel
// Version     : 1.0
// Description : Final Project 7-2
//============================================================================

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

struct Course {
    string courseId;
    string name;
    vector<string> prerequisites;
};
// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};
//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);
    void destroyTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
    
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    destroyTree(root);
    root = nullptr;

}
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) 
    {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}
/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // In order root
    // call inOrder fuction and pass root 
    inOrder(root);

}

/**
 * Insert a Course
 */
void BinarySearchTree::Insert(Course course) {
    // Implement inserting a course into the tree
    // if root equal to null ptr
    if (root == nullptr)
        root = new Node(course);  // root is equal to new node course
    // else
    else
        this->addNode(root, course); // add Node root and course
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // Implement removing a course from the tree
    // remove node root courseID
    this->removeNode(root, courseId);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // Implement searching the tree for a course
    // set current node equal to root
    Node* curNode = root;
    while (curNode != nullptr) {   // keep looping downwards until bottom reached or matching courseId found
        if (curNode->course.courseId.compare(courseId) == 0)  // if match found, return current course
            return curNode->course;
        else if (curNode->course.courseId.compare(courseId) < 0) // if course is larger than current node then traverse right
            curNode = curNode->right;
        else   // else smaller  so traverse left
            curNode = curNode->left;
    }

    Course course;
    return course;
}
/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // Implement removing a bid from the tree
    // if node = nullptr return node
    if (node == nullptr)
        return node;
    else if (courseId.compare(node->course.courseId) < 0) { // otherwise recurse down the left subtree
        node->left = removeNode(node->left, courseId); // check for match and if so, remove left node using recursive call 
    }
    else if (courseId.compare(node->course.courseId) > 0) {  // otherwise recurse down the right subtree
        node->right = removeNode(node->right, courseId); // check for match and if so, remove right node using recursive call
    }
    else { // found match
        if (node->left == nullptr && node->right == nullptr) {
            delete node; node = nullptr; //leaf node
        }
        else if (node->left != nullptr && node->right == nullptr) { // if only child is on the left
            Node* temp = node;
            node = node->left; // replace parent to remove w/ left child
            delete temp;
            temp = nullptr;
        }
        else if (node->left == nullptr && node->right != nullptr) { // if only child is on the right
            Node* temp = node;
            node = node->right; // replace parent to remove w/ right child 
            delete temp;
            temp = nullptr;
        }
        else {
            // find min on right
            Node* temp = node->right;
            while (temp->left != nullptr) {     // while left node is not nullptr keep moving temp left
                temp = temp->left;
            }
            node->course = temp->course;   // make node course (right) equal to temp course (left)
            removeNode(node->right, temp->course.courseId);   // remove right node using recursive call
        }
    }
    return node;
}
/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param course Course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Implement inserting a course into the tree
   // if node is larger then add to left
    if (node != nullptr && node->course.courseId.compare(course.courseId) > 0) {
        if (node->left == nullptr) {  // if no left node
            node->left = new Node(course);  // this node becomes left
            return;
        }
        else {
            this->addNode(node->left, course); // else recurse down the left node
        }
    }
    else if (node != nullptr && node->course.courseId.compare(course.courseId) < 0) {
        if (node->right == nullptr) { // if no right node
            node->right = new Node(course);    // this node becomes right
            return;
        }
        else {
            this->addNode(node->right, course);  // recurse down
        }
    }
}
void BinarySearchTree::inOrder(Node* node) {
    // In order root
    //if node is not equal to null ptr
    if (node != nullptr)
    {
        inOrder(node->left); // Deal with left and then right
        cout << node->course.courseId << ": " << node->course.name << " | ";
        for (int i = 0; i < node->course.prerequisites.size(); i++) {
            cout << node->course.prerequisites.at(i) << " | ";
        }
        cout << endl;
        inOrder(node->right);
    }
}

void loadCourses(string filename, BinarySearchTree* courses) {
    ifstream file(filename);

    // If file is not open
    if (!file.is_open()) {
        cout << "Error: File " << filename << " was not opened." << endl;
        exit(1);
    }

    string line;

    // Read each line
    while (getline(file, line)) 
    {
        vector<string> courseInfo;
        stringstream ss(line);
        string token;

        // Split line by commas
        while (getline(ss, token, ',')) 
        {
            courseInfo.push_back(token);
        }

        // If less than 2 strings on a line, print error
        if (courseInfo.size() < 2) 
        {
            cout << "Error: Incorrect file format in line: " << line << endl;
            exit(1);
        }

        // Create and fill course 
        Course course;
        course.courseId = courseInfo.at(0);
        course.name = courseInfo.at(1);

        for (unsigned int i = 2; i < courseInfo.size(); ++i) 
        {
            course.prerequisites.push_back(courseInfo.at(i));
        }

        // Insert course into tree
        courses->Insert(course);
    }

    file.close();
}

/*
* Print a singular course
*/
void printCourse(BinarySearchTree* courses, string courseId) {
    Course found = courses->Search(courseId);

    if (found.courseId.empty()) 
    {
        cout << "Course not found.\n";
        return;
    }
    else
    {
        cout << found.courseId << ": " << found.name << " | ";
        for (int i = 0; i < found.prerequisites.size(); i++) {
            cout << found.prerequisites.at(i) << " | ";
        }
        cout << endl;
    }
}
int main()
{
    // Define a binary search tree to hold all courses
   BinarySearchTree* courses;
    courses = new BinarySearchTree();
    Course course;

    string courseId;

    // Main Menu
    int choice = 0;
    while (choice != 9)
    {
        cout << "Main Menu\n";
        cout << "1. Load Courses\n";
        cout << "2. Display Ordered List of Courses\n";
        cout << "3. Display Specific Course\n";
        cout << "9. Exit\n";
        cin >> choice;
        switch (choice) 
        {
            case 1:
                //loadCourses
                loadCourses("courselist.csv", courses);
                break;

            case 2:
                courses->InOrder();
                break;

            case 3:
                cout << "Enter the courseId you want to search for\n";
                cin >> courseId;
                printCourse(courses, courseId);
                break;

            case 9:
                exit;
                break;

        }

    }
}

