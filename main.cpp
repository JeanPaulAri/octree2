#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <fstream>
#include <stack>
#include <sstream>
#include <cmath>
#include <limits>

using namespace std;

struct Point {
    double x,y,z;

    Point(double x, double y, double z) : x(x), y(y), z(z){}
    Point() : x(0), y(0), z(0){}
};

auto printPoint = [](const Point &p) ->void {
    cout << "Point created-> (" << p.x << ", " << p.y << ", " << p.z << ")\n";
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

         int calculate_index(const Point &p) {
            cout<<"calculate index"<<endl;
            int index = (static_cast<int>(p.x) * 100 + static_cast<int>(p.y) * 10 + static_cast<int>(p.z)) % 1000;
            return index;
        }

        bool exist(const Point &p){
            cout<<"exist"<<endl;
            int index = calculate_index(p);
            return existences.test(index);
        }

        void insert(const Point &p){

            cout<<"entre al insert"<<endl;
            //caso base
            cout << "Insertando punto (" << p.x << ", " << p.y << ", " << p.z << ")\n";
            if (p.x < bottomLeft.x || p.x >= bottomLeft.x + h ||
                p.y < bottomLeft.y || p.y >= bottomLeft.y + h ||
                p.z < bottomLeft.z || p.z >= bottomLeft.z + h) {
                cout << "El punto esta fuera del espacio del octree.\n";
                return;
            } 


            cout << "Calculando indice...\n";

            int index = calculate_index(p);
            if (existences.test(index)) {
                cout << "El punto ya existe en el octree.\n";
                return;
            }
            // Insertar el punto en el octree
                cout << "Dividiendo nodo...\n";
                // Verificar si el nodo actual ya esta al maximo nivel de detalle
                if( h <= 1.0){
                    if (points == nullptr){
                        points = new Point[1];
                    } else {
                        Point *newPoints = new Point[nPoints + 1];
                        copy(points, points + nPoints, newPoints);
                        delete[] points;
                        points = newPoints;
                    }
                    points[nPoints++] = p;
                    existences.set(index);

                    return;
                } 
                
                
                

                    // Si no estamos en el nivel maximo de detalle, dividimos el espacio y recursivamente insertamos el punto en el hijo correspondiente
                    if(children[0] == nullptr){
                        double newH = h/ 2.0;
                        for (int i = 0; i < 8; i++){
                            double x = bottomLeft.x + (i & 1) * newH;
                            double y = bottomLeft.y + ((i >> 1) & 1) * newH;
                            double z = bottomLeft.z + ((i >> 2) & 1) * newH;
                            children[i] = new Octree(Point(x, y, z), newH);
                        }
                    }

                    int childIndex = (p.x >= bottomLeft.x + h / 2) + 2 * (p.y >= bottomLeft.y + h / 2) + 4 * (p.z >= bottomLeft.z + h / 2);
                    children[childIndex]->insert(p);
            
                
            
            
        }

        Point find_closest( const Point &target, int radius){
            if(points == nullptr){
                throw runtime_error("No hay puntos en el octree");
            }
            Point closest = points[0];
            double minDist = numeric_limits<double>::max();

            auto distance = [](const Point &a, const Point &b) -> double {
                return sqrt(
                    (a.x - b.x) * (a.x - b.x) + 
                    (a.y - b.y) * (a.y - b.y) + 
                    (a.z - b.z) * (a.z - b.z));
            };

            for(int i = 0; i<nPoints; i++){
                double dist = distance(points[i], target);

                
                if(dist < minDist){
                    minDist = dist;
                    closest = points[i];
                }
            }

            if(children[0] != nullptr) {
                for(int i = 0 ; i < 8; i++){
                    if(children[i] != nullptr){
                        try {
                            cout<<"PEP"<<endl;
                            Point child_closest = children[i]->find_closest(target, radius);
                            double d = distance(child_closest, target);
                            if( d < minDist && d < radius){
                                minDist = d;
                                closest = child_closest;
                            }
                        }
                        catch(const runtime_error &e){
                            cout << e.what() << endl;
                        }
                    }
                }
            }
        }

};

int main(){
    cout<<"PEPE"<<endl;
    Octree *octree = new Octree(Point(0,0,0), 1000);
     
    string filename = "points1.csv";
    vector<Point> points = readCSV(filename);

    // Insertar los puntos en el octree
    for (const auto& point : points) {
        octree->insert(point);
    }
   
    return 0;
}