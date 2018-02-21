#ifndef CALCULATEFORCES_H
#define CALCULATEFORCES_H

class GraphicScene;

class CalculateForces
{
public:
    CalculateForces(GraphicScene& graphicScene);

    void operator()();

private:
    GraphicScene& scene;
};

#endif // CALCULATEFORCES_H
