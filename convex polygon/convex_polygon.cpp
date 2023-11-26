#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

// define class point with coordinates
class Point
{ 
public:
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

// define class polygon
class Polygon 
{
private:
    vector<Point> vertices;
    //bool edgeIntersects(const Point& p1, const Point& p2, const Point& q1, const Point& q2) const 


public:
    Polygon(const string& line) 
    {
        istringstream iss(line);
        int n;
        char brace;
        int x, y;

        iss >> n >> brace; // Reading the number of vertices and the opening brace

        for (int i = 0; i < n; ++i) 
        {
            iss >> x >> y;
            vertices.push_back(Point(x, y));
        }
    }
    float area() const 
    {
        float area = 0.0; // area as float number
        int n = vertices.size(); // amount of vertices

        for (int i = 0; i < n; ++i) 
        {
            int j = (i + 1) % n; // next vertex
            area += (vertices[i].x * vertices[j].y) - (vertices[j].x * vertices[i].y); // formula for convex polygon area
        }

        return abs(area / 2.0);
    }

    enum Orientation { COLLINEAR, CLOCKWISE, COUNTERCLOCKWISE }; 
    Orientation orientation(const Point& p, const Point& q, const Point& r) // check orientation and direction 
    {
        double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0) return COLLINEAR; 
        return (val > 0) ? CLOCKWISE : COUNTERCLOCKWISE;
    }

    bool onSegment(const Point& p, const Point& q, const Point& r)
    {
        return q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y); // output is T/F
    }

    bool isintersect(const Point& p1, const Point& q1, const Point& p2, const Point& q2) // four orientations are necessary for eneral and special cases
    {
        Orientation o1 = orientation(p1, q1, p2);
        Orientation o2 = orientation(p1, q1, q2);
        Orientation o3 = orientation(p2, q2, p1);
        Orientation o4 = orientation(p2, q2, q1);
        
        // general case
        if (o1 != o2 && o3 != o4) // if orientations are different => they intersect
            return true;

         // some special cases (check the colliearity)
        if (o1 == COLLINEAR && onSegment(p1, p2, q1)) // p1, q1, and p2 are collinear and p2 lies on segment p1q1
            return true; 
        if (o2 == COLLINEAR && onSegment(p1, q2, q1))  // p1, q1, and q2 are collinear and q2 lies on segment p1q1
            return true;
        if (o3 == COLLINEAR && onSegment(p2, p1, q2)) // p2, q2, and p1 are collinear and p1 lies on segment p2q2
            return true; 
        if (o4 == COLLINEAR && onSegment(p2, q1, q2)) // p2, q2, and q1 are collinear and q1 lies on segment p2q2
            return true; 
        
        return false;
    }

    bool intersects(const Polygon& other) const // check if any edge of the polygon intersects with any other edge
    {
        for (int i = 0; i < vertices.size(); ++i)
        {
            int next_i = (i + 1) % vertices.size();
            for (int j = 0; j < other.vertices.size(); ++j)
            {
                int next_j = (j + 1) % other.vertices.size();
                if (isintersect(vertices[i], vertices[next_i], other.vertices[j], other.vertices[next_j]))
                {
                    return true; // not convex polygon and nothing to do
                }
            }
        }
        return false; // it's convex polygon and they don't intersect 
    }

    void print() const // function with vertices of the point of polygon as output
    {
        cout << "Polygon with vertices: ";
        for (const auto& vertex : vertices) 
        {
            cout << "(" << vertex.x << ", " << vertex.y << ") ";
        }
        cout << endl;
    }
};

// parse&read poly*.txt 
vector<Polygon> readPolygonsFromFile(const string& filename) 
{
    vector<Polygon> polygons;
    ifstream file(filename);
    string line;

    if (!file.is_open())  // error handling (or we can use try/catch)
    {
        throw runtime_error("Unable to open file"); 
    }

    while (getline(file, line)) // parse the lines, while they exist
    {
        Polygon polygon(line);
        polygons.push_back(polygon);
    }

    return polygons;
}

void processCommands(const vector<Polygon>& polygons)
{
    float totalArea = 0.0;
    double maxArea = 0.0; // somewhy doesn't work with float, but works with double
    double minArea = numeric_limits<double>::max(); // the same
    int intersectCount = 0;

    for (const auto& polygon : polygons) 
    {
        double area = polygon.area();
        totalArea += area;
        maxArea = max(maxArea, area);
        minArea = min(minArea, area);
    }

    cout << "AVG: " << (polygons.empty() ? 0 : totalArea / polygons.size()) << endl;
    cout << "SUM: " << totalArea << endl;
    cout << "MAX: " << maxArea << endl;
    cout << "MIN: " << (polygons.empty() ? 0 : minArea) << endl;

    int polygonIndex = 1; // check intersection with the 1st polygon (not general solution)
    for (int i = 0; i < polygons.size(); ++i) 
    {
        if (i != polygonIndex && polygons[i].intersects(polygons[polygonIndex])) 
        {
            intersectCount++;
        }
    }
    cout << "INTERSECT: " << intersectCount << " polygons intersect with polygon number " << 1 <<endl; // notgeneral solution
}

int main() 
{
    try // check if file can be opened or not (else we can use (!file.open("poly*.txt"))...)
    {
        auto polygons = readPolygonsFromFile("poly*.txt"); // in my case: poly3.txt, poly4.txt, poly5.txt
        for (const auto& polygon : polygons) 
        {
            polygon.print();
        }
    }
    catch (const exception& e) // error handling 
    {
        cerr << "file is not found. Error: " << e.what() << endl;
        exit;
    }

    try // check the correctness of processing of commands (if the file is opened)
    {
        auto polygons = readPolygonsFromFile("poly*.txt");
        processCommands(polygons); // check all commands together
    }
    catch (const exception& e) 
    {
        cerr << "commands don't work correctly. Error: " << e.what() << endl;
    }

    return 0;
}
