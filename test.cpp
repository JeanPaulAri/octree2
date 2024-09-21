#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <stack>
#include <sstream>

using namespace std;

struct Point {
    double x,y,z;

    Point(double x, double y, double z) : x(x), y(y), z(z){}
    Point() : x(0), y(0), z(0){}
};

auto readCSV = [](const string& file)-> vector<Point>{

    ifstream in(file);
    vector<Point> points;

    if(!in.is_open()){
        cout << "No se pudo abrir el archivo.\n";
        return points;
    }

    string line;
    while(getline(in, line)){

        stringstream ss(line);
        string token;
        vector<double> tokens;

        while(getline(ss, token, ',')){
            double token_value = stod(token);
            tokens.push_back(token_value);
        }

         if(tokens.size() == 3){
            points.push_back(Point(tokens[0], tokens[1], tokens[2]));
        }else{
            cerr << "Error en el formato del archivo. Se necesita que cada punto tenga X Y Z\n";
            // return points;
        }
        // points.push_back(Point(stod(tokens[0]), stod(tokens[1]), stod(tokens[2]));
    }
    in.close();
    return points;
};


class Octree {

    private:
        Octree *children[8];
        bitset<1000> existences;
        Point *points;
        Point bottomLeft;
        double h;
        int nPoints;
    
    public:

        Octree(Point bl, double height) : bottomLeft(bl), h(height), nPoints(0){

            for(int i = 0; i < 8; i++){
                children[i] = nullptr;
            }
            points = nullptr;
        }

        bool exist(const Point &p){
            int index = p.x * 100 + p.y * 10 + p.z;
            return existences.test(index);
        }

        void insert(const Point &p){
            //caso base
            if (p.x < bottomLeft.x || p.x >= bottomLeft.x + h ||
                p.y < bottomLeft.y || p.y >= bottomLeft.y + h ||
                p.z < bottomLeft.z || p.z >= bottomLeft.z + h) {
                cout << "El punto está fuera del espacio del octree.\n";
                return;
            }   

            if(!exist(p)){
                existences.set(p.x * 100 + p.y * 10 + p.z);

                if( h<= 1.0){
                    if(points == nullptr){
                        points = new Point[1]{Point(p.x, p.y, p.z)};   
                    }else{
                        Point *newPoints = new Point[nPoints + 1];
                        for(int i = 0; i < nPoints; i++){
                            newPoints[i] = points[i];
                        }
                        newPoints[nPoints] = Point(p.x, p.y, p.z);
                        //nPoints++;
                        delete[] points;
                        points = newPoints;
                    }
                    nPoints++;
                }

            }
        }

        Point find_closest( const Point &, int radius);

};

int main(){

    Octree *octree = new Octree(Point(0,0,0), 1000);


    return 0;
}