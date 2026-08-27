#include <Siv3D.hpp>
#include <string>
#include <vector>
#include <random>
#include <memory>

class Circles{
public:
    Vec2 pos;
    std::vector<int> Monitor_size;
    int MoveSpeed = 10;
    int Circle_size = 30;
    Circles(std::vector<int> Monitor_size) : Monitor_size{Monitor_size}{
        pos = {random_pos(Monitor_size[0]) , random_pos(Monitor_size[1])};
        Circle(pos , Circle_size).draw();
    }
    ~Circles(){}
    int random_pos(int max_pos){
        static std::random_device seed_gen;
        static std::mt19937 engine(seed_gen());
        std::uniform_int_distribution<int> dist(0 , max_pos - 1);
        return dist(engine);
    }
    bool Moving(){
        pos.y += MoveSpeed;
        Circle(pos , Circle_size).draw();
        if(pos.y + 15 > Monitor_size[1]){
            return true;
        }
        return false;
    }
};

void Main(){
    Scene::SetBackground(HSV(0 , 0.3 , 1));
    std::vector<int> Monitor_size = {800 , 800};
    Window::Resize(Monitor_size[0] , Monitor_size[1]);
    const unsigned int Max = 15;
    std::vector<std::unique_ptr<Circles>> CircleVec;
    while(System::Update()){
        if(CircleVec.size() > 0){
            for(int i = CircleVec.size() - 1; i >= 0; i--){
                if(CircleVec[i] -> Moving()){
                    CircleVec.erase(CircleVec.begin() + i);
                }
            }
            if(CircleVec.size() < Max){
                std::unique_ptr<Circles> Circle_temp = std::make_unique<Circles>(Monitor_size);
                CircleVec.push_back(std::move(Circle_temp));
            }
        }else{
            std::unique_ptr<Circles> Circle_temp = std::make_unique<Circles>(Monitor_size);
            CircleVec.push_back(std::move(Circle_temp));
        }
    }
}
