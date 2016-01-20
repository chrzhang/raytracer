#include "PLYReader.hpp"
#include <vector>
#include "Object.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "Triangle.hpp"
#include "Vector3D.hpp"
#include "Helper.hpp"

std::vector<Object *> PLYReader::readFromPly(const std::string & filename) {
    std::vector<Object *> finalObjs;
    std::ifstream fileIn(filename);
    std::string currLine;
    getline(fileIn, currLine);
    assert(currLine.compare("ply") == 0);
    getline(fileIn, currLine);
    assert(currLine.compare("format ascii 1.0") == 0);
    std::string currWord;
    fileIn >> currWord;
    assert(currWord.compare("element") == 0);
    fileIn >> currWord;
    assert(currWord.compare("vertex") == 0);
    fileIn >> currWord;
    std::string::size_type sz;
    int numVertices = std::stoi(currWord, &sz);
    std::cout << "num vertices = " << numVertices << std::endl;
    getline(fileIn, currLine);
    getline(fileIn, currLine);
    assert(currLine.compare("property float x") == 0);
    getline(fileIn, currLine);
    assert(currLine.compare("property float y") == 0);
    getline(fileIn, currLine);
    assert(currLine.compare("property float z") == 0);
    fileIn >> currWord;
    assert(currWord.compare("element") == 0);
    fileIn >> currWord;
    assert(currWord.compare("face") == 0);
    fileIn >> currWord;
    int numFaces = std::stoi(currWord, &sz);
    std::cout << "num faces = " << numFaces << std::endl;
    getline(fileIn, currLine);
    getline(fileIn, currLine);
    getline(fileIn, currLine);
    assert(currLine.compare("end_header") == 0);
    std::vector<Vector3D> vertices;
    for (int i = 0; i < numVertices; ++i) {
        getline(fileIn, currLine);
        std::cout << "vertex: " << currLine << std::endl;
        std::istringstream iss(currLine);
        iss >> currWord;
        auto x = std::stod(currWord, &sz);
        iss >> currWord;
        auto y = std::stod(currWord, &sz);
        iss >> currWord;
        auto z = std::stod(currWord, &sz);
        vertices.push_back(Vector3D(x, y, z));
        std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
    }
    for (int i = 0; i < numFaces; ++i) {
        getline(fileIn, currLine);
        std::cout << "face: " << currLine << std::endl;
        std::istringstream iss(currLine);
        iss >> currWord;
        assert(currWord.compare("3") == 0);
        iss >> currWord;
        int iv1 = std::stoi(currWord, &sz);
        iss >> currWord;
        int iv2 = std::stoi(currWord, &sz);
        iss >> currWord;
        int iv3 = std::stoi(currWord, &sz);
        std::cout << "iv1: " << iv1 << " iv2: " << iv2 << " iv3: " << iv3 << std::endl;
        Triangle * t = new Triangle(vertices[iv1], vertices[iv2], vertices[iv3], randColor());
        finalObjs.push_back(dynamic_cast<Object *>(t));
        std::cout << "pushed back\n";
    }
    return finalObjs;
}
